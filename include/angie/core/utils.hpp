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
            size_t alignment_of(uintptr_t ptr) {
                size_t out_align = 0;

                if (ptr) {
                    bsf(out_align, ptr);
                }

                return 1 << out_align;
            }

        }
    }
}
