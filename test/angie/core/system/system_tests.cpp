// Copyright (c) 2017 Fabio Polimeni
// Created on: 19/03/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "angie/core/diagnostics/assert.hpp"
#include "angie/core/system/system.hpp"
#include "angie/core/system/report.hpp"
#include "angie/core/system/cpu.hpp"

#include <cstdio>

void report_callback(angie::core::system::report::level lvl,
	const angie::core::types::char8* msg) {
	using namespace angie::core;

	switch (lvl) {
	case system::report::level::info:
		printf("INFO: %s\n", msg);
		break;

	case system::report::level::debug:
		printf("DEBUG: %s\n", msg);
		break;

	case system::report::level::performance:
		printf("PERF: %s\n", msg);
		break;

	case system::report::level::warning:
		printf("WARN: %s\n", msg);
		break;

	case system::report::level::error:
		printf("ERROR: %s\n", msg);
		exit(EXIT_FAILURE);
		break;

	case system::report::level::fatal:
		printf("FATAL: %s\n", msg);
		abort();
		break;
	}
}

TEST_CASE( "System tests", "[system]" )
{
    using namespace angie::core;
	using namespace angie::core::system;

    SECTION("System initialisation") {
        REQUIRE(system::init() == system::error::ok);
    }

	SECTION("Report") {
		report::settings sets = {
			report_callback,		// report callback
			report::level::debug,	// max report level
			false,					// exit on error
			true,					// abort on fatal
			true					// callstack on exit
		};

		REQUIRE(report::init(sets));
	}

	SECTION("CPU brand name") {
		containers::dynamic_array<cpu_info> cpus = { 
			nullptr, 0, 0
		};

		REQUIRE(system::query_cpu_info(cpus));
		report::issue(report::level::info, cpus[0].brand_name);
		system::release_cpu_info(cpus);
	}

    SECTION("System shutdown") {
        system::shutdown();
    }
}

