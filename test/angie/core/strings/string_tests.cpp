// Copyright (c) 2017 Fabio Polimeni
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "angie/core/strings/dynamic_string.hpp"

TEST_CASE("Dynamic array tests", "[dynamic_array]")
{
	using namespace angie::core;
    using namespace angie::core::strings;

	SECTION("Init strings") {
        int16_t number = 6993;
		utf8_string fmt_string = format("number=%d", number);

		REQUIRE(true);
	}
}
