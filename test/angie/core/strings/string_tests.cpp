// Copyright (c) 2017 Fabio Polimeni
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "angie/core/system/system.hpp"
#include "angie/core/strings/dynamic_string.hpp"
#include "angie/core/strings/ansi_string.hpp"
#include "angie/core/strings/utf8_string.hpp"

#include <string.h>

TEST_CASE("String tests", "[dynamic strings]")
{
	using namespace angie::core;
	using namespace angie::core::containers;
	using namespace angie::core::strings;

	system::init();

	SECTION("String creation") {
		ansi_string str_a = "string with ANSI";
		REQUIRE(buffers::get_count(str_a) - 1 == strlen(strings::cstr(str_a)));
	}

	SECTION("String length") {
		ansi_string ansi_str = "sbasr3434_!";
		REQUIRE(ansi::length(ansi_str) == 11);
		REQUIRE(utf8::length(ansi_str) == 11);

		utf8_string utf8_str = u8"zß水🍌";
		REQUIRE(ansi::length(utf8_str) == 10);
		REQUIRE(utf8::length(utf8_str) == 4);
	}

	SECTION("Formatted strings") {
        int16_t number = 6993;
		auto fmt_string = format("number=%d", number);

		REQUIRE(memory::is_equal(fmt_string, "number=6993",
			ansi::length(fmt_string)));
	}

	SECTION("Join strings") {
        const dynamic_array<utf8_string> text = {
			{u8"This is a single phrase"},	// 23
			{u8"separated by a comma"}		// 20
		};

		utf8_string comma = u8", ";
		utf8_string sentence;
		REQUIRE(strings::join(text, sentence, comma) == 46);
		REQUIRE(memory::is_equal(sentence,
			u8"This is a single phrase, separated by a comma",
			utf8::length(sentence)));

		// dynamic_array<utf8_string> tokens;
		// REQUIRE(strings::split(sentence, tokens, comma) == 2);
		// REQUIRE(buffers::equal(buffers::get(text, 0), buffers::get(tokens, 0)));
		// REQUIRE(buffers::equal(buffers::get(text, 1), buffers::get(tokens, 1)));
	}

	system::shutdown();
}
