// Copyright (c) 2017 Fabio Polimeni
// Created on: 27/03/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "angie/core/defines.hpp"
#include "angie/core/types.hpp"

namespace angie {
    namespace core {
        namespace utils {

            inline
            types::size alignmentOf(types::uint_ptr ptr) {
                types::size out_align = 0;

                if (ptr) {
                    bsf(out_align, ptr);
                }

                return types::size(1) << out_align;
            }

            inline constexpr
            bool isPowerOfTwo(types::size a) {
                return !(a & (a - 1));
            }

            inline constexpr
            bool isMultipleOf(types::size n, types::size d) {
                return !(n % d);
            }

        }
    }
}
