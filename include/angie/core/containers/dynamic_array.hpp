// Copyright (c) 2017 Fabio Polimeni
// Created on: 09/04/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "angie/core/types.hpp"
#include "angie/core/utils.hpp"
#include "angie/core/diagnostics/assert.hpp"
#include "angie/core/algorithm.hpp"
#include "angie/core/memory/allocator.hpp"
#include "angie/core/memory/manipulation.hpp"
#include "angie/core/buffers/array_buffer.hpp"
#include "angie/core/containers/common.hpp"

namespace angie {
	namespace core {
		namespace containers {

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
			struct dynamic_array :
				buffers::array_allocator,
				buffers::array_buffer<T> {
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
				if (buffers::get_data(arr)) {
					// If data is not null, then count must be less or equal to
					// capacity, and the capacity cannot be zero. Finally memory
					// must be aligned to sizeof(T) and capacity power of two.
					return (buffers::get_capacity(arr)
						&& buffers::get_count(arr) <= buffers::get_capacity(arr)
						&& utils::is_multiple_of((types::uintptr)buffers::get_data(arr),
							buffers::get_type_alignment<T>()))
						? state::ready : state::inconsistent_properties;
				}
				else {
					// If data is null, then count and capacity must also be zero.
					return (!buffers::get_count(arr) && !buffers::get_capacity(arr))
						? state::ready : state::inconsistent_properties;
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
			 * Release memory and zero array's properties.
			 *
			 * @tparam T POD type
			 * @param arr Array to empty
			 */
			template <typename T>
			inline void release(dynamic_array<T>& arr) {
				if (buffers::get_data(arr)) {
					arr.allocator.free(buffers::get_data(arr));
					buffers::set_data(arr, static_cast<T*>(nullptr));
				}

				buffers::set_count(arr, 0);
				buffers::set_capacity(arr, 0);
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
			inline types::boolean reserve(dynamic_array<T>& arr, types::size num) {
				if (num > 0) {
					auto new_count = num + buffers::get_count(arr);
					auto new_capacity = buffers::compute_capacity(new_count);

					// Check whether the final capacity of the buffer is less
					// than, or equal, to the current one. If so, return.
					if (new_capacity <= buffers::get_capacity(arr)) {
						return true;
					}

					auto new_data = static_cast<T*>(arr.allocator.realloc(
						buffers::get_data(arr),
						buffers::compute_size<T>(new_capacity),
						buffers::get_type_alignment<T>()));

					if (new_data) {
						buffers::set_data(arr, new_data);
						buffers::set_capacity(arr, new_capacity);
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
			 * @param num Initial capacity. It will be ceil-ed to the next
			 * 	power of two value.
			 * @return true if the array has been successfully initialised,
			 * false otherwise.
			 */
			template <typename T>
			inline types::boolean init(dynamic_array<T>& arr, types::size num = 0) {
				if (!buffers::is_empty(arr)) {
					containers::release(arr);
				}

				return containers::reserve(arr, num);
			}

			template<typename T>
			inline dynamic_array<T> make_dynamic_array(types::size num = 0,
				const memory::allocator& allocator = memory::get_default_allocator()) {
				dynamic_array<T> new_array = {
					allocator, 0, 0, nullptr
				};

				init(new_array, num);
				return new_array;
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
				// than 0, and because we assume the user wants to return
				// as much memory as possible, this would not be a
				// desirable behaviour. Hence, if count == 0, we make this
				// function to behave as release().
				if (buffers::get_count(dst) == 0) {
					release(dst);
					return true;
				}

				auto new_capacity = buffers::compute_capacity(buffers::get_count(dst));
				if (new_capacity < buffers::get_capacity(dst)) {
					// We can't simply `realloc` here, because most of the
					// allocators do not truly reallocate memory if the
					// given size is less than the original one used when
					// initially instantiate the pointer. We assume that,
					// when the user calls this function, it is with the
					// intention of returning memory to the system.
					// Therefore, because of the above behaviour, if we would
					// simply `realloc`, the user would never benefit from
					// this function call, hence, we allocate a new buffer of
					// `new_capacity` size, move data from the original one
					// and finally release the old memory.
					auto* new_data = static_cast<T*>(dst.allocator.alloc(
						buffers::compute_size<T>(new_capacity),
						buffers::get_type_alignment<T>()));

					// Allocation might fail
					if (new_data == nullptr) {
						return false;
					}

					// dst.data must be not-null, because we check
					// whether the new capacity is less than the old
					// one, therefore, for dst.data to be null,
					// the capacity should be zero and we would never
					// enter this block as capacity is of an unsigned.
					angie_assert(dst.data, "dst.data can't be null");
					memory::move(new_data, buffers::get_data(dst),
						buffers::compute_size<T>(buffers::get_count(dst)));

					// Whether we have allocated new memory or not, this
					// function results in freeing the previous buffer.
					dst.allocator.free(buffers::get_data(dst));
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
				auto new_capacity = buffers::compute_capacity(new_size);

				// Check whether the final capacity of the buffer differs
				// if this is not the case, then, there is no need to realloc.
				if (new_capacity <= buffers::get_capacity(dst)) {
					dst.count = new_size;
					return true;
				}

				// If we reach this point, the new capacity can either be
				// less, or greater than the current one. In both cases
				// we issue a `realloc`.
				// @note: Memory will probably be truly reallocated only
				// when the new items wouldn't fit into the current capacity.
				auto* new_data = static_cast<T*>(dst.allocator.realloc(
					buffers::get_data(dst), buffers::compute_size<T>(new_capacity),
					buffers::get_type_alignment<T>()));

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
			 * Add `num` elements, starting at `from`.
			 *
			 * This function will make sure that allocated buffer is capable of
			 * holding `num` more elements. Elements can be added starting at
			 * any position, even greater than the current size. In such case
			 * the array will grow to `from` + `num`, and any eventual padding
			 * positions will be left uninitialized.
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

				auto new_size = buffers::get_count(dst) + num;

				// Because we want to insert uninitialized elements if starting
				// from a point which would overflow otherwise, then, adjust for
				// new size, taking into account the resulting padding.
				if (from > buffers::get_count(dst)) {
					new_size += from - buffers::get_count(dst);
				}

				auto old_count = buffers::get_count(dst);
				if (new_size && resize(dst, new_size)) {
					// In case we are inserting somewhere in the middle of the
					// array, then, we need to move the right part of the split
					// to the end of the new inserted memory chunk.
					if (from < old_count) {
						// Following logic is needed to respect the assumption
						// that we can manipulate overlapping memory buffers.
						auto count = algorithm::min(
							buffers::get_count(dst) - old_count,
							buffers::get_count(dst) - from + num);
						
						auto end = buffers::get_count(dst) - count;
						auto begin = end - count;
						while (count > 0) {
							// Move as much as memory in one call
							memory::move(buffers::get_data(dst) + end, buffers::get_data(dst) + begin,
								buffers::compute_size<T>(count));
							
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
			 * This function will not result in memory reallocation, and
			 * it will be potentially slower than `replace_with_last()`.
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
			 * @return true if successful, false otherwise
			 */
			template <typename T>
			inline types::boolean remove(dynamic_array<T>& dst,
				types::uintptr from, types::size num, types::size& removed) {
				angie_assert(from < buffers::get_count(dst));
				
				const auto end_index = get_count(dst);
				auto n_to_remove = algorithm::min(num, end_index - from);

				removed = buffers::left_shift(dst, from, num);
				dst.count -= removed;

				return removed == n_to_remove;
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
				angie_assert(from < buffers::get_count(dst));

				// Prevent memory override
				auto n_to_remove = algorithm::min(num, buffers::get_count(dst) - from);

				// Following calculations are needed in order to
				// guarantee not to move overlapping memory chunks.
				auto move_from = algorithm::max(
					buffers::get_count(dst) - n_to_remove, from + n_to_remove);
				
				// Move manipulator can't cope with zero size either
				if (auto n_to_move = buffers::get_count(dst) - move_from) {
					memory::move(buffers::get_data(dst) + from, buffers::get_data(dst) + move_from,
						buffers::compute_size<T>(n_to_move));
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
				
				if (n_bytes && make_space(dst, at, buffers::compute_count<T>(n_bytes))) {
					return !!(memory::copy(buffers::get_data(dst) + at, src, n_bytes));
				}
				
				return false;
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
				angie_assert(from < buffers::get_count(src));
				angie_assert(is_empty(dst));

				auto count = algorithm::min(buffers::get_count(src) - from, num);
				if (count && resize(dst, count)) {
					return !!(memory::copy(buffers::get_data(dst), buffers::get_data(src) + from,
						buffers::compute_size<T>(count)));
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
				angie_assert(from < buffers::get_count(src));

				auto n_to_copy = algorithm::min(buffers::get_count(src) - from, num);
				if (n_to_copy && make_space(dst, at, n_to_copy)) {
					return buffers::write(buffers::get_data(src) + from,
						buffers::compute_size<T>(n_to_copy), dst, at);
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

				return insert(dst, buffers::get_count(dst), src);
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
			inline types::boolean extract(
				dynamic_array<T>& dst, dynamic_array<U>& src,
				types::uintptr from, types::size num) {
				angie_assert(is_valid(src));
				angie_assert(is_valid(dst));

				// Copy from an array to another
				if (copy(dst, src, from, num)) {
					types::size removed = 0;
					// Remove those elements that have been copied
					return remove(src, from, num, removed);
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

				const auto count = buffers::get_count(dst);
				if (resize(dst, count + 1)) {
					
					if (!memory::copy(buffers::get_data(dst) + count, &elem, sizeof(T))) {
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

				if (const auto count = buffers::get_count(dst) > 0) {
					
					//elem = at(dst, count - 1);
					if (!memory::copy(&elem, buffers::get_data(dst) + count - 1, sizeof(T))) {
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