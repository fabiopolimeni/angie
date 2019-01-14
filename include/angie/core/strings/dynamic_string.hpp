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
			 * dynamic_string object passed to the hereafter functions is in
			 * `ready` state.
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

				dynamic_string(const memory::allocator& allocator,
					types::size count, types::size capacity, T* data)
					: containers::dynamic_array<T>{
						{allocator}, {count, capacity, data}} { }

				/**
				 * Default constructor from raw buffer.
				 *
				 * @eg: utf8_string str = u8"zß水🍌"
				 * @note: `count` cannot be greater than ANGIE_MAX_FORMAT_CHARS
				 * 
				 * @param input Source raw buffer pointer
				 * @param count Number of elements in the string. Bare in mind for UTF8/16
				 * 		strings this can be bigger than the actual number of characters.
				 * @param allocator The allocator to allocate memory for the string
				 */
				dynamic_string(const T* input, types::size count = 0,
					const memory::allocator& allocator = memory::get_default_allocator()) 
					: containers::dynamic_array<T>{{allocator}, {0, 0, nullptr}} {
					angie_assert(input != nullptr);

					// If count is not provided, we will need to count the
					// number of bytes and compute the count ourselves.
					if (count == 0 && input != nullptr) {
						auto ptr = input;
						while (*ptr++ != 0 && count++ < ANGIE_MAX_FORMAT_CHARS);
					}

					// Add 1 to the count, to make sure we have
					// enough space to store last null character.
					containers::from_buffer(input, count + 1, *this);
					
					// @fixme: Make sure last character is null.
					buffers::set_count(*this, count);
				}
				
				/**
				 * This operator allows to use the string class
				 * with lvalues of const T* type, e.g. const char*.
				 */
				operator T*() const { return cstr(*this); }

			};

			/**
			 * Get the C string representation.
			 */
			template <typename T>
			inline T* cstr(const dynamic_string<T>& str) {
				const auto count = buffers::get_count(str);
				
				// Unfortunately, because of the inner representation
				// of the string class, which simply inherits from a
				// dynamic_array, we need to check whether the data
				// buffer is big enough to hold last '\0' character.
				angie_assert(buffers::get_capacity(str) > count
					&& buffers::get_data(str)[count] == '\0');

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
			const dynamic_string<const types::char8>& format(const types::char8* fmt, ...);
            
        }
    }
}
