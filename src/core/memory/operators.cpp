// Copyright (c) 2017 Fabio Polimeni
// Created on: 20/03/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "angie/core/memory/global.hpp"

/**
 * All the new operators get overloaded, and we want to instruct
 * the compiler that none of those will throw and exception.
 */
void *operator new (size_t size) {
    return angie::core::memory::allocate(size);
}

void *operator new[](size_t size) {
    return angie::core::memory::allocate(size);
}

/**
 * All the delete operators get overloaded, and we want to instruct
 * the compiler that none of those will throw and exception.
 */
void operator delete (void* p) {
    angie::core::memory::deallocate(p);
}

void operator delete[](void* p) {
    angie::core::memory::deallocate(p);
}
