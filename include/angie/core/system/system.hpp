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
                generic_system_error,
                memory_manager_error
            };

            namespace report
            {
                enum class level
                {
                    info,
                    debug,
                    performance,
                    warning,
                    error,
                    fatal
                };
 
                using callback = void (level lvl, const types::char8* msg);
            }

            /**
             * Return the system callback passed at initialisation.
             */
            report::callback* get_report_callback();
            
            /**
             * Initializes the whole system.
             *
             * @param cb Report callback function.
             * @return error_e::ok if no error any of the error_e otherwise.
             * @see error_e
             * @see report::callback()
             * @since 0.0.1
             */
            error init(report::callback* cb);

            /**
             * Properly shuts down the system.
             *
             * @since 0.0.1
             */
            void shutdown();

        }
    }
}
