// Copyright (c) 2017 Fabio Polimeni
// Created on: 18/03/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <cstring> // memmove

#include "../global_impl.hpp"
#include "angie/core/utils.hpp"

// Because of some incoherent use of extern "C",
// and the way the compiler decides to generate
// mangled names depending on the file extension,
// we need to redefine the ltalloc functions here
// or we would need to rename ltalloc.cc into
// ltalloc.c, or include ltalloc.cc from this file
// while removing it from the source file list.
// I have opted for the former.

extern void   ltfree(void*);
extern void*  ltrealloc(void *, size_t);
extern void*  ltmemalign(size_t, size_t);
extern void   ltsqueeze(size_t pad); // return memory to system
extern size_t ltmsize(void*);

namespace angie {
    namespace core {
        namespace memory {
            namespace impl {

                void* allocate(types::size size, types::size align) {
                    // Always guarantee a minimum alignment
                    // as big as the size of a pointer
                    if (align < sizeof(types::uint_ptr))
                        align = sizeof(types::uint_ptr);

                    return ltmemalign(align, size);
                }

                void deallocate(void* ptr) {
                    ltfree(ptr);
                }

                void* reallocate(void* ptr, types::size sz, types::size al) {
                    // Handle special cases
                    if (!ptr) return ltmemalign(sz, al);
                    if (!sz) return ltfree(ptr), nullptr;

                    // If pointer and size are valid, then, check
                    // the current state of the given pointer, and
                    // if the new size and alignment
                    size_t osz = ltmsize(ptr);
                    size_t oal = utils::alignmentOf((uintptr_t) ptr);
                    if (sz <= osz) {
                        if (oal >= al) {
                            return ptr;
                        }
                    }

                    size_t nal = (oal < al) ? al : oal;
                    void* nptr = ltmemalign(sz, nal);
                    if (!memmove(nptr, ptr, osz)) {
                        ltfree(nptr);
                    }

                    ltfree(ptr);
                    return nptr;
                }

                void flush() {
                    ltsqueeze(0);
                }

                types::size sizeOf(void* ptr) {
                    return ltmsize(ptr);
                }

            }
        }
    }
}