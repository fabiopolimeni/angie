// Copyright (c) 2017 Fabio Polimeni
// Created on: 09/04/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "angie/core/types.hpp"

namespace angie {
    namespace core {
        namespace memory {

            /**
             * Simple structure that holds a v-table of allocation functions.
             */
            struct allocator {
                using vtb_alloc = void* (types::size sz, types::size al);
                using vtb_free = void (void* ptr);
                using vtb_realloc = void* (void* ptr, types::size sz,
                                           types::size al);

                vtb_alloc alloc;
                vtb_free free;
                vtb_realloc realloc;
            };

            /**
             * Returns the system default allocator.
             *
             * This allocator has its v-table functions pointing at
             * memory::allocate(), memory::deallocate() and memory::reallocate()
             * @return System default allocator;
             */
            allocator* get_default();

        }
    }
}
