// Copyright (c) 2018 Fabio Polimeni
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "angie/core/types.hpp"

namespace angie {
    namespace core {
        namespace constants {
            const types::size       max_size = SIZE_MAX;
            const types::index      invalid_index = UINT32_MAX;
            const types::uintptr    begin_ptr = 0;			    /*!< Identifies the origin of the array */
            const types::uintptr    end_ptr = UINTPTR_MAX;	    /*!< Identifies the end of the array */
            const types::uintptr    not_found = UINTPTR_MAX;	/*!< Element not found */
        }

    }
}