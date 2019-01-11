// Copyright (c) 2018 Fabio Polimeni
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "angie/core/types.hpp"
#include "angie/core/utils.hpp"
#include "angie/core/constants.hpp"
#include "angie/core/diagnostics/assert.hpp"
#include "angie/core/memory/allocator.hpp"
#include "angie/core/memory/manipulation.hpp"

namespace angie {
	namespace core {
		namespace buffers {

			/**
			 * Compute the capacity from the given size.
			 *
			 * We don't need to hold a variable, because we can compute it
			 * as we know that it is always the next power of two of its count,
			 * but `count` is zero. In the latter case the next power of two
			 * would be 1, although, because we assume that a zero count array
			 * means empty, then, we expect no memory allocated for the data.
			 *
			 * @tparam T POD type
			 * @param arr Array object to query
			 * @return Number of elements in the array
			 */
			inline types::size compute_capacity(types::size count) {
				return count ? utils::next_power_of_two(count) : 0;
			}

			/**
			 * Compute the number of elements from the given size in bytes.
			 *
			 * @tparam T POD type
			 * @param bytes Size in bytes
			 * @return Number of elements contained in `bytes`
			 */
			template <typename T>
			constexpr inline types::size compute_count(types::size bytes) {
				auto count = bytes / sizeof(T);
				return (bytes % sizeof(T)) ? count + 1 : count;
			}

			/**
			 * Compute the size in bytes from the given number of elements.
			 *
			 * @tparam T POD type
			 * @param num Number of elements
			 * @return The size in bytes necessary to store the elements
			 */
			template <typename T>
			constexpr inline types::size compute_size(types::size num) {
				return sizeof(T) * num;
			}

			/**
			 * Get the alignment of the given T type
			 *
			 * @tparam T POD type
			 * @return The alignment of the given type
			 */
			template <typename T>
			constexpr inline types::size get_type_alignment() {
				return utils::get_power_of_two(sizeof(T));
			}

			/**
			 * Struct to hold the allocator of an array buffer.
			 * This structure is meant to be used as an inherited
			 * struct of a dynamic buffer implementation.
			 */
			struct array_allocator {
				const memory::allocator& allocator = memory::get_default_allocator();

				// array_allocator(const array_allocator&) = delete;
				// array_allocator& operator=(const array_allocator&) = delete;
			};

			/**
			 * This data representation will allow to store data
			 * and meta-data within the same allocation unit.
			 * It will allow cache-friendly allocation patterns.
			 */
			template <typename T>
			struct array_buffer {
				types::size count		= 0;
				types::size	capacity	= 0;
				T* 			data		= nullptr;

				T& operator[](types::size idx) {
					return data[idx];
				}

    			const T& operator[](types::size idx) const {
					return data[idx];
				}
			};

            /**
			 * Get data the given data of the array.
			 *
			 * @tparam T POD type
			 * @param arr Array object to check
			 * @return Element at the given position
			 */
			template <typename T>
			inline T* get_data(const array_buffer<T>& arr) {
				return arr.data;
			}

            /**
			 * Set data pointer of the array.
			 *
			 * @tparam T POD type
			 * @param arr Array object to check
			 * @param ptr Memory address the data of this array points to
			 */
			template <typename T>
			inline void set_data(array_buffer<T>& arr, T* ptr) {
				arr.data = ptr;
			}

			/**
			 * Get the number of elements hold by this array.
			 *
			 * @tparam T POD type
			 * @param arr Array object to query
			 * @return Number of elements in the array
			 */
			template <typename T>
			inline types::size get_count(const array_buffer<T>& arr) {
				return arr.count;
			}

			/**
			 * Set the number of elements hold by this array.
			 *
			 * @tparam T POD type
			 * @param arr Array object to query
			 * @param count Number of element held by the array
			 */
			template <typename T>
			inline void set_count(array_buffer<T>& arr,
				types::size count) {
				arr.count = count;
			}

			/**
			 * Get the capacity of this array.
			 *
			 * @tparam T POD type
			 * @param arr Array object to query
			 * @return Number of max elements this array can hold
			 */
			template <typename T>
			inline types::size get_capacity(const array_buffer<T>& arr) {
				return arr.capacity;
			}

			/**
			 * Set the capacity of this array.
			 *
			 * @tparam T POD type
			 * @param arr Array object to query
			 * @param capacity Max number of elements this array can hold
			 */
			template <typename T>
			inline void set_capacity(array_buffer<T>& arr,
				types::size capacity) {
				arr.capacity = capacity;
			}

			/**
			 * Get the size in bytes used by the given
			 * array to accommodate its elements.
			 *
			 * @tparam T POD type
			 * @param arr Array object to query
			 * @return Bytes used to accommodate elements
			 */
			template <typename T>
			inline types::size get_size(const array_buffer<T>& arr) {
				return buffers::compute_size<T>(arr.count);
			}

			/**
			 * Get the amount of memory, in bytes, used by the
			 * array to accommodate the whole array capacity.
			 *
			 * @tparam T POD type
			 * @param arr Array object to query
			 * @return Bytes used to accommodate array's capacity
			 */
			template <typename T>
			inline types::size get_footprint(const array_buffer<T>& arr) {
				return buffers::compute_size<T>(arr.capacity);
			}

			/**
			 * Whether two arrays are considered equal or not.
			 *
			 * @tparam T POD type
			 * @param left Array object
			 * @param right Array object
			 * @return true if the two arrays are equals, false otherwise
			 */
			template <typename T, typename U>
			inline types::boolean equal(
				const array_buffer<T>& left,
				const array_buffer<U>& right) {
					
                static_assert(sizeof(T) == sizeof(U));
				
				// If a shallow compare fails, then compare memory content
				return (memory::is_equal(&left, &right, sizeof(left))
					|| ((get_count(left) == get_count(right) && get_count(left) > 0)
						&& memory::is_equal(get_data(left), get_data(right),
							buffers::compute_size<T>(get_count(left)))));
			}

			/**
			 * Access data at given position.
			 *
			 * @tparam T POD type
			 * @param arr Array object to check
			 * @param at Position to access the array at
			 * @return Element at the given position
			 */
			template <typename T>
			inline T& get(array_buffer<T>& arr, types::uintptr at) {
				angie_assert(at < arr.count);
				return get_data(arr)[at];
			}

			/**
			 * Whether or not the given array empty.
			 *
			 * This function does not check memory data to determine if the array
			 * is empty. It will only look at the `count` property only, therefore,
			 * the data pointer can be null.
			 * By design this function does not check whether the array passed
			 * is valid, it is responsibility of the user to do so if relevant.
			 *
			 * @tparam T POD type
			 * @param arr Array object to check
			 * @return true if the array is empty, false otherwise.
			 */
			template <typename T>
			inline types::boolean is_empty(const array_buffer<T>& arr) {
				return (get_count(arr) == 0);
			}

			/**
			 * Whether or not the given array is full.
			 *
			 * An array is considered full when `data` is not null, `capacity`
			 * and `count` are greater than zero, and `count` is equal to
			 * `capacity`. By design this function does not check whether the
			 * array is valid, it is responsibility of the user if necessary.
			 *
			 * @tparam T POD type
			 * @param arr Array object to check
			 * @return true if the array is full, false otherwise.
			 */
			template <typename T>
			inline types::boolean is_full(const array_buffer<T>& arr) {
				return (get_count(arr) > 0 && get_count(arr) == get_capacity(arr));
			}

			/**
			 * Remove last `num` elements from the array.
			 *
			 * This function does not resize the memory, so its capacity will
			 * remain the same after clearing. If you want to release memory
			 * to fit the array to the minimum required space, refer to `fit()`.
			 * Number of elements can be greater than the current total count,
			 * if this is the case, the array will be cleared entirely.
			 *
			 * @tparam T POD type
			 * @param dst Array to clear
			 * @param num Number of last elements to clear
			 * @param value Byte value splatted over buffer's `data`
			 */
			template <typename T>
			inline void clear(array_buffer<T>& dst,
				types::size num = constants::max_size, types::uint8 value = 0) {
				if (num > 0 && get_data(dst)) {
					auto n_to_clear = algorithm::clamp<types::size>(num, 1, get_count(dst));
					auto start = get_count(dst) - n_to_clear;

					memory::set(get_data(dst) + start, value,
						buffers::compute_size<T>(n_to_clear));

					buffers::set_count(dst, start);
				}
			}

			/**
			 * Overwrite `num` elements, beginning at `from`, with `elem`.
			 *
			 * This function will clamp the number of replaced elements
			 * between `num` and `dst.count`, will not resize the array.
			 * 
			 * @note: Copy constructor, or assignment, won't be called here,
			 * memory will be copied directly from the given object instead.
			 * 		   
			 * @tparam T POD type
			 * @param dst Array to operate on
			 * @param elem New element to overwrite with
			 * @param from Position where starting to overwrite from
			 * @param num Number of elements to overwrite
			 * @return The number of elements set
			 */
			template <typename T>
			inline types::size set(array_buffer<T>& dst, T elem,
				types::uintptr from, types::size num = 1) {
				angie_assert(from < get_count(dst));

				auto count = algorithm::min(from + num, get_count(dst));
				auto index = from;
				while (index < count && 
					memory::copy(get_data(dst) + index, &elem, sizeof(T))) {
					// We could use the assignment/copy contructor here.
					// Although, if the given POD object includes some
					// member declared const, the compiler will return an
					// error because a default copy constructor will be 
					// generated if not overwritten.
					// Because one of the principle of this library is to
					// remove as much as possible the common boilerplate code
					// usually necessary for C++ type definition, we simply
					// copy the memory.
					++index;
				}

				return index - from;
			}

			/**
			 * Shift towards left `num` elements starting at `from`.
			 *
			 * This function will not result in memory reallocation, and it will
			 * be potentially slower than `replace_with_last()`.
			 * Elements will be kept in the order they were before the removal.
			 * 
			 * @note: Memory `move` or `copy` cannot operate on overlapping buffers.
			 * Because this can be the case while manipulate array's memory this
			 * algorithm cannot take advantage of such manipulators.
			 * 
			 * @note: Copy constructor, or assignment, won't be called here,
			 * memory will be copied directly from the given object instead.
			 *
			 * @tparam T POD type
			 * @param dst Array to operate on
			 * @param from Position where starting the removal from
			 * @param num Number of elements to remove
			 * @return The number of successfully shifted positions
			 */
			template <typename T>
			inline types::size left_shift(array_buffer<T>& dst,
				types::uintptr from, types::size num) {
				angie_assert(from < get_count(dst));

				const auto end_index = get_count(dst);

				// Prevent memory override
				auto n_to_remove = algorithm::min(num, end_index - from);
				
				// Overwrite one element at a time
				auto remove_from = from + n_to_remove;
				while (remove_from < end_index) {

					// Copy from one elment head to the current position
					auto* data_ptr = get_data(dst) + remove_from;
					if (!memory::copy(data_ptr - n_to_remove, data_ptr, sizeof(T))) {
						return remove_from - (from + n_to_remove);
					}

					++remove_from;
				}

				return n_to_remove;
			}

			/**
			 * Replace `num` elements starting at `from` with last `num`.
			 *
			 * This function will not guarantee to keep the elements ordered
			 * w.r.t. the positions they have at the time the array is passed.
			 * This function replaces the positions of elements with the last
			 * number of elements. Because of this assumption, it may result
			 * in a faster operation than `remove()`, as it does not have to
			 * overwrite one element at a time with a reverse loop.
			 * This function does not reallocate memory in any how.
			 *
			 * @tparam T POD type
			 * @param dst Array to operate on
			 * @param from Position where starting to replace from
			 * @param num Number of elements to remove
			 * @return The number of replaced elements
			 */
			template <typename T>
			inline types::size replace_with_last(array_buffer<T>& dst,
				types::uintptr from, types::size num) {
				angie_assert(from < get_count(dst));

				const auto end_index = get_count(dst);

				// Prevent memory override
				auto n_to_remove = algorithm::min(num, end_index - from);

				// Following calculations are needed in order to
				// prevent moving overlapping memory chunks.
				auto move_from = algorithm::max(end_index - n_to_remove,
					from + n_to_remove);
				
				// Move manipulator can't cope with zero size either
				if (auto n_to_move = end_index - move_from) {
					auto* data_ptr = get_data(dst);
					memory::move(data_ptr + from, data_ptr + move_from,
						buffers::compute_size<T>(n_to_move));
				}

				return n_to_remove;
			}

			/**
			 * Overwrite array's data with the given memory starting from `at`.
			 *
			 * This function will copy memory from the buffer, into the array.
			 * It will not make space if the buffer size is greater than the
			 * current array size. This function can be, used to de-serialize
			 * an array from a raw blob of memory, if the array is capable of
			 * containing enough elements.
			 *
			 * @tparam T POD type
			 * @param src Source buffer to copy from
			 * @param n_bytes Number of bytes to write
			 * @param dst Destination buffer
			 * @param at Position where starting to write from
			 * @return The number of written bytes
			 */
			template <typename T>
			inline types::size write(const void* src, types::size n_bytes,
				array_buffer<T>& dst, types::uintptr from = constants::begin_ptr) {
				angie_assert(src, "Source buffer must be valid");
				angie_assert(from < get_count(dst));
				angie_assert(n_bytes <= buffers::compute_size<T>(get_count(dst) - from));

				return memory::copy(get_data(dst) + from, src, n_bytes);
			}

			/**
			 * Search in the src buffer for the first occurrence of the whole pattern.
			 * 
			 * The function will start at `from` walking forward, towards the tail
			 * of the buffer. If the given pattern is found, it will return the index
			 * inside `src` where the pattern has been firstly found.
			 * This function accepts a raw buffer, as it will be easier to manage
			 * and interact with third-party libraries
			 * 
			 * @param pattern Raw buffer pattern to look for
			 * @param num Number of items in the pattern raw buffer
			 * @param src The array where to look for the given pattern
			 * @param from Initial position where to start looking from
			 * @return The first index inside `src` where the whole `pattern`
			 * 		has been found, types::end_ptr if there is no match.
			 */
			template <typename T>
			inline types::uintptr find_first(const T* pattern, types::size num,
				const array_buffer<T>& src, types::uintptr from = constants::begin_ptr) {
				if (pattern == nullptr || num == 0) {
					return constants::not_found;
				}

				const types::size check_size = compute_size<T>(num);

				// If the number of items in the pattern is greater than
				// the number of elements left to seach in the source array,
				// then, this is the end of the searching algorithm.
				const auto src_count = buffers::get_count(src);
				const auto* src_data = buffers::get_data(src);
				while (from < src_count && src_count - from >= num) {
					if (memory::is_equal(pattern, src_data + from, check_size)) {
						return from;
					}

					// We can only advance one position at a time
					++from;
				}

				// If we reach this point, then, there is
				// no matching pattern in the source array.
				return constants::not_found;
			}

			/**
			 * Search in the src buffer for the last occurrence of the whole pattern.
			 * 
			 * The function will start at `from` walking backward, towards the head
			 * of the buffer. If the given pattern is found, it will return the index
			 * inside `src` where the pattern has been lastly found.
			 * This function accepts a raw buffer, as it will be easier to manage
			 * and interact with third-party libraries
			 * 
			 * @param pattern Raw buffer pattern to look for
			 * @param num Number of items in the pattern raw buffer
			 * @param src The array where to look for the given pattern
			 * @param from Initial position where to start looking from
			 * @return The last index inside `src` where the whole `pattern` 
			 * 		has been found, types::end_ptr if there is no match.
			 */
			template <typename T>
			inline types::uintptr find_last(const T* pattern, types::size num,
				const array_buffer<T>& src, types::uintptr from = constants::end_ptr) {
				if (pattern == nullptr || num == 0) {
					return constants::not_found;
				}	

				// Early exit if the source array is empty
				const auto src_count = buffers::get_count(src);
				if (src_count == 0){
					return constants::not_found;
				}

				const types::size check_size = compute_size<T>(num);

				// Clamp the starting position from the last position available.
				// @note: from = algorithm::min(from - 1, src_count) would lead
				// to overflow condition, because `from` can already be set to 
				// the maximum number allowed by its type representation.
				from = algorithm::min(from, src_count - 1) + 1;
				const auto* src_data = buffers::get_data(src);
				while (from >= num) {
					const auto start_ptr = src_data + from - num;
					if (memory::is_equal(pattern, start_ptr, check_size)) {
						return from - num;
					}

					// We can only step back one position at a time
					--from;
				}

				// If we reach this point, then, there is
				// no matching pattern in the source array.
				return constants::not_found;
			}

        }
    }
}
