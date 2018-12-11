// Copyright (c) 2017 Fabio Polimeni
// Created on: 12/04/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "angie/core/base.hpp"

namespace angie {
	namespace core {
		namespace diagnostics {

			struct symbol
			{
				const char*		function; /** function name */
				const char*		file;     /** file where symbol is defined */
				unsigned int	line;     /** line in the file */
				unsigned int	offset;   /** offset from function address */
			};

			/**
			 * Generate a callstack from the current location in the code
			 *
			 * @param skip_frames number of frames to skip in output to addresses
			 * @param addresses pointer to a buffer where to store addresses
			 * @param num_addresses size of addresses
			 * @return number of addresses in callstack
			 */
			types::size grab_callstack(types::size skip_frames,
				void** addresses, types::size num_addresses);

			/**
			 * Translate frame addresses to symbols.
			 *
			 * @param addresses list of pointers to translate
			 * @param out_syms array of translated symbols
			 * @param num_addresses number of addresses in addresses
			 * @param mem_buff memory used to allocate strings in out_syms
			 * @param mem_size size of addresses
			 * @return number of addresses translated
			 */
			types::size resolve_symbols(void** addresses, symbol* out_syms,
				types::size num_addresses, void* mem_buff,
				types::size mem_size);

		}
	}
}
