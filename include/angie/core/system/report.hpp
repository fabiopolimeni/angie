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
                    fatal,
                    error,
                    warning,
                    performance,
                    info,
					debug
                };
 
                using callback = void (level lvl, const types::char8* msg);

				struct settings {
					callback*		callback;
					level			max_level;
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

				inline void debug(const types::char8* msg) {
					issue(level::debug, msg);
				}

				inline void info(const types::char8* msg) {
					issue(level::info, msg);
				}

				inline void performance(const types::char8* msg) {
					issue(level::performance, msg);
				}

				inline void warning(const types::char8* msg) {
					issue(level::warning, msg);
				}

				inline void error(const types::char8* msg) {
					issue(level::error, msg);
				}

				inline void fatal(const types::char8* msg) {
					issue(level::fatal, msg);
				}
            }
            
        }
    }
}
