// Copyright (c) 2017 Fabio Polimeni
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "angie/core/system/system.hpp"
#include "angie/core/strings/dynamic_string.hpp"
#include "angie/core/strings/ansi_string.hpp"
#include "angie/core/strings/utf8_string.hpp"

TEST_CASE("String tests", "[dynamic strings]")
{
	using namespace angie::core;
	using namespace angie::core::strings;

	system::init();

	SECTION("Formatted strings") {
        int16_t number = 6993;
		ansi_string fmt_string = format("number=%d", number);

		REQUIRE(memory::is_equal(
			fmt_string, "number=6993",
			buffers::get_count(fmt_string)));
	}

	SECTION("String length") {
		auto ansi_str = format("sbasr3434_!");
		REQUIRE(ansi::length(ansi_str) == 11);
		REQUIRE(utf8::length(ansi_str) == 11);

		auto utf8_str = format(u8"zß水🍌");
		REQUIRE(ansi::length(utf8_str) == 10);
		REQUIRE(utf8::length(utf8_str) == 4);
	}

	system::shutdown();
}
