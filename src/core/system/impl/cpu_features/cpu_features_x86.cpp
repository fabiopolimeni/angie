// Copyright (c) 2017 Fabio Polimeni
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "angie/core/system/cpu.hpp"
#include "angie/core/containers/dynamic_array.hpp"

namespace angie {
	namespace core {
		namespace system {

			types::boolean query_cpu_info(
				containers::dynamic_array<cpu_info>& cpus) {
				return false;
			}

			void release_cpu_info(containers::dynamic_array<cpu_info>& cpus) {
				
			}

			types::boolean get_current_core(types::index& core_id) {
				return false;
			}
		}
	}
}
