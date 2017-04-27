// Copyright (c) 2017 Fabio Polimeni
// Created on: 18/03/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "angie/core/config.hpp"
#include "angie/core/types.hpp"

namespace angie {
    namespace core {
        namespace memory {

            /**
             * Allocate global memory.
             *
             * This function behaves as malloc() of standard C11,
             * but alignment is expected to be power of 2.
             * @param size Number of adjacent bytes to allocate.
             * @param alignment Returned pointer will be aligned by this value.
             * @return Pointer to the beginning of the memory allocated.
             * @note Default alignment ANGIE_DEFAULT_MEMORY_ALIGNMENT.
             * @note Thread-safe.
             */
            void* allocate(types::size size,
                           types::size alignment =
                           ANGIE_DEFAULT_MEMORY_ALIGNMENT);

            /**
             * Deallocate a global pointer.
             *
             * This function behaves as free() of standard C11.
             * @param pointer This pointer has to be allocated by allocate().
             * @note Thread-safe.
             */
            void deallocate(void* pointer);

            /**
             * Reallocate memory from the pointer provided.
             *
             * This function behaves mostly as realloc() in standard C11.
             * The difference is in that, if the alignment passed in differs
             * from the one originally used when the pointer was initially
             * allocated, then, it will be overruled by the new alignment.
             * If the function fails while re-allocating a new pointer, a null
             * pointer is returned and the original `ptr` is not altered.
             *
             * @param ptr Memory pointer of initial allocated memory.
             * I can be null, and a new one will be allocated.
             * @param sz The new size of memory we want to requested.
             * @param al Alignment to use in case different from the
             * one passed in when the ptr was initially allocated.
             * @return Pointer to the beginning of the memory allocated.
             * @note Thread-safe.
             */
            void* reallocate(void* ptr, types::size sz, types::size al = 0);

            /**
             * Flush global memory.
             *
             * Some memory managers don't return memory to the operating system
             * even when a pointer is de-allocated, whether for performance or
             * tracking reasons, therefore they need an explicit call to perform
             * such operation.
             * @note Calling this function too often can deteriorate
             * performance.
             * @note Thread-safe.
             */
            void flush();

            /**
             * Retrieve the size of the pointer at allocation time
             *
             * @param ptr Memory pointer previously allocated
             * @return The size of the allocated pointer if not null
             * @note Thread-safe.
             */
            types::size sizeOf(void *ptr);

        }
    }
}
