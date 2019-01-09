// Copyright (c) 2017 Fabio Polimeni
// Created on: 27/03/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "angie/core/defines.hpp"
#include "angie/core/types.hpp"

#include <cstdarg>
#include <cstdio>

namespace angie {
    namespace core {
        namespace utils {

            inline
            types::size alignment_of(types::uintptr ptr) {
                types::size out_align = 0;

                if (ptr) {
                    angie_bsf(out_align, ptr);
                }

                return types::size(1) << out_align;
            }

            inline constexpr
            bool is_multiple_of(types::size v, types::size d) {
                return (d) ? !(v % d) : false;
            }

            inline constexpr
            bool is_power_of_two(types::size a) {
                return (a) ? !(a & (a - 1)) : false;
            }

            /**
             * Little differences between prev, next,
             * floor and ceil power of 2 calculation.
             * 	    floor	prev	ceil	next
             *  0: 	1   	0   	1	    2
             *  1: 	1	    0	    1	    2
             *  2: 	2	    1	    2	    4
             *  4: 	4	    2	    4	    8
             *  5: 	4	    2	    8	    8
             *  6: 	4	    2	    8	    8
             *  7: 	4	    2	    8	    8
             *  8: 	8	    4	    8	    16
             */

            inline
            types::size next_power_of_two(types::size v) {
                types::size out_num = 0;

                if (v) {
                    angie_clz(out_num, v);
                    ++out_num;
                }

                return types::size(1) << out_num;
            }

            inline
            types::size prev_power_of_two(types::size v) {
                types::size out_num = 0;

                if (v > 1) {
                    angie_clz(out_num, v);
                    --out_num;
                } else {
                    return 0;
                }

                return types::size(1) << out_num;
            }

            inline constexpr
            types::size power_of_two_floor(types::size x) {
                types::size power = 1;
                while (x >>= 1) power <<= 1;
                return power;
            }
            
            inline constexpr
            types::size power_of_two_ceil(types::size x) {
                if (x <= 1) return 1;
                types::size power = 2;
                x--;
                while (x >>= 1) power <<= 1;
                return power;
            }

            inline constexpr
            types::size get_power_of_two(types::size v) {
                return power_of_two_ceil(v);
            }

        }
    }
}
