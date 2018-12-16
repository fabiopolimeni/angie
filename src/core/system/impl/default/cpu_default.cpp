// Copyright (c) 2017 Fabio Polimeni
// Created on: 10/05/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "angie/core/system/cpu.hpp"
#include "angie/core/containers/dynamic_array.hpp"

namespace angie {
	namespace core {
		namespace system {

			types::boolean query_cpu_info(containers::dynamic_array<cpu_info>& cpus) {
				// Assume we have only one CPU, no big.LITTLE DynamicIQ or NUMA detection
				cpu_info cpu = {
					"Unknown CPU",				// Name
					0,							// Id
					1, 							// Cores
					2,							// Logical Processors
					{64000},					// Cache size
					{256},						// Cache line size
					{false}						// CPU features
				};

				containers::push(cpus, cpu);
				return true;
			}

			types::index get_current_cpu_id() {
				return 0; //core::invalid_index;
			}
		}
	}
}
