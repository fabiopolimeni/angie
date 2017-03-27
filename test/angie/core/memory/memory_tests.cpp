// Copyright (c) 2017 Fabio Polimeni
// Created on: 26/03/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "angie/core/memory/global.hpp"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE( "Memory allocation", "[allocation]" )
{
    using namespace angie::core;
    using namespace angie::core::types;

    SECTION("Simple C-style allocation") {
        void* buffer = memory::allocate(64);
        REQUIRE(buffer != nullptr);
        memory::deallocate(buffer);
    }

    SECTION("Simple C++ style allocations") {
        int_8 * m = new int_8;
        REQUIRE(m != nullptr);
        delete m;
    }

    SECTION("No throw memory overflow") {
        uint_8* big_array = nullptr;
        REQUIRE_NOTHROW(big_array = new uint_8[1024*1024*1024*1024ULL]);
    }
}



