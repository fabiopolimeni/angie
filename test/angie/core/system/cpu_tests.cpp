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
		array::dynamic<cpu::info*> cpu_infos = { 0 };
		cpu_infos.ator = memory::get_default_allocator();

		REQUIRE(cpu::query(cpu_infos));
	}

	SECTION("CPU current id") {
		REQUIRE(cpu::get_current_id() != invalid_index);
	}
}
