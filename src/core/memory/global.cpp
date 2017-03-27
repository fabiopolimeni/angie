// Copyright (c) 2017 Fabio Polimeni
// Created on: 20/03/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "angie/core/memory/global.hpp"
#include "impl/global_impl.hpp"

namespace angie {
    namespace core {
        namespace memory {

            void* allocate(types::size size, types::size align) {
                return impl::allocate(size, align);
            }

            void deallocate(void* ptr) {
                impl::deallocate(ptr);
            }

            void* reallocate(void* ptr, types::size size, types::size align) {
                return impl::reallocate(ptr, size, align);
            }

            void flush() {
                impl::flush();
            }

            types::size sizeOf(void* ptr) {
                return impl::sizeOf(ptr);
            }
        }
    }
}

/**
 * All the new operators get overloaded, and we want to instruct
 * the compiler that none of those will throw and exception.
 */
void *operator new  (size_t size) {
    return angie::core::memory::allocate(size);
}

void *operator new[](size_t size) {
    return angie::core::memory::allocate(size);
}

/**
 * All the delete operators get overloaded, and we want to instruct
 * the compiler that none of those will throw and exception.
 */
void operator delete  (void* p) {
    angie::core::memory::deallocate(p);
}

void operator delete[](void* p) {
    angie::core::memory::deallocate(p);
}
