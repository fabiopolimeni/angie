// Copyright (c) 2017 Fabio Polimeni
// Created on: 18/03/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "angie/core/base.hpp"

namespace angie {
    namespace core {
        namespace system {

            enum class error
            {
                ok,
                unknown_system_error,
                memory_manager_error
            };
            
            /**
             * Initialize the whole system.
             *
             * @return error::ok if no error occurred, any error otherwise.
             * @see error
             * @since 0.0.1
             */
            error init();

            /**
             * Properly shuts the system down.
             *
             * @since 0.0.1
             */
            void shutdown();

        }
    }
}
