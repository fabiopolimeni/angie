// Copyright (c) 2017 Fabio Polimeni
// Created on: 10/03/2017
//
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
		containers::dynamic_array<cpu_info*> cpu_infos = { 
			nullptr, 0, 0
		 };
		 
		REQUIRE(system::query_cpu_info(cpu_infos));
	}

	SECTION("CPU current id") {
		REQUIRE(system::get_current_cpu_id() != invalid_index);
	}
}
