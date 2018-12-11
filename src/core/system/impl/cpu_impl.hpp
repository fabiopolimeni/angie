// Copyright (c) 2017 Fabio Polimeni
// Created on: 10/05/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "angie/core/system/cpu.hpp"

namespace angie {
	namespace core {
		namespace system {
			namespace impl {

				/**
				 * Retrieve CPU system information.
				 *
				 * The number of CPUs found available is the count of the
				 * dynamic array passed in. This array doesn't need to be
				 * initialised, although, feel free to do so, in case you
				 * want to use an allocator different from the default one.
				 *
				 * @param cpus Array of CPUs found available on the system.
				 * @return true if query CPU info is supported on the current
				 *         system, false otherwise.
				 */
				types::boolean query_cpu_info(containers::dynamic_array<cpu_info*>& cpus);

				/**
				 * Return the current CPU the calling thread is running on.
				 * 
				 * If a valid index is returned, this refers to the cpu stored
				 * in the array returned by `query()` function, and the `id`
				 * property of the `info` structure must match it.
				 *
				 * @return CPU id if valid, or `invalid_index` if fails.
				 */
				types::index get_current_cpu_id();

			}
		}
	}
}
