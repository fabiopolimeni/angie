// Copyright (c) 2017 Fabio Polimeni
// Created on: 10/05/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "angie/core/system/cpu.hpp"

namespace angie {
	namespace core {
		namespace cpu {
			namespace impl {

				types::boolean query(containers::dynamic_array<info*>& cpus);
				
				types::index get_current_id();

			}
		}
	}
}
