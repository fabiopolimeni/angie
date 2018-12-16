// Copyright (c) 2017 Fabio Polimeni
// Created on: 09/04/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "angie/core/containers/dynamic_array.hpp"

TEST_CASE("Dynamic array tests", "[dynamic_array]")
{
	using namespace angie::core;

	SECTION("Init/Release array") {
		containers::dynamic_array<types::uint32> u32_array = {
			nullptr,  0, 0
		};

		REQUIRE(containers::init(u32_array, 1));

		REQUIRE(containers::init(u32_array, 12));
		REQUIRE(u32_array.data != nullptr);
		REQUIRE(u32_array.count == 0);
		REQUIRE(containers::is_empty(u32_array));

		containers::release(u32_array);
		REQUIRE(containers::get_state(u32_array) == containers::state::ready);
	}

	SECTION("Reserve memory") {
		containers::dynamic_array<types::uint32> u32_array = {
			nullptr,  0, 0
		};

		containers::reserve(u32_array, 4);
		REQUIRE(containers::get_state(u32_array) == containers::state::ready);
		REQUIRE(containers::is_empty(u32_array));
		REQUIRE(containers::get_capacity(u32_array) > 0);

		types::size more_elements = 13;
		containers::reserve(u32_array, more_elements);
		REQUIRE(u32_array.capacity > 13);
		REQUIRE(containers::is_empty(u32_array));

		containers::release(u32_array);
	}

	SECTION("Resize array") {
		containers::dynamic_array<types::int16> i16_a = {
			nullptr,  0, 0, memory::get_default_allocator()
		};
		
		REQUIRE(containers::init(i16_a));
		REQUIRE(containers::is_valid(i16_a));
		REQUIRE(containers::is_empty(i16_a));
		REQUIRE(containers::resize(i16_a, 3));
		REQUIRE(containers::is_empty(i16_a) == false);
		REQUIRE(i16_a.count == 3);
		REQUIRE(i16_a.capacity == 4);
		REQUIRE(containers::reserve(i16_a, 2));
		REQUIRE(i16_a.capacity == 8);
		REQUIRE(i16_a.count == 3);
		REQUIRE(containers::resize(i16_a, 7));
		REQUIRE(i16_a.capacity == 8);
		REQUIRE(i16_a.count == 7);
		REQUIRE(containers::is_full(i16_a) == false);
		REQUIRE(containers::resize(i16_a, 0));
		REQUIRE(containers::is_valid(i16_a));
		REQUIRE(containers::is_empty(i16_a));

		containers::release(i16_a);
	}

	SECTION("Clear array") {
		containers::dynamic_array<types::uint32> u32_a = {
			nullptr,  0, 0, memory::get_default_allocator()
		};

		REQUIRE(containers::resize(u32_a, 5));
		REQUIRE(u32_a.count == 5);
		REQUIRE(u32_a.capacity == 8);

		containers::clear(u32_a, 3);
		REQUIRE(u32_a.count == 2);
		REQUIRE(u32_a.capacity == 8);

		containers::release(u32_a);
	}

	SECTION("Refit array") {
		containers::dynamic_array<types::uint32> u32_a = {
			nullptr,  0, 0, memory::get_default_allocator()
		};

		REQUIRE(containers::resize(u32_a, 5));
		REQUIRE(u32_a.count == 5);
		REQUIRE(u32_a.capacity == 8);

		containers::clear(u32_a, 3);
		REQUIRE(u32_a.count == 2);
		REQUIRE(u32_a.capacity == 8);

		REQUIRE(containers::fit(u32_a));
		REQUIRE(u32_a.count == 2);
		REQUIRE(u32_a.capacity == 4);

		containers::clear(u32_a, 6);
		REQUIRE(containers::is_empty(u32_a));
		REQUIRE(containers::fit(u32_a));
		REQUIRE(u32_a.capacity == 0);

		containers::release(u32_a);
	}

	SECTION("Set elements") {
		containers::dynamic_array<types::uint32> u32_a = {
			nullptr,  0, 0, memory::get_default_allocator()
		};

		REQUIRE(containers::resize(u32_a, 5));
		REQUIRE(containers::set(u32_a, 57u, 0, 7));

		for (auto i = 0u; i < u32_a.count; ++i) {
			REQUIRE(u32_a.data[0] == 57u);
		}

		REQUIRE(containers::set(u32_a, 27u, 1, 1));
		REQUIRE(u32_a.data[0] == 57u);
		REQUIRE(u32_a.data[1] == 27u);

		containers::release(u32_a);
	}

	SECTION("Add elements") {
		containers::dynamic_array<types::uint32> u32_a = {
			nullptr,  0, 0, memory::get_default_allocator()
		};

		REQUIRE(containers::init(u32_a, 8));
		REQUIRE(containers::is_empty(u32_a));

		// [0,0,0]
		REQUIRE(containers::add(u32_a, 0u, 0, 3));
		REQUIRE(u32_a.count == 3);

		for (auto i = 0u; i < u32_a.count; ++i) {
			REQUIRE(u32_a.data[0] == 0u);
		}

		// [0,0,0,2,2]
		REQUIRE(containers::add(u32_a, 2u, 3, 2));
		REQUIRE(u32_a.count == 5);
		REQUIRE(u32_a.data[3] == u32_a.data[4]);
		REQUIRE(u32_a.data[3] == 2u);

		// [0,0,0,3,3,2,2]
		REQUIRE(containers::add(u32_a, 3u, 3, 2));
		REQUIRE(u32_a.count == 7);
		REQUIRE(u32_a.data[3] == u32_a.data[4]);
		REQUIRE(u32_a.data[3] == 3u);

		// [0,0,0,3,3,2,2,?,?,?,4,4,4]
		REQUIRE(containers::add(u32_a, 4u, 10, 3));
		REQUIRE(u32_a.count == 13);
		REQUIRE(u32_a.data[10] == u32_a.data[12]);
		REQUIRE(u32_a.data[11] == u32_a.data[10]);
		REQUIRE(u32_a.data[10] == 4u);

		// [0,1,1,1,0,0,3,3,2,2,?,?,?,4,4,4]
		REQUIRE(containers::add(u32_a, 1u, 1, 3));
		REQUIRE(u32_a.count == 16);
		REQUIRE(u32_a.data[1] == u32_a.data[2]);
		REQUIRE(u32_a.data[2] == u32_a.data[3]);
		REQUIRE(u32_a.data[3] == 1u);

		containers::release(u32_a);
	}

	SECTION("Remove elements") {
		containers::dynamic_array<types::uint32> u32_a = {
			nullptr,  0, 0, memory::get_default_allocator()
		};

		REQUIRE(containers::init(u32_a, 7));
		REQUIRE(containers::resize(u32_a, 7));

		// [0,1,2,3,4,5,6]
		for (types::uint32 i = 0; i < u32_a.count; ++i) {
			u32_a.data[i] = i;
		}

		// [0,1,4,5,6]
		REQUIRE(containers::remove(u32_a, 2, 2));
		REQUIRE(u32_a.count == 5);
		REQUIRE(u32_a.capacity == 8);
		REQUIRE(u32_a.data[0] == 0u);
		REQUIRE(u32_a.data[1] == 1u);
		REQUIRE(u32_a.data[2] == 4u);
		REQUIRE(u32_a.data[3] == 5u);
		REQUIRE(u32_a.data[4] == 6u);

		// [0,1,4]
		REQUIRE(containers::remove(u32_a, 3, 3));
		REQUIRE(u32_a.count == 3);

		// [1,4]
		REQUIRE(containers::remove(u32_a, 0, 1));
		REQUIRE(u32_a.count == 2);
		REQUIRE(u32_a.capacity == 8);
		REQUIRE(u32_a.data[0] == 1u);

		containers::release(u32_a);
	}

	SECTION("Loose/Replace elements") {
		containers::dynamic_array<types::uint8> u8_a = {
			nullptr,  0, 0, memory::get_default_allocator()
		};

		REQUIRE(containers::init(u8_a, 8));
		REQUIRE(containers::resize(u8_a, 8));

		// [0,1,2,3,4,5,6,7]
		for (types::uint8 i = 0; i < u8_a.count; ++i) {
			u8_a.data[i] = i;
		}

		// [0,1,2,6,7,5]
		types::uint8 a1[] = { 0, 1, 2, 6, 7, 5 };
		REQUIRE(containers::replace_with_last(u8_a, 3, 2));
		REQUIRE(u8_a.count == 6);
		REQUIRE(memory::is_equal(a1, u8_a.data, 6));

		// [0,1,2,6]
		REQUIRE(containers::replace_with_last(u8_a, 4, 3));
		REQUIRE(u8_a.count == 4);
		REQUIRE(memory::is_equal(a1, u8_a.data, 4));

		// [0,6]
		REQUIRE(containers::replace_with_last(u8_a, 1, 2));
		REQUIRE(u8_a.count == 2);
		REQUIRE(u8_a.data[0] == 0);
		REQUIRE(u8_a.data[1] == 6);

		REQUIRE(containers::replace_with_last(u8_a, 0, 4));
		REQUIRE(containers::is_empty(u8_a));

		containers::release(u8_a);
	}

	SECTION("Write buffers") {
		containers::dynamic_array<types::char8> str_a = {
			nullptr,  0, 0, memory::get_default_allocator()
		};

		const types::char8* str_buffer = "Lore Ipsum est";

		REQUIRE(containers::resize(str_a, 14));

		REQUIRE(containers::write_buffer(str_buffer, 4, str_a));
		REQUIRE(memory::is_equal(str_a.data, str_buffer, 4));

		REQUIRE(containers::write_buffer(str_buffer, 14, str_a));
		REQUIRE(memory::is_equal(str_a.data, str_buffer, 14));
	}

	SECTION("Copy array") {
		containers::dynamic_array<types::char8> str_a = {
			nullptr,  0, 0, memory::get_default_allocator()
		};

		const containers::dynamic_array<const types::char8> phrase_a = {
			"Lore Ipsum est", 14, 0, memory::get_default_allocator()
		};

		REQUIRE(containers::copy(str_a, phrase_a));
		REQUIRE(containers::equal(str_a, phrase_a));
	}

	SECTION("Insert array") {
		containers::dynamic_array<types::char8> str_a = {
			nullptr,  0, 0, memory::get_default_allocator()
		};

		const containers::dynamic_array<const types::char8> name_a = {
			"Fabio", 5, 0, memory::get_default_allocator()
		};

		const containers::dynamic_array<const types::char8> surname_a = {
			"Polimeni", 8, 0, memory::get_default_allocator()
		};

		// [?,P,o,l,i,m,e,n,i]
		REQUIRE(containers::insert(str_a, 1, surname_a));
		REQUIRE(containers::get_count(str_a) == 9);
		
		containers::at(str_a, 0) = ' ';

		// [F,a,b,i,o, ,P,o,l,i,m,e,n,i]
		REQUIRE(containers::insert(str_a, 0, name_a));

		REQUIRE(memory::is_equal(str_a.data, "Fabio Polimeni", 14));
	}

	SECTION("Append array") {
		containers::dynamic_array<types::char8> str_a = {
			nullptr,  0, 0, memory::get_default_allocator()
		};

		const containers::dynamic_array<const types::char8> phrase_a = {
			"Lore Ipsum est", 14, 0, memory::get_default_allocator()
		};

		REQUIRE(containers::append(str_a, phrase_a));
		REQUIRE(containers::equal(str_a, phrase_a));
	}

	SECTION("Extract array") {
		containers::dynamic_array<types::char8> into_a = {
			nullptr,  0, 0, memory::get_default_allocator()
		};
		REQUIRE(containers::init(into_a));

		containers::dynamic_array<types::char8> from_a = {
			nullptr,  0, 0, memory::get_default_allocator()
		};
		REQUIRE(containers::init(from_a, 14));
		REQUIRE(containers::resize(from_a, 14));
		REQUIRE(containers::write_buffer("Lore Ipsum est", 14, from_a));

		REQUIRE(containers::extract(from_a, 5, 6, into_a));
		REQUIRE(memory::is_equal(into_a.data, "Ipsum", 5));
		REQUIRE(memory::is_equal(from_a.data, "Lore est", from_a.count));
	}

	SECTION("Push/Pop elements") {
		// const containers::dynamic_array<const types::char8> phrase_a = {
		// 	"Lore", 4, 0, memory::get_default_allocator()
		// };

		containers::dynamic_array<types::char8> str_a = {
			nullptr,  0, 0
		};

		containers::init(str_a);
		//containers::copy(str_a, phrase_a);
		
		REQUIRE(containers::from_buffer("Lore", 4, str_a));
		
		REQUIRE(containers::push(str_a, ' '));
		REQUIRE(containers::push(str_a, 'I'));
		REQUIRE(containers::push(str_a, 'p'));
		REQUIRE(containers::push(str_a, 's'));
		REQUIRE(containers::push(str_a, 'u'));
		REQUIRE(containers::push(str_a, 'm'));
		REQUIRE(memory::is_equal(str_a.data, "Lore Ipsum", str_a.count));
		REQUIRE(containers::get_count(str_a) == 10);
		REQUIRE(containers::get_capacity(str_a) == 16);
		
		auto count = containers::get_count(str_a);
		for (auto i = 0u; i < count; ++i) {
			types::char8 c = '\0';
			REQUIRE(containers::pop(str_a, c));
		}

		REQUIRE(containers::is_empty(str_a));

		types::char8 e = 'r';
		REQUIRE(containers::pop(str_a, e) == false);
		REQUIRE(e == 'r');

		containers::release(str_a);
	}
}
