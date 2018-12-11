// Copyright (c) 2017 Fabio Polimeni
// Created on: 20/03/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "angie/core/diagnostics/assert.hpp"
#include "angie/core/diagnostics/callstack.hpp"
#include "angie/core/memory/global.hpp"
#include "angie/core/memory/manipulation.hpp"
#include "angie/core/system/system.hpp"
#include "impl/system_impl.hpp"

#include <cstdio> // snprintf
#include <cstdlib> // exit, atexit, abort

#ifdef ANGIE_DEBUG_TOOLS
assert_action assert_callback(const char* cond, const char* msg,
	const char* file, unsigned int line, void* user_data) {
	
	using namespace angie::core;
	using namespace angie::core::types;
	using namespace angie::core::system;

	auto* system_cb = static_cast<report::callback*>(user_data);

	if (system_cb) {
		static const auto max_chars = 256;
		char8 assert_string[max_chars] = { 0 };

		// !! ASSERT !!: (${cond}) ${msg} [${file}:${line}]
		snprintf(assert_string, max_chars,
			"!! ASSERT !!: (%s) %s [%s:%d]\n",	cond, msg, file, line);

		system_cb(report::level::fatal, assert_string);
	}

	// You can use assert_action::ASSERT_ACTION_NONE
	// if you are sure that a specific assert is harmless.
	return assert_action::ASSERT_ACTION_BREAK;
}
#endif

extern "C" void atexit_callback() {
	using namespace angie::core::system;
	using namespace angie::core;
	auto report_settings = report::get_settings();

#ifdef ANGIE_DEBUG_TOOLS
	if (report_settings.callstack_at_exit) {
		const static auto max_addresses = 16u;
		void* addresses[max_addresses] = { nullptr };

		// Grab function addresses
		if (auto n_frames = diagnostics::grab_callstack(
			1, addresses, max_addresses)) {
			const static auto names_buffer_size = max_addresses * 512u;
			types::char8 names_buffer[names_buffer_size] = { 0 };
			diagnostics::symbol symbols[max_addresses] = { 0 };

			// We need to store the whole symbol structure in strings
			const static auto string_buffer_size = max_addresses * 1024u;
			types::char8 string_buffer[string_buffer_size] = { 0 };
			types::char8* string_ptr = string_buffer;
			types::char8* string_end = string_ptr + string_buffer_size;

			// Resolve addresses to symbols
			if (auto n_symbols = diagnostics::resolve_symbols(addresses, symbols,
				n_frames, names_buffer, names_buffer_size)) {

				// Format symbols
				for (auto sIt = 0u; sIt < n_symbols; ++sIt) {
					const auto& symbol = symbols[sIt];
					auto written_chars = snprintf(string_ptr,
						string_end - string_ptr,
						"%0zX + %0X: %s\n[%s:%d]\n",
						(types::uintptr)addresses[sIt], symbol.offset,
						symbol.function, symbol.file, symbol.line);

					// No more space in the buffer, break
					if (written_chars <= 0) {
						break;
					}

					// Advance the cursor on the string buffer
					// We add 1 because the nul character is not
					// taken into account by the snprintf result.
					if (string_ptr + written_chars + 1 < string_end) {
						string_ptr += written_chars;
					}
				}
			}
			// Symbols translation can fail, but we
			// can still report frame's addresses.
			else {
				for (auto fIt = 0u; fIt < n_frames; ++fIt) {
					auto written_chars = snprintf(string_ptr,
						string_end - string_ptr,
						"%0zX\n", (types::uintptr)addresses[fIt]);

					// No more space in the buffer, break
					if (written_chars <= 0) {
						break;
					}

					// Advance the cursor on the string buffer
					// We add 1 because the nul character is not
					// taken into account by the snprintf result.
					if (string_ptr + written_chars + 1 < string_end) {
						string_ptr += written_chars;
					}
				}
			}

			// Issue a report to give the opportunity to print out the callstack
			report::issue(report::level::debug, string_buffer);
		}
	}
#endif
}

namespace angie {
    namespace core {
        namespace system {

            error init() {
				return impl::init();
            }

            void shutdown() {
                impl::shutdown();
            }

			report::settings g_settings;

			types::boolean report::init(const report::settings& sets) {
				g_settings = sets;

#ifdef ANGIE_DEBUG_TOOLS
				if (g_settings.call_back) {
					// Register the assert callback
					assert_register_callback(assert_callback,
						g_settings.call_back);
				}
#endif

				atexit(atexit_callback);
				return true;
			}

			report::callback* report::get_callback() {
				return g_settings.call_back;
			}

			report::settings& report::get_settings() {
				return g_settings;
			}

			void report::issue(report::level lvl, const types::char8* msg) {
				if (g_settings.min_level < lvl || !g_settings.call_back) {
					return;
				}

				g_settings.call_back(lvl, msg);
				
				if (lvl == report::level::error
					&& g_settings.exit_on_error) {
					exit(EXIT_FAILURE);
				}
				else if (lvl == report::level::fatal
					&& g_settings.abort_on_fatal) {
					abort();
				}
			}
        }
    }
}
