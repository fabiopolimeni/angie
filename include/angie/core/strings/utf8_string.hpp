// Copyright (c) 2018 Fabio Polimeni
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "angie/core/strings/dynamic_string.hpp"

namespace angie {
	namespace core {
		namespace strings {
            namespace utf8 {

                /**
                 * Get the length of a utf8 string.
                 *
                 * Because a UTF8 codepoint can require one
                 * or more bytes, we can't comput the length
                 * directly from the byte count, we will have
                 * to walk through the whole list of codepoints.
                 */
                types::size length(const dynamic_string<const types::char8>& str);

                /**
                 * Get the length of a utf8 string.
                 *
                 * Because a UTF8 codepoint can require one
                 * or more bytes, we can't comput the length
                 * directly from the byte count, we will have
                 * to walk through the whole list of codepoints.
                 */
                types::size length(const dynamic_string<types::char8>& str);

            }
        }
    }
}
