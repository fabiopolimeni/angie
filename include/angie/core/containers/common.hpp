// Copyright (c) 2017 Fabio Polimeni
// Created on: 09/04/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "angie/core/types.hpp"

namespace angie {
	namespace core {
		namespace containers {

			/**
			 * Current state of the array.
			 */
			enum class state : types::int8 {
				ready,                  /*!< Ready to be manipulated */
				inconsistent_properties,/*!< Properties are inconsistent */
				invalid_allocator,		/*!< A valid allocator is compulsory */
			};

			/**
			 * Pointer helpers.
			 */
			enum class index : types::uintptr {
				begin = 0,			/*!< Identifies the origin of the array */
				end = SIZE_MAX,		/*!< Identifies the end of the array */
				none = end			/*!< Identifies and invalid index */
			};

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
				return utils::get_power_of_two(sizeof(T));
			}

			/**
			 * This data representation will allow to store data
			 * and meta-data within the same allocation unit.
			 * It will allow to have a cache-friendly allocation
			 * pattern.
			 * 
			 * @note: It is an internal definition, do not user explicitly.
			 */
			template <typename T>
			struct _array_data {
				types::size count;
				types::size	capacity;
				T 			buffer[];
			};

        }
    }
}
