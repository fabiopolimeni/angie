// Copyright (c) 2018 Fabio Polimeni
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "angie/core/defines.hpp"

#if defined(ANGIE_HAVE_CPP11_INITIALIZER_LIST)
#include <initializer_list>
#else
namespace std {
    // See the C++11 Standard, section 18.9.
    // @at: https://github.com/electronicarts/EASTL/blob/master/include/EASTL/initializer_list.h
    template<class E> 
    class initializer_list {
    public:
        typedef E         value_type;
        typedef const E&  reference;
        typedef const E&  const_reference;
        typedef size_t    size_type;
        typedef const E*  iterator;             // Must be const, as initializer_list (and its mpArray) is an immutable temp object.
        typedef const E*  const_iterator;

    private:
        iterator  _array;
        size_type _array_size;

        // This constructor is private, but the C++ compiler has the ability to call it, as per the C++11 Standard.
        constexpr initializer_list(const_iterator p_array, size_type array_size) noexcept
            : _array(p_array), _array_size(array_size) { }

    public:
        constexpr initializer_list() noexcept
            : _array(nullptr), _array_size(0) { }

        constexpr size_type      size()  const noexcept { return _array_size; }
        constexpr const_iterator begin() const noexcept { return _array; }
        constexpr const_iterator end()   const noexcept { return _array + _array_size; }
    };

    template<class T>
    constexpr const T* begin(std::initializer_list<T> list) noexcept {
        return list.begin();
    }

    template<class T>
    constexpr const T* end(std::initializer_list<T> list) noexcept {
        return list.end();
    }
}
#endif

// Define the initializer list as if it was part of the core
namespace angie {
    namespace core {
        template<typename T>
        using initializer = std::initializer_list<T>;
    }
}