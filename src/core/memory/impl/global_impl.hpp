// Copyright (c) 2017 Fabio Polimeni
// Creation date: 20/03/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "angie/core/base.hpp"

namespace angie {
    namespace core {
        namespace memory {
            namespace impl {

                /**
                 * Allocate global memory.
                 *
                 * This function behaves as malloc() of standard C11,
                 * but alignment is expected to be power of 2.
                 * @param size Number of adjacent bytes to allocate.
                 * @param align Returned pointer will be aligned by this value.
                 * @return Pointer to the beginning of the memory allocated.
                 * @note Default alignment ANGIE_DEFAULT_MEMORY_ALIGNMENT.
                 * @note Thread-safe.
                 */
                void* allocate(types::size size, types::size align);

                /**
                 * Deallocate a global pointer.
                 *
                 * This function behaves as free() of standard C11.
                 * @param ptr This pointer has to be allocated by allocate().
                 * @note Thread-safe.
                 */
                void deallocate(void* ptr);

                /**
                 * Reallocate memory from the pointer provided.
                 *
                 * This function behaves as realloc() in standard C11.
                 * The alignment is respected on the new pointer returned even
                 * when it doesn't match the one (if) provided when the memory
                 * was initially allocated.
                 * @param ptr Memory pointer of initial allocated memory.
                 * I can be null, and a new one will be allocated.
                 * @param size The new size of memory we want to requested.
                 * @param align Returned pointer will be aligned by this value
                 * @return Pointer to the beginning of the memory allocated.
                 * @note Default alignment ANGIE_DEFAULT_MEMORY_ALIGNMENT.
                 * @note Thread-safe.
                 */
                void* reallocate(void* ptr, types::size size,
                                 types::size align);

                /**
                 * Flush global memory.
                 *
                 * Some memory managers don't return memory to the operating
                 * system even when a pointer is de-allocated, whether for
                 * performance or tracking reasons, therefore they need an
                 * explicit call to perform such operation.
                 * @note Calling this function too often can deteriorate
                 * performance.
                 * @note Thread-safe.
                 */
                void flush();

            }
        }
    }
}
