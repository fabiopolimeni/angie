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

	SECTION("CPU query info") {
		containers::dynamic_array<cpu::info*> cpu_infos = { 
			nullptr, 0, 0
		 };
		 
		REQUIRE(cpu::query(cpu_infos));
	}

	SECTION("CPU current id") {
		REQUIRE(cpu::get_current_id() != invalid_index);
	}
}
