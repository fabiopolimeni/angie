// Copyright (c) 2017 Fabio Polimeni
// Created on: 10/05/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "angie/core/diagnostics/assert.hpp"
#include "angie/core/system/cpu.hpp"
#include "impl/cpu_impl.hpp"

namespace angie {
	namespace core {
		namespace cpu {

			types::boolean query(containers::dynamic_array<info*>& cpus) {
				return impl::query(cpus);
			}

			types::index get_current_id() {
				return impl::get_current_id();
			}
		}
	}
}