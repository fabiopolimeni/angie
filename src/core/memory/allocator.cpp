// Copyright (c) 2017 Fabio Polimeni
// Created on: 10/04/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "angie/core/memory/allocator.hpp"
#include "angie/core/memory/global.hpp"

namespace angie {
    namespace core {
        namespace memory {

            const allocator g_default_alloc = {
                    memory::allocate,
                    memory::deallocate,
                    memory::reallocate,
            };

            const allocator* get_default_allocator() {
                return &g_default_alloc;
            }

        }
    }
}