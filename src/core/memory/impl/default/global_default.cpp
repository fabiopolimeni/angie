// Copyright (c) 2017 Fabio Polimeni
// Created on: 26/03/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <cstdlib> // malloc/free
#include <cstring> // memmove

#include "../global_impl.hpp"
#include "angie/core/utils.hpp"

namespace {

    static inline
    void * alloc_aligned(size_t sz, size_t al) {
        // Alignment must be a power of two.
        if (al & (al - 1))
            return nullptr;
        else if (!sz)
            return nullptr;

        // We need extra bytes to store the
        // original value returned by malloc.
        if (al < sizeof(void *))
            al = sizeof(void *);

        void *const malloc_ptr = malloc(sz + al);
        if (!malloc_ptr)
            return nullptr;

        // Align to the requested value, leaving
        // room for the original malloc value.
        void *const aligned_ptr =
                (void *) (((uintptr_t) malloc_ptr + al) & -al);

        // Store the original malloc value where
        // it can be found by operator delete.
        ((void **) aligned_ptr)[-1] = malloc_ptr;

        return aligned_ptr;
    }

    static inline
    void free_aligned(void* ptr) {
        free(((void**)ptr)[-1]);
    }

    static inline
    size_t memory_size(void* ptr) {
#if defined(ANGIE_CC_MSVC) || defined(ANGIE_CC_MINGW)
        return _msize(ptr);
#else
        return malloc_usable_size(ptr);
#endif
    }

    static inline
    void* realloc_aligned(void* ptr, size_t sz, size_t al) {
        // Handle special cases
        if (!ptr) return alloc_aligned(sz, al);
        if (!sz) return free_aligned(ptr), (void *)0;

        // If pointer and size are valid, then, check
        // the current state of the given pointer, and
        // if the new size and alignment
        size_t osz = memory_size(ptr);
        if (sz <= osz) {
            size_t oal = angie::core::utils::alignment_of((uintptr_t)ptr);
            if (oal <= al) {
                return ptr;
            }
        }

        void* nptr = alloc_aligned(sz, al);
        if (!memmove(nptr, ptr, osz)) {
            free_aligned(nptr);
        }

        free_aligned(ptr);
        return nptr;
    }

}

namespace angie {
    namespace core {
        namespace memory {
            namespace impl {

                void* allocate(types::size size, types::size align) {
                    return alloc_aligned(size, align);
                }

                void deallocate(void* ptr) {
                    free_aligned(ptr);
                }

                void* reallocate(void* ptr, types::size sz, types::size al) {
                    return realloc_aligned(ptr, sz, al);
                }

                void flush() {
                }

                types::size sizeOf(void* ptr) {
                    return memory_size(ptr);
                }

            }
        }
    }
}
