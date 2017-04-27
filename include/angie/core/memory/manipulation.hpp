// Copyright (c) 2017 Fabio Polimeni
// Created on: 11/04/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "angie/core/base.hpp"

namespace angie {
    namespace core {
        namespace memory {

            /**
             * Copy n-bytes from `src` to `dst`.
             *
			 * Buffer operations are supposed not to overlap.
             *
             * @param dst Destination buffer
             * @param src Source buffer
             * @param bytes Number of bytes to copy
             * @return Number of bytes copied, zero if fails
             */
            types::size copy(void* ANGIE_RESTRICT dst,
                             const void* ANGIE_RESTRICT src,
                             types::size bytes);

            /**
             * Move n-bytes from `src` to `dst`.
             *
			 * Buffer operations are supposed not to overlap.
			 * Accessing again memory that has been previously,
			 * moved is considered an undefined operation.
             *
             * @param dst Destination buffer
             * @param src Source buffer
             * @param bytes Number of bytes to move
             * @return Number of bytes copied, zero if fails
             */
            types::size move(void* ANGIE_RESTRICT dst,
                             const void* ANGIE_RESTRICT src,
                             types::size bytes);

            /**
             * Splat `value` over the destination buffer for n-bytes.
             *
             * @param dst Destination buffer
             * @param value Value to splat over the memory
             * @param bytes Number of bytes
             * @return Number of bytes overwritten
             */
            types::size set(void* dst, types::byte value, types::size bytes);

            /**
             * Compare n-bytes from `src` to `dst`.
             *
             * @param dst Destination buffer
             * @param src Source buffer
             * @param bytes Number of buffers to compare
             * @return true if memory matches for n-bytes, false otherwise
             */
            types::boolean is_equal(const void* dst, const void* src,
				                    types::size bytes);

        }
    }
}