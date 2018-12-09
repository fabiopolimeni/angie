// Copyright (c) 2017 Fabio Polimeni
// Created on: 09/04/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "angie/core/types.hpp"

namespace angie {
	namespace core {
		namespace containers {

			/**
			 * Current state of the array.
			 */
			enum class state : types::int8 {
				ready,                  /*!< Ready to be manipulated */
				inconsistent_properties,/*!< Properties are inconsistent */
				invalid_allocator,		/*!< A valid allocator is compulsory */
			};

			/**
			 * Pointer helpers.
			 */
			enum class index : types::uintptr {
				begin = 0,			/*!< Identifies the origin of the array */
				end = SIZE_MAX,		/*!< Identifies the end of the array */
				none = end			/*!< Identifies and invalid index */
			};

        }
    }
}
