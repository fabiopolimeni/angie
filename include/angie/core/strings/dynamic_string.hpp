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
			 * ANSI string.
			 * 
			 * Each character is represented by 1 byte only.
			 * ANSI is not really a standard, as it may end
			 * up using all 256 possible characters. Although,
			 * the first 127 are standardized in the ASCII,
			 * the second half depend on the encoding. Still,
			 * each character is made of 1 single byte.
			 */
			using ansi_string = dynamic_string<types::char8>;

			/**
			 * UTF8 string.
			 * 
			 * UTF8 strings can require multiple bytes to 
			 * represent a single character (codepoint).
			 * This string representation is one of the
			 * Unicode ones, and its main difference from
			 * simpler ANSI strings is the fact that can
			 * have a variable size per represented character.
			 * Therefore, getting the length of a utf8_string
			 * can greatly differ from its simpler counterpart
			 * ANSI string, and can take considerably longer
			 * to compute. Nevertheless, keep in mind that
			 * ASCII strings are, by definition, valid UTF8
			 * strings too.
			 */
			using utf8_string = dynamic_string<types::char8>;

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
			 * of the current executing thread, copy its content into a
			 * new string object and use that instead.
			 * 
			 * @param msg String to format
			 * @return Formatted string
			 */
			const dynamic_string<types::char8>& format(const types::char8* fmt, ...);
            
        }
    }
}
