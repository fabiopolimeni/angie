// Copyright (c) 2017 Fabio Polimeni
// Created on: 11/04/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#define __STDC_WANT_LIB_EXT1__ 1
#include <cstring>

#include "angie/core/memory/manipulation.hpp"
#include "angie/core/debug/assert.hpp"

namespace angie {
    namespace core {
        namespace memory {

            types::size copy(void* ANGIE_RESTRICT dst,
                             const void* ANGIE_RESTRICT src,
                             types::size bytes) {
                angie_assert((static_cast<types::byte*>(dst) + bytes <= src)
					|| (static_cast<const types::byte*>(src) + bytes <= dst),
                    "Destination range overlaps the source buffer");
				angie_assert(dst && src,
					"Destination and source arrays must be not-null");

                // if fails it returns a error_t != 0, otherwise
                // we can guarantee the whole buffer has been copied.
                return memcpy_s(dst, bytes, src, bytes) ? 0 : bytes;
            }

            types::size move(void* ANGIE_RESTRICT dst,
                             const void* ANGIE_RESTRICT src,
                             types::size bytes) {
				angie_assert((static_cast<types::byte*>(dst) + bytes <= src)
					|| (static_cast<const types::byte*>(src) + bytes <= dst),
					"Destination range overlaps the source buffer");
				angie_assert(dst && src,
					"Destination and source arrays must be not-null");

                // if fails it returns a error_t != 0, otherwise
                // we can guarantee the whole buffer has been copied.
                return memmove_s(dst, bytes, src, bytes) ? 0 : bytes;
            }

            types::size set(void* dst, types::byte value, types::size bytes) {
                angie_assert(dst, "Destination array must be not-null");
                return memset(dst, value, bytes) ? bytes : 0;
            }

            types::boolean is_equal(const void* dst, const void*  src,
                                    types::size bytes) {
				angie_assert(dst && src,
					"Destination and source arrays must be not-null");

                return memcmp(dst, src, bytes) == 0;
            }

        }
    }
}
