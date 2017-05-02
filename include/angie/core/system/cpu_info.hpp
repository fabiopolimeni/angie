// Copyright (c) 2017 Fabio Polimeni
// Created on: 03/04/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "angie/core/base.hpp"
#include "angie/core/containers/dynamic_array.hpp"

namespace angie {
    namespace core {
        namespace cpu {

            /**
             * Some major cpu features identifiers
             */
            enum feature {
                PSE,                /*!< Page size extension */
                TSC,                /*!< Time-stamp counter */
                CLFLUSH,            /*!< CLFLUSH instruction supported */
                DTS,                /*!< Debug store supported */
                SSE,                /*!< Streaming-SIMD Extensions supported */
                SSE2,               /*!< SSE2 instructions supported */
                HT,                 /*!< Hyper-threading supported */
                TM,                 /*!< Thermal monitor */
                IA64,               /*!< IA64 supported (Itanium only) */
                PNI,                /*!< PNI (SSE3) instructions supported */
                DTS64,              /*!< 64-bit Debug store supported */
                VMX,                /*!< Virtualization technology supported */
                SMX,                /*!< Safer mode exceptions */
                TM2,                /*!< Thermal monitor 2 */
                SSSE3,              /*!< SSSE3 instructions supported */
                CID,                /*!< Context ID supported */
                CX16,               /*!< CMPXCHG16B instruction supported */
                DCA,                /*!< Direct cache access supported */
                SSE4_1,             /*!< SSE 4.1 instructions supported */
                SSE4_2,             /*!< SSE 4.2 instructions supported */
                SYSCALL,            /*!< SYSCALL/SYSRET instructions */
                AES,                /*!< AES* instructions supported */
                AVX,                /*!< Advanced vector extensions supported */
                MMXEXT,             /*!< AMD MMX-extended instructions */
                AMD3DNOW,           /*!< AMD 3DNow! instructions supported */
                AMD3DNOWEXT,        /*!< AMD 3DNow! extended instructions */
                RDTSCP,             /*!< RDTSCP instruction supported (AMD) */
                LM,                 /*!< Long mode (x86_64/EM64T) supported */
                ABM,                /*!< LZCNT instruction support */
                MISALIGNSSE,        /*!< Misaligned SSE supported */
                SSE4A,              /*!< SSE 4a from AMD */
                XOP,                /*!< The XOP instruction set */
                AMD3DNOWPREFETCH,   /*!< PREFETCH/PREFETCHW support */
                WDT,                /*!< Watchdog timer support */
                TS,                 /*!< Temperature sensor */
                CONSTANT_TSC,       /*!< TSC ticks at constant rate */
                FMA3,               /*!< The FMA3 instruction set */
                FMA4,               /*!< The FMA4 instruction set */
                TBM,                /*!< Trailing bit manipulation support */
                F16C,               /*!< 16-bit FP convert support */
                RDRAND,             /*!< RdRand instruction */
                RDSEED,             /*!< RDSEED instruction */
                AVX2,               /*!< AVX2 instructions */
                AVX512F,            /*!< AVX-512 Foundation */
                AVX512DQ,           /*!< AVX-512 Double/Quad granular */
                AVX512PF,           /*!< AVX-512 Prefetch */
                AVX512ER,           /*!< AVX-512 Exponential/Reciprocal */
                AVX512CD,           /*!< AVX-512 Conflict detection */
                AVX512BW,           /*!< AVX-512 Byte/Word granular */
                AVX512VL,           /*!< AVX-512 128/256 vector length */
                SHA_NI,             /*!< SHA-1/SHA-256 instructions */
                SGX,                /*!< SGX extensions */

                COUNT
            };

            /**
             * Typical cache level identifiers
             */
            enum cache {
                L1,
                L2,
                L3,
                L4,

                COUNT
            };

            /**
             * This structure holds the CPU information.
             *
             * @param name CPU friendly name
			 * @param id Identifies which CPU this refers to
			 * @param physical_cores Number of physical cores
			 * @param logical_processors Number of logical CPUs (HT)
			 * @param data_cache Data cache sizes per cache level
			 * @param instruction_cache Instruction cache sizes per cache level
			 * @param cache_line Cache line sizes per cache level
			 * @param features CPU capable features
             */
            struct info {
                const types::char8*     name;
                const types::uint32     id;
                const types::uint32     physical_cores;
                const types::uint32     logical_processors;
                const types::size       data_cache[cache::COUNT];
                const types::size       instruction_cache[cache::COUNT];
                const types::size       cache_line[cache::COUNT];
                const types::boolean    features[feature::COUNT];
            };

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
            types::boolean query(array::dynamic<info*>& cpus);

        }
    }
}
