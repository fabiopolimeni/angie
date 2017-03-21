// Copyright (c) 2017 Fabio Polimeni
// Creation date: 19/03/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "angie/core/system/system.hpp"

TEST_CASE( "System initialisation", "[init]" )
{
    // Initialise the system
    using namespace angie::core;
    REQUIRE(system::init(nullptr) == system::error_e::ok);

    // Shutdown the system
    system::shutdown();
}

