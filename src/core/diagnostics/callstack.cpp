// Copyright (c) 2017 Fabio Polimeni
// Created on: 10/04/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "angie/core/diagnostics/callstack.hpp"
#include "dbgtools/callstack.h"

namespace angie {
	namespace core {
		namespace diagnostics {

			types::size grab_callstack(types::size skip_frames,
				void** addresses, types::size num_addresses) {
				return callstack(skip_frames, addresses, num_addresses);
			}

			types::size resolve_symbols(void** addresses, symbol* out_syms,
				types::size num_addresses, void* mem_buff,
				types::size mem_size) {
				static_assert(sizeof(symbol) == sizeof(callstack_symbol_t));
				return callstack_symbols(addresses, 
					reinterpret_cast<callstack_symbol_t*>(out_syms),
					num_addresses, static_cast<char*>(mem_buff), mem_size);
			}
		}
	}
}