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
				// Assume we have only one CPU, no big.LITTLE,
				// DynamicIQ or NUMA implementation.
				cpu_info cpu = {
					0,							// Id
					"Unknown CPU",				// Unknown
					1, 							// Cores
					2							// Logical Processors
				};

				containers::push(cpus, cpu);
				return true;
			}

			types::boolean get_current_core(types::index& core_id) {
				core_id = 0;
				return true;
			}
		}
	}
}
