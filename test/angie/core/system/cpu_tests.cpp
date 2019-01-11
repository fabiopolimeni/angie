// Copyright (c) 2017 Fabio Polimeni
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "angie/core/system/cpu.hpp"

TEST_CASE("CPU tests", "[cpu]")
{
	using namespace angie::core;
	using namespace angie::core::system;

	SECTION("CPU query info") {
		containers::dynamic_array<cpu_info> cpus;

		REQUIRE(system::query_cpu_info(cpus));
		REQUIRE(buffers::get_count(cpus) > 0);

		auto cpu_info = buffers::get(cpus, 0);
		REQUIRE(cpu_info.physical_cores >= 1);
		REQUIRE(cpu_info.logical_processors >= 1);

		system::release_cpu_info(cpus);
	}

	SECTION("CPU current id") {
		containers::dynamic_array<cpu_info> cpus;

		REQUIRE(system::query_cpu_info(cpus));
		auto core_id = system::get_current_processor();
		REQUIRE(core_id != constants::invalid_index);
		
		system::release_cpu_info(cpus);
	}
}
