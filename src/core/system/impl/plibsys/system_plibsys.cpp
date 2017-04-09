// Copyright (c) 2017 Fabio Polimeni
// Created on: 18/03/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "angie/core/system/system.hpp"
#include "angie/core/memory/global.hpp"
#include "../system_impl.hpp"
#include "plibsys.h"

extern "C" ppointer pmem_alloc (psize nbytes) {
    return (ppointer) angie::core::memory::allocate(nbytes);
}

extern "C" ppointer pmem_realloc (ppointer block, psize nbytes) {
    return (ppointer) angie::core::memory::reallocate(block, nbytes, 0);
}

extern "C" void pmem_free (ppointer block) {
    angie::core::memory::deallocate(block);
}

namespace angie {
    namespace core {
        namespace system {
            namespace impl {

                error init(report::callback *cb) {

                    PMemVTable vtable;
                    vtable.free    = pmem_free;
                    vtable.malloc  = pmem_alloc;
                    vtable.realloc = pmem_realloc;

                    p_libsys_init_full(&vtable);

                    return error::ok;
                }

                void shutdown() {
                    p_libsys_shutdown();
                }

            }
        }
    }
}