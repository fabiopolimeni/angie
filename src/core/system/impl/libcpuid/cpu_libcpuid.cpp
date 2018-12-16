// Copyright (c) 2017 Fabio Polimeni
// Created on: 10/05/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "angie/core/system/cpu.hpp"
#include "libcpuid.h"

namespace angie {
	namespace core {
		namespace system {

			types::boolean query_cpu_info(
				containers::dynamic_array<cpu_info>& cpus) {
				return false;
			}

			types::boolean get_current_cpu_core(
				types::index& cpu, types::index& core) {
				return false;
			}
		}
	}
}
