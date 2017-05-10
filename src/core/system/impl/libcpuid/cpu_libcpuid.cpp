// Copyright (c) 2017 Fabio Polimeni
// Created on: 10/05/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "angie/core/system/cpu.hpp"
#include "../cpu_impl.hpp"

namespace angie {
	namespace core {
		namespace cpu {
			namespace impl {

				types::boolean query(array::dynamic<info*>& cpus) {
					return false;
				}

				types::index get_current_id() {
					return invalid_index;
				}

			}
		}
	}
}
