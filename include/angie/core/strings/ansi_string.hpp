// Copyright (c) 2018 Fabio Polimeni
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "angie/core/strings/dynamic_string.hpp"

namespace angie {
	namespace core {
		namespace strings {
            namespace ansi {

                /**
                 * Get the length of an ansi string.
                 * ANSI strings differs from UTF8 strings by
                 * the fact that we can compute their length
                 * directly from the number of bytes, as each
                 * character has a fixed size, that is, 1 byte.
                 */
                template <typename T>
                inline types::size length(const ansi_string& str) {
                    return buffers::get_count(str);
                }

            }
        }
    }
}