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

            inline
            types::size get_power_of_two(types::size v) {
                return (is_power_of_two(v))
                    ? v : next_power_of_two(v);
            }

            /**
             * @fixme: This implementation is everything but secure or elegant.
             *         Change and make it part of a proper string implementation.
             */
            inline
            const types::char8* format(const char* msg, ...)
            {
                types::size const STRING_BUFFER(4096);
                static types::char8 buffer[STRING_BUFFER] = { 0 };
                
                if (msg != nullptr) {
                    va_list list;
                    va_start(list, msg);
                    vsprintf_s(buffer, msg, list);
                    va_end(list);
                }

                return buffer;
            }

        }
    }
}
