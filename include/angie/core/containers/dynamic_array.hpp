// Copyright (c) 2017 Fabio Polimeni
// Created on: 09/04/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "angie/core/types.hpp"
#include "angie/core/memory/allocator.hpp"

namespace angie {
    namespace core {
        namespace array {

            /**
             * Template array implementation.
             *
             * Functions that operates on this array, assume T is a POD.
             * The allocator is not part of the object type, otherwise,
             * this would limit the usage, as we should know beforehand
             * what type of allocator we would like to use, while we want
             * to leave the user to decide which one is the most appropriate.
             * @tparam T it must be a POD type.
             */
            template <typename T>
            struct dynamic {
                T*                  data;
                types::size         count;
                types::size         capacity;
                memory::allocator*  ator;
            };

            template <typename T>
            dynamic<T>* init(memory::allocator* ator = nullptr,
                             types::size num_of_elements = 0);

            template <typename T>
            types::boolean reserve(dynamic<T>* dst, types::size num);

            template <typename T>
            void destroy(dynamic<T>* arr);

            template <typename T>
            types::size clear(dynamic<T>* dst, types::uint8 value,
                              types::size slack);

            template <typename T>
            types::size set(dynamic<T>* dst, T elem,
                            types::size from, types::size num);

            template <typename T>
            types::size add(dynamic<T>* dst, T elem,
                            types::size from, types::size num);

            template <typename T>
            types::boolean rebase(dynamic<T>* dst, types::size from,
                                  types::size num);

            template <typename T>
            types::boolean push_back(dynamic<T>* dst, T elem);

            template <typename T>
            types::boolean push_front(dynamic<T>* dst, T elem);

            template <typename T>
            types::boolean pop_back(dynamic<T>* arr, T& elem);

            template <typename T>
            types::boolean pop_front(dynamic<T>* arr, T& elem);

            template <typename T>
            types::boolean append_back(dynamic<T>* dst, const dynamic<T>* src);

            template <typename T>
            types::boolean append_front(dynamic<T>* dst, const dynamic<T>* src);

        }
    }
}