// Copyright (c) 2017 Fabio Polimeni
// Creation date: 19/03/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include <cstdint>
#include <cstddef>

namespace angie {
    namespace core {
        namespace types {

            using int_8 = int8_t;
            using uint_8 = uint8_t;

            using int_16 = int16_t;
            using uint_16 = uint16_t;

            using int_32 = int32_t;
            using uint_32 = uint32_t;

            using char_8 = char;
            using char_16 = char16_t;
            using char_32 = char32_t;

            using int_ptr = intptr_t;
            using uint_ptr = uintptr_t;

            using size = size_t;

        }
    }
}