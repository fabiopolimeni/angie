// Copyright (c) 2017 Fabio Polimeni
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "angie/core/types.hpp"
#include "angie/core/utils.hpp"
#include "angie/core/algorithm.hpp"
#include "angie/core/memory/allocator.hpp"
#include "angie/core/memory/manipulation.hpp"
#include "angie/core/buffers/array_buffer.hpp"
#include "angie/core/containers/common.hpp"
#include "angie/core/diagnostics/assert.hpp"

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
			struct dynamic_string : buffers::array_buffer<T> {
				const memory::allocator*    allocator 	= &memory::get_default_allocator();

				T& operator[](types::size idx) {
                    angie_assert(buffer);
					return buffer->data[idx];
				}

    			const T& operator[](types::size idx) const {
                    angie_assert(buffer);
					return buffer->data[idx];
				}
			};

			template<typename T>
			inline dynamic_string<T> format(const T* msg, ...) {
				return dynamic_string<T>();
			}
            
        }
    }
}

