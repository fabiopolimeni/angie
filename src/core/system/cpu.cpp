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
		namespace system {

			types::boolean query_cpu_info(containers::dynamic_array<cpu_info*>& cpus) {
				return impl::query_cpu_info(cpus);
			}

			types::index get_current_cpu_id() {
				return impl::get_current_cpu_id();
			}
		}
	}
}