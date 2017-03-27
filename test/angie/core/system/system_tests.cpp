// Copyright (c) 2017 Fabio Polimeni
// Created on: 19/03/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "angie/core/system/system.hpp"

TEST_CASE( "System tests", "[system]" )
{
    using namespace angie::core;

    SECTION("System initialisation") {
        REQUIRE(system::init(nullptr) == system::error_e::ok);
    }

    SECTION("System shutdown") {
        system::shutdown();
    }
}

