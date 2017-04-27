// Copyright (c) 2017 Fabio Polimeni
// Created on: 16/04/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "angie/core/defines.hpp"
#include "angie/core/types.hpp"

namespace angie {
    namespace core {
        namespace algorithm {

            template <typename T>
            using predicate = bool (const T& a, const T& b);

            template<typename T> inline constexpr
            bool less(const T& a, const T& b) { return a < b; }

			template<class T>
			inline constexpr
			const T& min(const T& a, const T& b, predicate<T> cmp = less)
			{
				return cmp(a, b) ? a : b;
			}

			template<class T>
			inline constexpr
			const T& max(const T& a, const T& b, predicate<T> cmp = less)
			{
				return cmp(a, b) ? b : a;
			}

            template<class T>
			inline constexpr
            const T& clamp(const T& v, const T& lo, const T& hi,
                            predicate<T> cmp = less)
            {
                return cmp(v, lo) ? lo : cmp(hi, v) ? hi : v;
            }

        }
    }
}
