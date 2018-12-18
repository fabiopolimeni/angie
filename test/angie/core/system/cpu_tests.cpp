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
		containers::dynamic_array<cpu_info> cpu_infos = { 
			nullptr, 0, 0
		 };
		 
		REQUIRE(system::query_cpu_info(cpu_infos));
		REQUIRE(containers::get_count(cpu_infos) > 0);

		auto cpu_info = containers::at(cpu_infos, 0);
		REQUIRE(cpu_info.physical_cores >= 1);
		REQUIRE(cpu_info.logical_processors >= 1);
	}

	SECTION("CPU current id") {
		auto core_id = invalid_index;

		REQUIRE(system::get_current_core(core_id));
		REQUIRE(core_id != invalid_index);
	}
}
