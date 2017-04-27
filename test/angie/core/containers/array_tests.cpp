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

	SECTION("Create array on the heap") {
		auto* u16_array_ptr = array::make<types::uint16>(6);
		REQUIRE(u16_array_ptr);
		REQUIRE(u16_array_ptr->data);
		REQUIRE(u16_array_ptr->count == 0);
		REQUIRE(u16_array_ptr->capacity >= 6);

		array::destroy(u16_array_ptr);
	}

	SECTION("Destory allocated array") {
		auto* u16_array_ptr = array::make<types::uint16>(6);
		array::destroy(u16_array_ptr);
		REQUIRE(u16_array_ptr == nullptr);
	}

	SECTION("Init/Release array") {
		array::dynamic<types::uint32> u32_array = { 0 };

		u32_array.ator = memory::get_default_allocator();
		REQUIRE(array::init(u32_array, 1));

		u32_array.ator = memory::get_default_allocator();
		REQUIRE(array::init(u32_array, 12));
		REQUIRE(u32_array.data != nullptr);
		REQUIRE(u32_array.count == 0);
		REQUIRE(array::is_empty(u32_array));

		array::release(u32_array);
		REQUIRE(array::get_state(u32_array) == array::state::ready);
	}

	SECTION("Reserve memory") {
		array::dynamic<types::uint32> u32_array = { 0 };
		u32_array.ator = memory::get_default_allocator();

		array::reserve(u32_array, 4);
		REQUIRE(array::get_state(u32_array) == array::state::ready);
		REQUIRE(array::is_empty(u32_array));
		REQUIRE(u32_array.capacity > 0);

		types::size more_elements = 13;
		array::reserve(u32_array, more_elements);
		REQUIRE(u32_array.capacity > 13);
		REQUIRE(array::is_empty(u32_array));

		array::release(u32_array);
	}

	SECTION("Resize array") {
		auto* i16_a = array::make<types::int16>();
		REQUIRE(array::is_valid(*i16_a));
		REQUIRE(array::is_empty(*i16_a));
		REQUIRE(array::resize(*i16_a, 3));
		REQUIRE(array::is_empty(*i16_a) == false);
		REQUIRE(i16_a->count == 3);
		REQUIRE(i16_a->capacity == 4);
		REQUIRE(array::reserve(*i16_a, 2));
		REQUIRE(i16_a->capacity == 8);
		REQUIRE(i16_a->count == 3);
		REQUIRE(array::resize(*i16_a, 7));
		REQUIRE(i16_a->capacity == 8);
		REQUIRE(i16_a->count == 7);
		REQUIRE(array::is_full(*i16_a) == false);
		REQUIRE(array::resize(*i16_a, 0));
		REQUIRE(array::is_valid(*i16_a));
		REQUIRE(array::is_empty(*i16_a));

		array::destroy(i16_a);
	}

	SECTION("Clear array") {
		array::dynamic<types::uint32> u32_a = { 0 };
		u32_a.ator = memory::get_default_allocator();

		REQUIRE(array::resize(u32_a, 5));
		REQUIRE(u32_a.count == 5);
		REQUIRE(u32_a.capacity == 8);

		array::clear(u32_a, 3);
		REQUIRE(u32_a.count == 2);
		REQUIRE(u32_a.capacity == 8);

		array::release(u32_a);
	}

	SECTION("Refit array") {
		array::dynamic<types::uint32> u32_a = { 0 };
		u32_a.ator = memory::get_default_allocator();

		REQUIRE(array::resize(u32_a, 5));
		REQUIRE(u32_a.count == 5);
		REQUIRE(u32_a.capacity == 8);

		array::clear(u32_a, 3);
		REQUIRE(u32_a.count == 2);
		REQUIRE(u32_a.capacity == 8);

		REQUIRE(array::fit(u32_a));
		REQUIRE(u32_a.count == 2);
		REQUIRE(u32_a.capacity == 4);

		array::clear(u32_a, 6);
		REQUIRE(array::is_empty(u32_a));
		REQUIRE(array::fit(u32_a));
		REQUIRE(u32_a.capacity == 0);

		array::release(u32_a);
	}

	SECTION("Set elements") {
		array::dynamic<types::uint32> u32_a = { 0 };
		u32_a.ator = memory::get_default_allocator();

		REQUIRE(array::resize(u32_a, 5));
		REQUIRE(array::set(u32_a, 57u, 0, 7));

		for (auto i = 0u; i < u32_a.count; ++i) {
			REQUIRE(u32_a.data[0] == 57u);
		}

		REQUIRE(array::set(u32_a, 27u, 1, 1));
		REQUIRE(u32_a.data[0] == 57u);
		REQUIRE(u32_a.data[1] == 27u);

		array::release(u32_a);
	}

	SECTION("Add elements") {
		array::dynamic<types::uint32> u32_a = { 0 };
		u32_a.ator = memory::get_default_allocator();

		REQUIRE(array::init(u32_a, 8));
		REQUIRE(array::is_empty(u32_a));

		// [0,0,0]
		REQUIRE(array::add(u32_a, 0u, 0, 3));
		REQUIRE(u32_a.count == 3);

		for (auto i = 0u; i < u32_a.count; ++i) {
			REQUIRE(u32_a.data[0] == 0u);
		}

		// [0,0,0,2,2]
		REQUIRE(array::add(u32_a, 2u, 3, 2));
		REQUIRE(u32_a.count == 5);
		REQUIRE(u32_a.data[3] == u32_a.data[4]);
		REQUIRE(u32_a.data[3] == 2u);

		// [0,0,0,3,3,2,2]
		REQUIRE(array::add(u32_a, 3u, 3, 2));
		REQUIRE(u32_a.count == 7);
		REQUIRE(u32_a.data[3] == u32_a.data[4]);
		REQUIRE(u32_a.data[3] == 3u);

		// [0,0,0,3,3,2,2,?,?,?,4,4,4]
		REQUIRE(array::add(u32_a, 4u, 10, 3));
		REQUIRE(u32_a.count == 13);
		REQUIRE(u32_a.data[10] == u32_a.data[12]);
		REQUIRE(u32_a.data[11] == u32_a.data[10]);
		REQUIRE(u32_a.data[10] == 4u);

		// [0,1,1,1,0,0,3,3,2,2,?,?,?,4,4,4]
		REQUIRE(array::add(u32_a, 1u, 1, 3));
		REQUIRE(u32_a.count == 16);
		REQUIRE(u32_a.data[1] == u32_a.data[2]);
		REQUIRE(u32_a.data[2] == u32_a.data[3]);
		REQUIRE(u32_a.data[3] == 1u);

		array::release(u32_a);
	}

	SECTION("Remove elements") {
		array::dynamic<types::uint32> u32_a = { 0 };
		u32_a.ator = memory::get_default_allocator();

		REQUIRE(array::init(u32_a, 7));
		REQUIRE(array::resize(u32_a, 7));

		// [0,1,2,3,4,5,6]
		for (types::uint32 i = 0; i < u32_a.count; ++i) {
			u32_a.data[i] = i;
		}

		// [0,1,2,5,6]
		REQUIRE(array::remove(u32_a, 2, 2));
		REQUIRE(u32_a.count == 5);
		REQUIRE(u32_a.capacity == 8);
		REQUIRE(u32_a.data[3] == 5u);
		REQUIRE(u32_a.data[4] == 6u);

		// [0,1,2]
		REQUIRE(array::remove(u32_a, 3, 3));
		REQUIRE(u32_a.count == 3);

		// [1,2]
		REQUIRE(array::remove(u32_a, 0, 1));
		REQUIRE(u32_a.count == 2);
		REQUIRE(u32_a.capacity == 8);
		REQUIRE(u32_a.data[0] == 1u);

		array::release(u32_a);
	}

	SECTION("Loose/Replace elements") {
		array::dynamic<types::uint8> u8_a = { 0 };
		u8_a.ator = memory::get_default_allocator();

		REQUIRE(array::init(u8_a, 8));
		REQUIRE(array::resize(u8_a, 8));

		// [0,1,2,3,4,5,6,7]
		for (types::uint8 i = 0; i < u8_a.count; ++i) {
			u8_a.data[i] = i;
		}

		// [0,1,2,6,7,5]
		types::uint8 a1[] = { 0, 1, 2, 6, 7, 5 };
		REQUIRE(array::replace_with_last(u8_a, 3, 2));
		REQUIRE(u8_a.count == 6);
		REQUIRE(memory::is_equal(a1, u8_a.data, 6));

		// [0,1,2,6]
		REQUIRE(array::replace_with_last(u8_a, 4, 3));
		REQUIRE(u8_a.count == 4);
		REQUIRE(memory::is_equal(a1, u8_a.data, 4));

		// [0,6]
		REQUIRE(array::replace_with_last(u8_a, 1, 2));
		REQUIRE(u8_a.count == 2);
		REQUIRE(u8_a.data[0] == 0);
		REQUIRE(u8_a.data[1] == 6);

		REQUIRE(array::replace_with_last(u8_a, 0, 4));
		REQUIRE(array::is_empty(u8_a));

		array::release(u8_a);
	}

	SECTION("Write buffers") {
		array::dynamic<types::char8> str_a = { 0 };
		str_a.ator = memory::get_default_allocator();

		const array::dynamic<types::char8> phrase_a = {
			"Lore Ipsum est", 14, 16, nullptr
		};

		REQUIRE(array::resize(str_a, 14));

		REQUIRE(array::write_buffer(phrase_a.data, 4, str_a));
		REQUIRE(memory::is_equal(str_a.data, phrase_a.data, 4));

		REQUIRE(array::write_buffer(phrase_a.data,
			array::compute_size<types::char8>(phrase_a.count), str_a));
		REQUIRE(array::equal(str_a, phrase_a));
	}

	SECTION("Copy array") {
		array::dynamic<types::char8> str_a = { 0 };
		str_a.ator = memory::get_default_allocator();

		const array::dynamic<types::char8> phrase_a = {
			"Lore Ipsum est", 14, 16, nullptr
		};

		REQUIRE(array::copy(str_a, phrase_a));
		REQUIRE(array::equal(str_a, phrase_a));
	}

	SECTION("Insert array") {
		array::dynamic<types::char8> str_a = { 0 };
		str_a.ator = memory::get_default_allocator();

		const array::dynamic<types::char8> name_a = {
			"Fabio", 5, 0, nullptr
		};

		const array::dynamic<types::char8> surname_a = {
			"Polimeni", 8, 0, nullptr
		};

		// [?,P,o,l,i,m,e,n,i]
		REQUIRE(array::insert(str_a, 1, surname_a));
		REQUIRE(array::get_count(str_a) == 9);
		
		array::at(str_a, 0) = ' ';

		// [F,a,b,i,o, ,P,o,l,i,m,e,n,i]
		REQUIRE(array::insert(str_a, 0, name_a));

		REQUIRE(memory::is_equal(str_a.data, "Fabio Polimeni", 14));
	}

	SECTION("Append array") {
		array::dynamic<types::char8> str_a = { 0 };
		str_a.ator = memory::get_default_allocator();

		const array::dynamic<types::char8> phrase_a = {
			"Lore Ipsum est", 14, 16, nullptr
		};

		REQUIRE(array::append(str_a, phrase_a));
		REQUIRE(array::equal(str_a, phrase_a));
	}

	SECTION("Extract array") {
		array::dynamic<types::char8> into_a = { 0 };
		REQUIRE(array::init(into_a));

		array::dynamic<types::char8> from_a = { 0 };
		REQUIRE(array::init(from_a, 14));
		REQUIRE(array::resize(from_a, 14));
		REQUIRE(array::write_buffer("Lore Ipsum est", 14, from_a));

		REQUIRE(array::extract(from_a, 5, 6, into_a));
		REQUIRE(memory::is_equal(into_a.data, "Ipsum", 5));
		REQUIRE(memory::is_equal(from_a.data, "Lore est", from_a.count));
	}

	SECTION("Push/Pop elements") {
		const array::dynamic<types::char8> phrase_a = {
			"Lore", 4, 8, nullptr
		};

		auto* str_a = array::make_copy(phrase_a);
		
		REQUIRE(array::push(*str_a, ' '));
		REQUIRE(array::push(*str_a, 'I'));
		REQUIRE(array::push(*str_a, 'p'));
		REQUIRE(array::push(*str_a, 's'));
		REQUIRE(array::push(*str_a, 'u'));
		REQUIRE(array::push(*str_a, 'm'));
		REQUIRE(memory::is_equal(str_a->data, "Lore Ipsum", str_a->count));
		REQUIRE(array::get_count(*str_a) == 10);
		REQUIRE(array::get_capacity(*str_a) == 16);
		
		auto count = array::get_count(*str_a);
		for (auto i = 0u; i < count; ++i) {
			types::char8 c;
			REQUIRE(array::pop(*str_a, c));
		}

		REQUIRE(array::is_empty(*str_a));

		types::char8 e = 'r';
		REQUIRE(array::pop(*str_a, e) == false);
		REQUIRE(e == 'r');

		array::destroy(str_a);
	}
}
