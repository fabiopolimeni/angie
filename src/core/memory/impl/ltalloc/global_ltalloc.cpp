// Copyright (c) 2017 Fabio Polimeni
// Creation date: 18/03/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Because of some incoherent use of extern "C",
// and the way the compiler decides to generate
// mangled names depending on the file extension,
// we need to redefine the ltalloc functions here
// or we would need to rename ltalloc.cc into
// ltalloc.c, or include ltalloc.cc from this file
// while removing from the source file list. I have
// opted for the former.
#include <stdlib.h>

void*  ltmalloc(size_t);
void   ltfree(void*);
void*  ltrealloc(void *, size_t);
void*  ltcalloc(size_t, size_t);
void*  ltmemalign(size_t, size_t);
void   ltsqueeze(size_t pad); /*return memory to system (see README.md)*/
size_t ltmsize(void*);

#include "../global_impl.hpp"

namespace angie {
    namespace core {
        namespace memory {
            namespace impl {

                void* allocate(types::size size, types::size align) {
                    return ltmemalign(size, align);
                }

                void deallocate(void* ptr) {
                    ltfree(ptr);
                }

                void* reallocate(void* ptr,
                                 types::size size,
                                 types::size align) {
                    return --align, ltrealloc(ptr, ((size+align)&~align));
                }

                void flush() {
                    ltsqueeze(0);
                }

            }
        }
    }
}