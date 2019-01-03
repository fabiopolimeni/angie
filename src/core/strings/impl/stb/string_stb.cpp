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
    
    char* stb_callback(char *buf, void *user, int len) {
        return buf;
    }

}

namespace angie {
    namespace core {
        namespace strings {

            /**
			 * Create a string on the heap.
			 * 
			 * If the string won't be reallocated, its access
			 * will be cache-friendly, as the buffer data is
			 * allocated on the same linear chunk of memory
			 * of the the object.
			 * 
			 * +---------+-----+--------+--------+
			 * |allocator|count|capacity|data ...|
			 * +---------+-----+--------+--------+
			 */
			const ansi_string* create_ansi_string(types::size length,
				const memory::allocator& allocator = memory::get_default_allocator()) {
				const types::size obj_size = sizeof(ansi_string);
				const types::size buffer_capacity = buffers::compute_capacity(length);
				const types::size buffer_size = buffers::compute_size<types::char8>(buffer_capacity);

				const types::size alloc_size = obj_size + buffer_size;
				void* heap_space = allocator.alloc(alloc_size,
					buffers::get_type_alignment<types::char8>());

				types::char8* buffer_data = static_cast<types::char8*>(heap_space) + alloc_size - buffer_size;
				
				ansi_string* str_result = new(heap_space) ansi_string {
					allocator, length, buffer_capacity, buffer_data
				};

				return str_result;
			}

			/**
			 * Destroy the object allocated by `create(...)`.
			 */
			void destroy_ansi_string(ansi_string* str) {
				if (str) {
					str->allocator.free(str);
				}
			}

			const ansi_string& format(const types::char8* fmt, ...) {
                const ansi_string* ansi = strings::create_ansi_string(
                    ANGIE_MAX_FORMAT_CHARS, memory::get_default_allocator());
                    
                if (ansi) {
                    va_list list;
                    va_start(list, fmt);
                    stbsp_vsnprintf(strings::cstr(*ansi), strings::length(*ansi), fmt, list);
                    va_end(list);
                }

                return *ansi;
            }

        }
    }
}