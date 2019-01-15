// Copyright (c) 2018 Fabio Polimeni
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "angie/core/config.hpp"
#include "angie/core/strings/dynamic_string.hpp"

#include <cstdarg>
#include <cstring>

#define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"

namespace {

	using namespace angie::core;
	using namespace angie::core::strings;

	thread_local const dynamic_string<const types::char8>* g_format_string = nullptr;
	
	alignas(ANGIE_DEFAULT_MEMORY_ALIGNMENT)
	thread_local types::char8 g_format_heap[
		sizeof(dynamic_string<types::char8>) + ANGIE_MAX_FORMAT_CHARS] = {0};

	void* format_alloc(types::size size, types::size alignment) {
		memory::set(g_format_heap, 0, sizeof(g_format_heap));
		return g_format_heap;
	}

	void format_dealloc(void* pointer) {

	}

	void* format_realloc(void* ptr, types::size sz, types::size al = 0) {
		return ptr;
	}

	const memory::allocator g_thread_format_alloc = {
			format_alloc,
			format_dealloc,
			format_realloc,
	};

	/**
	 * Create a string on the heap.
	 * 
	 * If the string won't be reallocated, its access
	 * will be cache-friendly, as the buffer resides on
	 * the same chunk of memory of the object instance.
	 * 
	 * +---------+-----+--------+---+----..-+
	 * |allocator|count|capacity|pad|data.. |
	 * +---------+-----+--------+---+----..-+
	 */
	template<typename T>
	const dynamic_string<T>* create_string(types::size capacity, const memory::allocator& allocator) {
		const types::size obj_size = sizeof(dynamic_string<T>);

		// Because sizeof char is 1, buffer size and capacity coincide.
		const types::size buffer_size = utils::power_of_two_ceil(capacity);

		const types::size alloc_size = obj_size + buffer_size;
		void* heap_space = allocator.alloc(alloc_size,
			buffers::get_type_alignment<types::char8>());

		types::char8* buffer_data = static_cast<types::char8*>(heap_space)
			+ alloc_size - buffer_size;

		return new(heap_space) dynamic_string<const types::char8> {
		 	allocator, 0, buffer_size, buffer_data
		};
	}

	/**
	 * Destroy the object allocated by `create(...)`.
	 */
	// void destroy_string(ansi_string* str) {
	// 	if (str) {
	// 		str->allocator.free(str);
	// 	}
	// }

    // char* stb_callback(char *buf, void *user, int len) {
    //     return buf;
    // }

}

namespace angie {
    namespace core {
        namespace strings {

			const dynamic_string<const types::char8>& format(const types::char8* fmt, ...) {

				// This string will be allocated on the thread stack
				auto string_result = create_string<const types::char8>(
					ANGIE_MAX_FORMAT_CHARS, g_thread_format_alloc);

                if (string_result) {
                    va_list list;
                    va_start(list, fmt);
					
					const auto written_chars = stbsp_vsnprintf(
						const_cast<types::char8*>(strings::cstr(*string_result)),
						buffers::get_capacity(*string_result), fmt, list);

						buffers::set_count(*(const_cast<dynamic_string<const types::char8>*>(
							string_result)), written_chars + 1);

					va_end(list);
                }

				g_format_string = string_result;
                return *g_format_string;
            }

        }
    }
}