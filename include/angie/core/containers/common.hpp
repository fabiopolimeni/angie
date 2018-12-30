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

        }
    }
}
