// Copyright (c) 2017 Fabio Polimeni
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "angie/core/base.hpp"
#include "angie/core/containers/dynamic_array.hpp"

namespace angie {
    namespace core {
        namespace system {

            using namespace containers;

            /**
             * Some major cpu features identifiers
             */
            enum class x86_cpu_feature : uint8_t {
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

            enum class arm_cpu_feature : uint8_t {
                VFP,                /* Vector Floating Point */
                VFPV3,              /* VFP version 3 */
                VFPV3D16,           /* VFP version 3 for double precision */
                VFPV4,              /* VFP with fast context switching */
                IDIVA,              /* DDIV and UDIV hardware divisions in ARM mode */
                IDIVT,              /* DDIV and UDIV hardware divisions in Thumb mode */
                NEON,               /* Advanced SIMD instructions */
                AES,                /* Hardware accelerated Advanced Encryption Standards */
                SHA1,               /* Hardware accelerated SHA1 */
                SHA2,               /* Hardware accelerated SHA2-256 */
                CRC32,              /* Hardware accelerated CRC32 */
                PMULL,              /* Polynomial multiply long */

                COUNT
            };

            enum class mips_cpu_feature : uint8_t {
                MSA,                /* MIPS SIMD instructions */
                EVA,                /* Enhanced Virtual Addressing */
                XPA,                /* Extended Physical Address*/

                COUNT
            };

            enum class not_supported_cpu_feature : uint8_t {
                COUNT = 0
            };

            #if defined(ANGIE_CPU_x86)
            using cpu_feature = x86_cpu_feature;
            #elif defined(ANGIE_CPU_ARM)
            using cpu_feature = arm_cpu_feature;
            #elif defined(ANGIE_CPU_MIPS)
            using cpu_feature = mips_cpu_feature;
            #else
            #error Not supported CPU
            using cpu_feature = not_supported_cpu_feature;
            #endif

            /**
             * CPU cache usage type
             */
            enum class cache_usage {
                INSTRUCTION,
                DATA
            };

            /**
             * CPU cache access type
             */
            enum class cache_access {
                DEDICATED,
                UNIFIED
            };

            /*
             * CPU cache descriptor
             */
            struct cpu_cache {
                types::uint8    level;
                types::size     total_size;
                types::size     line_size;
                cache_usage     usage;
                cache_access    access;
            };

            /**
             * This structure holds the CPU information.
             *
             * @param name CPU friendly name
			 * @param id Identifies which CPU this refers to
			 * @param physical_cores Number of physical cores
			 * @param logical_processors Number of logical CPUs (HT)
			 * @param cache_sizes Data cache sizes per cache level
			 * @param cache_lines Cache line sizes per cache level
			 * @param features CPU capable features
             */
            struct cpu_info {
                const types::char8*                 name;
                const types::index		            id;
                const types::uint32                 physical_cores;
                const types::uint32                 logical_processors;
                const dynamic_array<cpu_cache>      caches;   
                const dynamic_array<types::boolean> features;
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
            types::boolean query_cpu_info(containers::dynamic_array<cpu_info>& cpus);

			/**
			 * Return the current core the calling thread is running on.
			 * 
			 * If a valid cpu_id is returned, this refers to the cpu stored
			 * in the array returned by `query_cpu_info()`, and the core_id
			 * is the core this function is currently executed from.
			 *
			 * @return true if a valid CPU and core are returned, false otherwise.
			 */
			types::boolean get_current_cpu_core(types::index& cpu, types::index &core);

        }
    }
}
