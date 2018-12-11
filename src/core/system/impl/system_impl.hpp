// Copyright (c) 2017 Fabio Polimeni
// Created on: 21/03/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "angie/core/system/system.hpp"

namespace angie {
    namespace core {
        namespace system {
            namespace impl {

                /**
                 * Initializes the whole system.
                 *
                 * @return error_e::ok if no error any of the error_e otherwise.
                 * @see error_e
                 * @since 0.0.1
                 */
                error init();

                /**
                 * Properly shuts down the system.
                 *
                 * @since 0.0.1
                 */
                void shutdown();

            }
        }
    }
}