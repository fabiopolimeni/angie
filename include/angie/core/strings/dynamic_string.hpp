// Copyright (c) 2018 Fabio Polimeni
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "angie/core/containers/dynamic_array.hpp"

namespace angie {
	namespace core {
		namespace strings {

            /**
			 * Template base for string implementation.
			 *
			 * Functions operating on this structure, assume T is a POD type.
			 * It is responsibility of the user to guarantee that the given
			 * dynamic_string object passed to the hereafter functions,
			 * is in a `ready` state.
			 *
			 * The allocator is not part of the object type. It would have
			 * limited the usage of the class, while we want to leave the user
			 * to decide which one is the most appropriate at any given time.
			 * The allocator cannot be null, as this class is meant for heap
             * array allocation.
			 *
			 * @tparam T it must be of a POD type.
			 */
			template <typename T>
			struct dynamic_string : containers::dynamic_array<T> {
				
				// This operator allows to use the string class
				// with lvalues of const T* type, such const char*.
				operator const T*() const { return cstr(*this); }
			};

			/**
			 * Get the C string pointer.
			 */
			template <typename T>
			inline T* cstr(const dynamic_string<T>& str) {
				return buffers::get_data(str);
			}

			/**
			 * Get the length of the string.
			 */
			template <typename T>
			inline types::size length(const dynamic_string<T>& str) {
				return buffers::get_count(str);
			}

			/**
			 * ANSI string
			 */
			using ansi_string = dynamic_string<types::char8>;

			/**
			 * Format a string from the given arguments.
			 * Formatting rules are the same as the C standard library.
			 * 
			 * @note: This function returns a global string allocated
			 * in thread space. Even though you can't manipulate the
			 * memory returned, you maybe tempted to pass the returned
			 * reference to another thread. This can result in unexpected
			 * results, as the memory referenced by the resulted string
			 * can be changed underneath by the thread who owns the memory.
			 * Therefore, if you want to use the string outside the scope
			 * of the current thread who has generated the string, copy
			 * its content into a new string object and use that instead.
			 * 
			 * @param msg String to format
			 * @return Formatted string
			 */
			const ansi_string& format(const types::char8* fmt, ...);

			/**
			 * UTF8 string
			 */
			using utf8_string = dynamic_string<types::char8>;
            
        }
    }
}
