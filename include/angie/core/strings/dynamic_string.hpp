// Copyright (c) 2018 Fabio Polimeni
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "angie/core/containers/dynamic_array.hpp"

namespace angie {
	namespace core {
		namespace strings {

			template<typename T>
			const T nullchar = static_cast<T>('\0');

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

				dynamic_string(
					const memory::allocator& allocator = memory::get_default_allocator(),
					types::size count = 0, types::size capacity = 0, T* data = nullptr) noexcept
					: containers::dynamic_array<T>(allocator, count, capacity, data)
					{ }

				/**
				 * Construct from raw buffer.
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
					const memory::allocator& allocator = memory::get_default_allocator()) noexcept
					: containers::dynamic_array<T>(allocator, 0, 0, nullptr) {
					angie_assert(input != nullptr);

					// If count is not provided, we will need to count the
					// number of bytes and compute the count ourselves.
					if (count == 0 && input != nullptr) {
						auto ptr = input;
						while (*ptr++ != 0 && count++ < ANGIE_MAX_FORMAT_CHARS);
					}

					// Add 1 to the counter, to make enough
					// space to store the last null character.
					// To get the real number of character of
					// a string, use strings::length().
					containers::create_from_buffer(input, count + 1, *this);
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
					&& buffers::get_data(str)[count-1] == nullchar<T>);

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

			/**
			 * This function joins an array of strings into a single string.
			 *
			 * @param src Source array of strings which will be joined
			 * @param dst The final destination string formed from the join
			 * @param sep Separator string to insert between strings
			 * @return The number of new strings added to the final one
			 */
			template<typename T, typename U, typename C>
			types::size join(const containers::dynamic_array<dynamic_string<T>>& src,
				dynamic_string<U>& dst, const dynamic_string<C>& sep) {
				static_assert(sizeof(T) == sizeof(U));
				static_assert(sizeof(U) == sizeof(C));
				const auto src_count = buffers::get_count(src);
				if (src_count == 0) {
					return 0;
				}

				// The destination array, won't be cleared, therefore,
				// we need to keep an extra variable to store the number
				// of new elements added.
				const auto initial_elements = buffers::get_count(dst);
				for(types::size i = 0; i < src_count; ++i) {
					// Do not process empty strings
					if (buffers::is_empty(src[i])) {
						continue;
					}

					T last_char;

					// Remove last character, which must be a nullchar
					if (i > 0) {
						containers::pop(dst, last_char);
						angie_assert(last_char == nullchar<T>);
					}

					containers::append(dst, src[i]);

					// If this is not last source array, then,
					// add the separator to the final string.
					if (i < src_count - 1) {
						// Remove last character, which must be a nullchar
						containers::pop(dst, last_char);
						angie_assert(last_char == nullchar<T>);

						containers::append(dst, sep);
					}
				}

				// Normalize the string with the final null character
				return buffers::get_count(dst) - initial_elements;
			}

			/**
			 * This function splits a string into a new array of strings
			 * at every position in the string which matches the separator.
			 * The content of each generated sub-string will be copied from
			 * the source string until a next split is encountered, or the
			 * source string ends.
			 *
			 * @param src Source string where to look for the separator
			 * @param dst The resulted array of sub-strings generated
			 * @param sep Separator string to look for in the source string
			 * @return The number of new strings in the destination array
			 */
			template<typename T, typename U, typename C>
			types::size split(const dynamic_string<U>& src,
				containers::dynamic_array<dynamic_string<T>>& dst,
				const dynamic_string<C>& sep) {
				static_assert(sizeof(T) == sizeof(U));
				static_assert(sizeof(U) == sizeof(C));
				const auto src_count = buffers::get_count(src);
				if (src_count == 0) {
					return 0;
				}
				
				// Array where temporarely store the split indices.
				// @fixme: Use a temporary stack-like allocator.
				containers::dynamic_array<types::uintptr> indices;

				// To know where to start splitting from, we need
				// to find all matches of the given separator within
				// the source string.
				const auto n_splits = containers::find_forward(indices,
					buffers::get_data(sep), buffers::get_size(sep), src);

				// If any split is possible, then, a new string is created
				// with the content copied starting from the split index.
				if (n_splits) {
					types::uintptr split_id = constants::begin_ptr;
					const auto sep_count = buffers::get_count(sep);
					for (types::size i = 0; i < n_splits + 1; ++i) {

						const auto next_id = (i < n_splits)
							? buffers::get(indices, i)
							: buffers::get_count(src) - 1;

						// Count how many characters to copy
						const types::size split_count = algorithm::max(
							sep_count, types::size(next_id - split_id));

						// Create a split, which will be copied
						// into the destination array later on.
						dynamic_string<T> split = {
							containers::get_allocator(dst),
							0, 0, nullptr
						};

						// Allocate memory for the token,
						// and copy the data from the split.
						if (containers::create_from_array(
							buffers::get_data(src) + split_id,
							split_count, split) == split_count) {

							// Add the final null character if not already
							if (buffers::get(split, split_count - 1) != 0) {
								containers::push(split, nullchar<T>);
							}

							// Push the new token into the destination array
							containers::push(dst, split);
						}

						split_id = next_id;
					}
				}
				else {
					// If there is no matchings with the separator provided,
					// copy the whole source string into the final array.
					if (containers::resize(dst, 1)) {
						containers::copy(buffers::get(dst, 0), src);
					}
				}

				containers::release(indices);
				return buffers::get_count(dst);
			}
        }
    }
}
