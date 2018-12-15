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

            namespace report
            {
                enum class level : types::uint8
				{
					debug,
                    info,
                    performance,
                    warning,
                    error,
                    fatal
                };
 
                using callback = void (level lvl, const types::char8* msg);

				struct settings {
					callback*		callback;
					level			min_level;
					types::boolean	exit_on_error;
					types::boolean	abort_on_fatal;
					types::boolean	callstack_at_exit;
				};

				/*
				 * Initialise the report manager.
				 * 
				 * @param sets report settings
				 * @return true if successful, false otherwise.
				 */
				types::boolean init(const settings& sets);

				/**
				 * Return the system report callback.
				 */
				report::callback* get_callback();

				/*
				 * Access report settings.
				 */
				report::settings& get_settings();

				/*
				 * Issue a report.
				 */
				void issue(level lvl, const types::char8* msg);
            }
            
        }
    }
}
