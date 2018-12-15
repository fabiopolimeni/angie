// Copyright (c) 2017 Fabio Polimeni
// Created on: 26/03/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <cstring> // memmove
#include <cstdlib> // malloc/free
#include <malloc.h>

#include "angie/core/utils.hpp"
#include "angie/core/memory/manipulation.hpp"

namespace {

#ifdef ANGIE_CC_MSVC
#pragma warning(disable:4146) // negate an unsigned integer
#endif

	inline
	void* get_allocated_ptr(void* ptr) {
		return ((void**)ptr)[-1];
	}

    inline
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

    inline
    void free_aligned(void* ptr) {
        free(get_allocated_ptr(ptr));
    }

    inline
    size_t memory_size(void* ptr) {
		void* alloc_ptr = get_allocated_ptr(ptr);
#if defined(ANGIE_CC_MSVC) || defined(ANGIE_CC_MINGW)
        return _msize(alloc_ptr);
#else
        return malloc_usable_size(alloc_ptr);
#endif
    }

	// memory_size() returns the size of the allocated block
	// not the size available to the user, as we had to switch the pointer
	// back at least one position to hold the returned address. Therefore,
	// we need to take into account this difference when calculating the
	// final available size to the user.
    inline
	size_t get_available_memory(void* ptr) {
		const size_t dsz = (uintptr_t)ptr - (uintptr_t)get_allocated_ptr(ptr);
		return memory_size(ptr) - dsz;
	}

    inline
    void* realloc_aligned(void* ptr, size_t sz, size_t al) {
        // Handle special cases
        if (!ptr) return alloc_aligned(sz, al);
        if (!sz) return free_aligned(ptr), nullptr;

        // If pointer and size are valid, then, check the current state of the
		// given pointer, and, if the new size and alignment are less or equal
        // to the original ones, then, just return the original pointer.
		const size_t osz = get_available_memory(ptr);
        const size_t oal = angie::core::utils::alignment_of((uintptr_t) ptr);
        if (sz <= osz) {
            if (oal >= al) {
                return ptr;
            }
        }

        const size_t nal = (oal < al) ? al : oal;
        void* nptr = alloc_aligned(sz, nal);

        // Memory move can't cope with null pointers, as
        // it would result into an undefined behaviour in
        // release mode, or would result into an assert
        // in debug. Therefore, we need to intercept an
        // eventual memory allocation failure, and return
        // a null pointer.
        if (nptr) {
			const size_t nsz = osz > sz ? sz : osz;
            if (!memmove(nptr, ptr, nsz)) {
                free_aligned(nptr);
            }

            free_aligned(ptr);
        }

        return nptr;
    }

}

namespace angie {
    namespace core {
        namespace memory {

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

            types::size size_of(void* ptr) {
                return memory_size(ptr);
            }

        }
    }
}
