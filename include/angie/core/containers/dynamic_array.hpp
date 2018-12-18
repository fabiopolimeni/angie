// Copyright (c) 2017 Fabio Polimeni
// Created on: 09/04/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "angie/core/types.hpp"
#include "angie/core/utils.hpp"
#include "angie/core/algorithm.hpp"
#include "angie/core/memory/allocator.hpp"
#include "angie/core/memory/manipulation.hpp"
#include "angie/core/diagnostics/assert.hpp"
#include "angie/core/containers/common.hpp"

namespace angie {
	namespace core {
		namespace containers {

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
			constexpr inline types::size compute_capacity(types::size count) {
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
				return sizeof(T);
			}

			/**
			 * Template dynamic_array array implementation.
			 *
			 * Functions operating on this structure, assume T is a POD type.
			 * It is responsibility of the user to guarantee that the given
			 * dynamic_array object passed to the hereafter functions defined,
			 * is in a `ready` state.
			 *
			 * The allocator is not part of the object type, because it would
			 * have limited its usage, while we want to leave the user
			 * to decide which one is the most appropriate at any given time.
			 * If the allocator is null, then we assume we can't manipulate the
			 * array as memory cannot be requested or release internally, but
			 * we can still use this to retrieve or copy data. It is a useful
			 * approach in case we want to initialise static/fixed arrays.
			 *
			 * @tparam T it must be a POD type.
			 */
			template <typename T>
			struct dynamic_array {
				T* ANGIE_RESTRICT           data		= nullptr;
				types::size                 count		= 0;
				types::size					capacity	= 0;
				const memory::allocator&    allocator 	=  memory::get_default_allocator();

				T& operator[](types::size idx) {
					return data[idx];
				}

    			const T& operator[](types::size idx) const {
					return data[idx];
				}
			};

			/**
			 * Check whether all properties of the structure are consistent.
			 *
			 * @tparam T Element type (POD)
			 * @param arr Array object
			 * @return true if all properties of the structure are consistent
			 * false otherwise.
			 */
			template <typename T>
			inline state get_state(const dynamic_array<T>& arr) {
				if (arr.data) {
					// If data is not null, then count must be less or equal to
					// capacity, and the capacity cannot be zero. Finally memory
					// must be aligned to sizeof(T) and capacity power of two.
					return (get_capacity(arr) && arr.count <= get_capacity(arr) &&
						utils::is_multiple_of((types::uintptr)arr.data,
							get_type_alignment<T>()))
						? state::ready
						: state::inconsistent_properties;
				}
				else {
					// If data is null, then count
					// and capacity must also be zero.
					return (!arr.count && !get_capacity(arr))
						? state::ready
						: state::inconsistent_properties;
				}
			}

			/**
			 * Whether or not the given array is in ready state.
			 *
			 * If you need to know why this array is not considered valid,
			 * query its state with `get_state()`.
			 *
			 * @tparam T POD type
			 * @param arr Array object to check
			 * @return true if it is valid, false otherwise
			 */
			template <typename T>
			inline types::boolean is_valid(const dynamic_array<T>& arr) {
				return get_state(arr) == state::ready;
			}

			/**
			 * Whether two arrays are considered equal or not.
			 *
			 * @tparam T POD type
			 * @param left Array object
			 * @param right Array object
			 * @return true if the two arrays are equals, false otherwise
			 */
			template <typename T>
			inline types::boolean equal(const dynamic_array<T>& left,
				const dynamic_array<T>& right) {
				// If a shallow compare fails, then compare memory content
				return (memory::is_equal(&left, &right, sizeof(left))
					|| ((left.count == right.count && left.count > 0)
						&& memory::is_equal(left.data, right.data,
							compute_size<T>(left.count))));
			}

			/**
			 * Whether two arrays are considered equal or not.
			 *
			 * @tparam T POD type
			 * @param left Array object
			 * @param right Array object
			 * @return true if the two arrays are equals, false otherwise
			 */
			template <typename T>
			inline types::boolean equal(const dynamic_array<const T>& left,
				const dynamic_array<T>& right) {
				return (memory::is_equal(&left, &right, sizeof(left))
					|| ((left.count == right.count && left.count > 0)
						&& memory::is_equal(left.data, right.data,
							compute_size<const T>(left.count))));
			}

			/**
			 * Whether two arrays are considered equal or not.
			 *
			 * @tparam T POD type
			 * @param left Array object
			 * @param right Array object
			 * @return true if the two arrays are equals, false otherwise
			 */
			template <typename T>
			inline types::boolean equal(const dynamic_array<T>& left,
				const dynamic_array<const T>& right) {
				return (memory::is_equal(&left, &right, sizeof(left))
					|| ((left.count == right.count && left.count > 0)
						&& memory::is_equal(left.data, right.data,
							compute_size<T>(left.count))));
			}

			/**
			 * Get data the given data of the array.
			 *
			 * @tparam T POD type
			 * @param arr Array object to check
			 * @return Element at the given position
			 */
			template <typename T>
			inline T* get_data(dynamic_array<T>& arr) {
				return arr.data;
			}

			/**
			 * Get the number of elements hold by this array.
			 *
			 * @tparam T POD type
			 * @param arr Array object to query
			 * @return Number of elements in the array
			 */
			template <typename T>
			inline types::size get_count(const dynamic_array<T>& arr) {
				return arr.count;
			}

			/**
			 * Get the capacity of this array.
			 *
			 * @tparam T POD type
			 * @param arr Array object to query
			 * @return Number of max elements this array can hold without
			 *         resizing
			 */
			template <typename T>
			inline types::size get_capacity(const dynamic_array<T>& arr) {
				return arr.capacity;
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
			inline T& at(dynamic_array<T>& arr, types::uintptr at) {
				angie_assert(at < arr.count);
				return arr.data[at];
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
			inline types::boolean is_empty(const dynamic_array<T>& arr) {
				return (arr.count == 0);
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
			inline types::boolean is_full(const dynamic_array<T>& arr) {
				return (arr.count > 0 && arr.count == get_capacity(arr));
			}

			/**
			 * Release memory and zero array's properties.
			 *
			 * @tparam T POD type
			 * @param arr Array to empty
			 */
			template <typename T>
			inline void release(dynamic_array<T>& arr) {
				angie_assert(is_valid(arr));
				if (arr.data) {
					arr.allocator.free(arr.data);
					arr.data = nullptr;
				}

				arr.count = 0;
				arr.capacity = 0;
			}

			/**
			 * Reserve space for `num` more elements.
			 *
			 * This function reserves more memory to potentially hold `num`
			 * more elements. If this procedure success, it will increase the
			 * capacity of the array, not the size.
			 *
			 * @tparam T POD type
			 * @param dst Destination array to increase the memory of
			 * @param num Least number of elements to reserve memory for
			 * @return true if the function is successful, false otherwise.
			 */
			template <typename T>
			inline types::boolean reserve(dynamic_array<T>& dst, types::size num) {
				angie_assert(is_valid(dst));

				if (num > 0) {
					auto new_count = num + dst.count;
					auto new_capacity = compute_capacity(new_count);

					auto new_data = static_cast<T*>(dst.allocator.realloc(
						dst.data, compute_size<T>(new_capacity),
						get_type_alignment<T>()));

					if (new_data) {
						dst.data = new_data;
						dst.capacity = new_capacity;
					}

					// Allocation might fail
					return (new_data != nullptr);
				}

				return true;
			}

			/**
			 * Initialise the given object array.
			 *
			 * Because we receive a reference, it may happen that
			 * the array is not empty. In such case this function
			 * will behave as re-init(). The content will be emptied
			 * before reserveing new memory to accommodate `num` elements.
			 *
			 * @tparam T POD type
			 * @param dst Object array to initialise
			 * @param num Initial capacity, it will be ceil-ed
			 * to the next power of two value.
			 * @return true if the array has been successfully initialised,
			 * false otherwise.
			 */
			template <typename T>
			inline types::boolean init(dynamic_array<T>& dst, types::size num = 0) {
				if (!is_empty(dst)) {
					release(dst);
				}

				return reserve(dst, num);
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
			 * @param value Byte value splatted over `data`;
			 * by default memory will be cleared to zeros
			 */
			template <typename T>
			inline void clear(dynamic_array<T>& dst, types::size num,
				types::uint8 value = 0) {
				angie_assert(is_valid(dst));
				if (num > 0) {
					auto n_to_clear = algorithm::clamp<types::size>(num, 1, dst.count);

					auto start = dst.count - n_to_clear;
					angie_assert(dst.data, "Non empty arrays must have valid `data`");

					memory::set(dst.data + start, value, compute_size<T>(n_to_clear));
					dst.count = start;
				}
			}

			/**
			 * Reallocate memory to best fit the number of elements.
			 *
			 * This function will reallocate memory if the new `capacity`
			 * calculated from the `count` value would result into an smaller
			 * buffer than the current one. This function must be considered
			 * an expensive process, because, in case of a reallocation,
			 * it will always perform alloc/move/release operations.
			 *
			 * @tparam T POD type
			 * @param dst Array to operate on
			 * @return true if successful, false otherwise
			 */
			template <typename T>
			inline types::boolean fit(dynamic_array<T>& dst) {
				angie_assert(is_valid(dst));

				// If dst.count = 0, then, its next power of two is 1,
				// therefore, we would always have a capacity greater
				// than 0, and because we assume the user want to return
				// as much memory as possible, this would not be a
				// desirable behaviour, hence, if count == 0, we make this
				// function to behave as release().
				if (dst.count == 0) {
					release(dst);
					return true;
				}

				auto new_capacity = compute_capacity(dst.count);
				if (new_capacity < get_capacity(dst)) {
					// We can't simply `realloc` here, because most of the
					// allocators do not truly reallocate memory if the
					// given size is less than the original one used when
					// initially instantiate the pointer. We assume that,
					// when the user calls this function, she does it with
					// the intention of returning memory to the system.
					// Therefore, because of the above behaviour, if we would
					// simply `realloc`, the user would never benefit from
					// this function call, hence, we allocate a new buffer of
					// `new_capacity` size, move data from the original one
					// and finally release the old memory.
					auto* new_data = static_cast<T*>(dst.allocator.alloc(
						compute_size<T>(new_capacity), get_type_alignment<T>()));

					// Allocation might fail
					if (new_data == nullptr) {
						return false;
					}

					// dst.data must be not-null, because we check
					// whether the new capacity is less than the old
					// one, therefore dst.data for being null,
					// the capacity should be zero and we would never
					// enter this block as capacity is of an unsigned.
					angie_assert(dst.data, "dst.data can't be null");
					memory::move(new_data, dst.data,
						compute_size<T>(dst.count));

					// Whether we have allocated new memory or not, this
					// function results in freeing the previous buffer.
					dst.allocator.free(dst.data);
					dst.data = new_data;
					dst.capacity = new_capacity;
				}

				return true;
			}

			/**
			 * Reallocate `data` memory to accommodate the new size.
			 *
			 * This function will always issue a `realloc`, although,
			 * depending on the allocator, this may, or may not actually
			 * reallocate the original buffer.
			 *
			 * @tparam T POD type
			 * @param dst Array to operate on
			 * @param new_size New number of elements in the array
			 * @return true if successful, false otherwise
			 */
			template <typename T>
			inline types::boolean resize(dynamic_array<T>& dst, types::size new_size) {
				angie_assert(is_valid(dst));

				auto new_capacity = compute_capacity(new_size);

				// Check whether the final capacity of the buffer would
				// differ, if this is not the case, then, there is no need
				// for reallocating memory.
				if (new_capacity == get_capacity(dst)) {
					dst.count = new_size;
					return true;
				}

				// If reach this point, the new capacity can either be
				// less, or greater than the current one, in both cases
				// we issue a `realloc`, although, memory will probably
				// be truly reallocated only for the letter case.
				auto* new_data = static_cast<T*>(dst.allocator.realloc(
					dst.data, compute_size<T>(new_capacity), get_type_alignment<T>()));

				// Either both `capacity` and `data` are null,
				// or both need to be valid.
				if ((new_capacity && new_data)
					|| (!new_capacity && !new_data)) {
					dst.data = new_data;
					dst.count = new_size;
					dst.capacity = new_capacity;

					return true;
				}

				return false;
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
			 * @return true if successful, false otherwise
			 */
			template <typename T>
			inline types::boolean set(dynamic_array<T>& dst, T elem,
				types::uintptr from, types::size num) {
				angie_assert(is_valid(dst));
				angie_assert(from < dst.count);

				auto count = algorithm::min(from + num, dst.count);
				while (from < count) {
					// We could use the assignment/copy contructor here.
					// Although, if the given POD object includes some const
					// member definition in it, the compiler will return an
					// error because a default copy constructor will bot be 
					// generated if not overwritten.
					// Because one of the principle of this library is to
					// remove as much as possible the common boilerplate code
					// usually necessary for C++ type definition, we simply
					// copy the memory.
					//dst.data[from++] = elem;
					if (!memory::copy(dst.data + from, &elem, sizeof(T))) {
						return false;
					}

					++from;
				}

				return true;
			}

			/**
			 * Add `num` elements, starting at `from`.
			 *
			 * This function will make sure that allocated buffer is capable of
			 * holding `num` more elements. Elements can be added starting at
			 * any position, even greater than the current size. In such case
			 * the array will grow to `from` + `num`, and any eventual padding
			 * position will be left uninitialized.
			 *
			 * @tparam T POD type
			 * @param dst Array to operate on
			 * @param from Position where starting from
			 * @param num Number of elements to add
			 * @return true if successful, false otherwise
			 */
			template <typename T>
			inline types::boolean make_space(dynamic_array<T>& dst,
				types::uintptr from, types::size num) {
				angie_assert(is_valid(dst));
				angie_assert(from < SIZE_MAX);

				auto new_size = dst.count + num;

				// Because we want to insert uninitialized elements if starting
				// from a point which would overflow otherwise, then, adjust for
				// new size, taking into account the resulting padding.
				if (from > dst.count) {
					new_size += from - dst.count;
				}

				auto old_count = dst.count;
				if (new_size && resize(dst, new_size)) {
					// In case we are inserting somewhere in the middle of the
					// array, then, we need to move the right part of the split
					// to the end of the new inserted memory chunk.
					if (from < old_count) {
						// Following logic is needed to respect the assumption
						// that we can manipulate overlapping memory buffers.
						auto count = algorithm::min(
							dst.count - old_count,
							dst.count - from + num);
						
						auto end = dst.count - count;
						auto begin = end - count;
						while (count > 0) {
							// Move as much as memory in one call
							memory::move(dst.data + end, dst.data + begin,
								compute_size<T>(count));
							
							auto diff = algorithm::min(count, begin - from);
							count = diff;

							end -= count;
							begin -= count;
						} 
					}

					return true;
				}

				// Do not consider zero size calls an error.
				return false || new_size == 0;
			}
			
			/**
			 * Add `num` `elem`s, starting at `from`.
			 *
			 * This function will add a new set of elements, and will resize the
			 * memory if necessary. You can add elements starting from an
			 * arbitrary position, even greater than the current size, in such
			 * case the array will resize to `from` + `num`. In the latter case
			 * elements from 0 to `from` will be left uninitialized.
			 *
			 * @tparam T POD type
			 * @param dst Array to operate on
			 * @param elem New element value to add
			 * @param from Position where starting from
			 * @param num Number of elements to add
			 * @return true if successful, false otherwise
			 */
			template <typename T>
			inline types::boolean add(dynamic_array<T>& dst, T elem,
				types::uintptr from, types::size num) {
				if (make_space(dst, from, num)) {
					// Overwrite the remaining positions with `elem`.
					return set(dst, elem, from, num);
				}

				return false;
			}

			/**
			 * Remove `num` elements starting at `from` maintaining the order.
			 *
			 * This function will not result in memory reallocation, and it will
			 * be potentially slower than `loose()`. Elements will be kept in
			 * the order they were before the removal. Memory `move` or `copy`
			 * cannot operate on overlapping buffers, and this can be the case
			 * for the type of operations required by the function, therefore
			 * the algorithm used cannot take advantage of such manipulators.
			 * 
			 * @note: Copy constructor, or assignment, won't be called here,
			 * memory will be copied directly from the given object instead.
			 *
			 * @tparam T POD type
			 * @param dst Array to operate on
			 * @param from Position where starting the removal from
			 * @param num Number of elements to remove
			 * @return true if successful, false otherwise
			 */
			template <typename T>
			inline types::boolean remove(dynamic_array<T>& dst, types::uintptr from,
				types::size num) {
				angie_assert(from < dst.count);

				// Prevent memory override
				auto n_to_remove = algorithm::min(num, dst.count - from);
				
				// Overwrite one element at a time
				auto remove_from = from + n_to_remove;
				while (remove_from < dst.count) {

					//dst.data[remove_from - n_to_remove] = dst.data[remove_from];
					if (!memory::copy(dst.data + remove_from - n_to_remove,
						dst.data + remove_from, sizeof(T))) {	
						dst.count -= remove_from - (from + n_to_remove);
						return false;
					}

					++remove_from;
				}

				dst.count -= n_to_remove;
				return true;
			}

			/**
			 * Replace `num` elements starting at `from` with last `num`.
			 *
			 * This function will not guarantee to keep the elements ordered
			 * w.r.t. the positions they have been added. This is due to the
			 * fact that this function replaces the positions of elements you
			 * want to remove, with the last `num`. Because of this assumption
			 * it may result in a faster operation than `remove()`, as it does
			 * not have to overwrite empty positions with a reverse loop.
			 * This function does not reallocate memory in any how.
			 *
			 * @tparam T POD type
			 * @param dst Array to operate on
			 * @param from Position where starting to replace from
			 * @param num Number of elements to remove
			 * @return true if successful, false otherwise
			 */
			template <typename T>
			inline types::boolean replace_with_last(dynamic_array<T>& dst,
				types::uintptr from, types::size num) {
				angie_assert(from < dst.count);

				// Prevent memory override
				auto n_to_remove = algorithm::min(num, dst.count - from);

				// Following calculations are needed in order to
				// guarantee not to move overlapping memory chunks.
				auto move_from = algorithm::max(
					dst.count - n_to_remove, from + n_to_remove);
				
				// Move manipulator can't cope with zero size either
				if (auto n_to_move = dst.count - move_from) {
					memory::move(dst.data + from, dst.data + move_from,
						compute_size<T>(n_to_move));
				}

				dst.count -= n_to_remove;
				return true;
			}

			/**
			 * Grow the array to accommodate the given buffer.
			 *
			 * This function will make enough space, to accommodate and copy
			 * the data contained in the buffer, into the array.
			 * This can be, used to de-serialize an array from raw memory.
			 *
			 * @tparam T POD type
			 * @param src Source buffer to copy from
			 * @param n_bytes Number of bytes to write
			 * @param dst Destination buffer
			 * @param at Position where starting to write from
			 * @return true if successful, false otherwise
			 */
			template <typename T>
			inline types::boolean from_buffer(const void* src, types::size n_bytes,
				dynamic_array<T>& dst, types::uintptr at = 0) {
				angie_assert(src, "Source buffer must be not-null");
				
				if (n_bytes && make_space(dst, at, compute_count<T>(n_bytes))) {
					return !!(memory::copy(dst.data + at, src, n_bytes));
				}
				
				return false;
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
			 * @return true if successful, false otherwise
			 */
			template <typename T>
			inline types::boolean write_buffer(const void* src,
				types::size n_bytes, dynamic_array<T>& dst,
				types::uintptr at = 0) {
				angie_assert(src, "Source buffer must be valid");
				angie_assert(at < dst.count);
				angie_assert(n_bytes <= compute_size<T>(dst.count - at));

				return !!(memory::copy(dst.data + at, src, n_bytes));
			}

			/**
			 * Create a copy of the given array.
			 *
			 * This function initialise the destination array copying the given
			 * one. Moreover, you can pick only a sub portion of the array to
			 * be copied. The destination array can have an invalid allocator,
			 * in such case, the one from the source will be used.
			 * The number of elements to copy will be clamped to the maximum
			 * available from the source array.
			 *
			 * @tparam T POD type
			 * @tparam U POD type
			 * @param dst The copy of source array
			 * @param src Source array to copy data from
			 * @param from Position where starting to copy from
			 * @param num Number of elements to copy
			 * @return true if successful, false otherwise
			 */
			template <typename T, typename U>
			inline types::boolean copy(dynamic_array<T>& dst,
				const dynamic_array<U>& src, types::uintptr from = 0,
				types::size num = SIZE_MAX) {
                static_assert(sizeof(T) == sizeof(U));
				angie_assert(from < src.count);
				angie_assert(is_empty(dst));

				auto count = algorithm::min(src.count - from, num);
				if (count && resize(dst, count)) {
					return !!(memory::copy(dst.data, src.data + from,
						compute_size<T>(count)));
				}

				// Do not consider zero size calls an error.
				return false || count == 0;
			}
			
			/**
			 * Insert elements from the source array into the destination.
			 *
			 * This function will make space in the destination array starting
			 * at `at` in order to be able to store `num` more elements, which
			 * then will be copied from the source array.
			 *
			 * @tparam T POD type
			 * @tparam U POD type
			 * @param dst Destination array to grow
			 * @param at Position where to insert elements from
			 * @param src Source array to copy elements from
			 * @param from Position where starting to copy from
			 * @param num Number of elements to copy
			 * @return true if successful, false otherwise
			 */
			template <typename T, typename U>
			inline types::boolean insert(dynamic_array<T>& dst, types::uintptr at,
				const dynamic_array<U>& src, types::uintptr from = 0,
				types::size num = SIZE_MAX) {
                static_assert(sizeof(T) == sizeof(U));
				angie_assert(is_valid(dst));
				angie_assert(from < src.count);

				auto n_to_copy = algorithm::min(src.count - from, num);
				if (n_to_copy && make_space(dst, at, n_to_copy)) {
					return write_buffer(src.data + from,
						compute_size<T>(n_to_copy), dst, at);
				}

				// Do not consider a zero size copy an error
				return false || n_to_copy == 0;
			}

			/**
			 * Append elements from the source array into the destination.
			 *
			 * This function will make space in the destination array starting
			 * at the end in order to be able to store `num` more elements, which
			 * then will be copied from the source array.
			 *
			 * @tparam T POD type
			 * @tparam U POD type
			 * @param dst Destination array to grow
			 * @param src Source array to copy elements from
			 * @param from Position where starting to copy from
			 * @param num Number of elements to copy
			 * @return true if successful, false otherwise
			 */
			template <typename T, typename U>
			inline types::boolean append(dynamic_array<T>& dst,
				const dynamic_array<U>& src,
				types::uintptr from = 0, types::size num = SIZE_MAX) {
				static_assert(sizeof(T) == sizeof(U));
				angie_assert(is_valid(dst));
				angie_assert(from < src.count);

				return insert(dst, dst.count, src);
			}

			/**
			 * Remove the range of elements from `src` and copy to `dst`.
			 *
			 * @tparam T POD type
			 * @tparam U POD type
			 * @param src Source array to extract elements from
			 * @param from Position where starting to remove from
			 * @param num Number of elements to extract
			 * @param dst Destination array to hold the extracted elements
			 * @return true if successful, false otherwise
			 */
			template <typename T, typename U>
			inline types::boolean extract(dynamic_array<U>& src,
				types::uintptr from, types::size num, dynamic_array<T>& dst) {
				angie_assert(is_valid(src));
				angie_assert(is_valid(dst));

				// Copy from an array to another
				if (copy(dst, src, from, num)) {
					// Remove those elements that have been copied
					return remove(src, from, num);
				}
				
				return false;
			}

			/**
			 * Add one element at the end of the array.
			 * 
			 * @note: Copy constructor, or assignment, won't be called here,
			 * memory will be copied directly from the given object instead.
			 *
			 * @tparam T POD type
			 * @param dst Source array to add the element to
			 * @param elem Element to add
			 * @return true if the element has been successfully added to the
			 *         array, false otherwise.
			 */
			template <typename T>
			inline types::boolean push(dynamic_array<T>& dst, T elem) {
				angie_assert(is_valid(dst));

				const auto count = get_count(dst);
				if (resize(dst, count + 1)) {
					
					if (!memory::copy(dst.data + count, &elem, sizeof(T))) {
						return false;
					}
					
					return true;
				}

				return false;
			}

			/**
			 * Remove one element from the end of the array.
			 *
			 * If the function fails, the given variable will be left untouched.
			 * 
			 * @note: Copy constructor, or assignment, won't be called here,
			 * memory will be copied directly from the given object instead.
			 *
			 * @tparam T POD type
			 * @param dst Source array to add the element to
			 * @param elem Variable that will hold the element removed
			 * @return true if the element has been successfully removed to the
			 * array, false otherwise.
			 */
			template <typename T>
			inline types::boolean pop(dynamic_array<T>& dst, T& elem) {
				angie_assert(is_valid(dst));

				if (const auto count = get_count(dst) > 0) {
					
					//elem = at(dst, count - 1);
					if (!memory::copy(&elem, dst.data + count - 1, sizeof(T))) {
						return false;
					}

					--dst.count;
					return true;
				}

				return false;
			}
		}
	}
}