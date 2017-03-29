// Copyright (c) 2017 Fabio Polimeni
// Created on: 26/03/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <cstdio>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "angie/core/utils.hpp"
#include "angie/core/memory/global.hpp"

TEST_CASE( "Memory allocation", "[allocation]" )
{
    using namespace angie::core;
    using namespace angie::core::types;

    SECTION("Simple C-style allocation") {
        void* buffer = memory::allocate(64);
        REQUIRE(buffer != nullptr);
        memory::deallocate(buffer);
    }

    SECTION("Aligned C-style allocation") {
        void* buffer = memory::allocate(116, 32);
        REQUIRE(buffer != nullptr);
        REQUIRE(utils::isMultipleOf((size)buffer, 16));
        REQUIRE(memory::sizeOf(buffer) >= 116);
        memory::deallocate(buffer);
    }

    SECTION("Simple C++ style allocations") {
        int_8 * m = new int_8;
        REQUIRE(m != nullptr);
        delete m;
    }

    SECTION("No throw memory overflow") {
        uint_8* a = nullptr;

		// Workaround for a stupid VC cross-compiler 32bit->64bit bug.
		// VS does use the cross-compiler when launched from the editor,
        // even when compiling a solution with a 64 bit configuration.
		// http://stackoverflow.com/questions/19803162/array-size-error-x64-process

		types::size sz = types::size(1) << 43;  // sz is non-const

        REQUIRE_NOTHROW(a = new uint_8[sz]);
        REQUIRE(a == nullptr);
    }

    SECTION("Reallocation procedures") {
        char* b = (char*)memory::reallocate(nullptr, 2*1024, 32);
        REQUIRE(b != nullptr);
        REQUIRE(utils::isMultipleOf((size)b, 32));

        // Write some data to the buffer to verify
        // that it will be carried on correctly.
        auto init_sz = memory::sizeOf(b);

        static const size_t str_sz = 32;
        char init_str[str_sz] = {0};
        sprintf_s(init_str, str_sz, "Initial buffer size: %zd", init_sz);
        memcpy(b, init_str, 32);

        void* c = memory::reallocate(b, 3*1024);
        REQUIRE(c != nullptr);
        REQUIRE(utils::isMultipleOf((size)c, 32));
        REQUIRE(memory::sizeOf(c) >= 3*1024);
        REQUIRE(strcmp((const char*)c, init_str) == 0);

        void* d = memory::reallocate(c, 1024, 64);
        REQUIRE(d != nullptr);
        REQUIRE(utils::isMultipleOf((size)d, 32));
        REQUIRE(utils::isMultipleOf((size)d, 64));
        REQUIRE(strcmp((const char*)d, init_str) == 0);

        void* e = memory::reallocate(d, 0);
        REQUIRE(e == nullptr);
    }
}



