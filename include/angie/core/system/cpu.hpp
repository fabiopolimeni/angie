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
        #if defined(ANGIE_CPU_x86)
            enum x86_cpu_feature : uint8_t {
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

                CX8,                /*!< CMPXCHG8B supported */
                RDTSC,              /*!< RDTCS instruction present */

                COUNT
            };
            using cpu_feature = x86_cpu_feature;
        #elif defined(ANGIE_CPU_ARM)
            enum arm_cpu_feature : uint8_t {
                AES,
                ATOMICS,
                CRC32,
                FCMA,
                FP16_ARITH,
                JSCVT,
                IDIV,
                NEON,
                NEON_FMA,
                NEON_FP16,
                NEON_FP16_ARITH,
                NEON_RDM,
                PMULL,
                SHA1,
                SHA2,
                THUMB,
                THUMB2,
                VFPV2,
                VFPV3,
                VFPV3_D32,
                VFPV3_FP16,
                VFPV3_FP16_D32,
                VFPV4,
                VFPV4_D32,
                WMMX,
                WMMX2,

                COUNT
            };
            using cpu_feature = arm_cpu_feature;
        #elif defined(ANGIE_CPU_MIPS)
            enum mips_cpu_feature : uint8_t {
                MSA,                /* MIPS SIMD instructions */
                EVA,                /* Enhanced Virtual Addressing */
                XPA,                /* Extended Physical Address*/

                COUNT
            };
            using cpu_feature = mips_cpu_feature;
        #elif defined(ANGIE_CPU_PPC)
            enum ppc_cpu_feature : uint8_t {
                PPC32,
                PPC64,
                ALTIVEC,
                FPU,
                MMU,
                UNIFIED_CACHE,
                CELL,
                SMT,
                DFP,
                VSX,
                VCRYPTO,
                IEEE128,
                DARN,
                SCV

                COUNT
            };
            using cpu_feature = ppc_cpu_feature;
        #else
            enum not_supported_cpu_feature : uint8_t {
                COUNT = 0
            };
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
                const types::uint8    level;
                const types::size     total_size;
                const types::size     line_size;
                const cache_usage     usage;
                const cache_access    access;
            };

            /**
             * This structure holds the CPU information.
             *
			 * @param id Identifies which CPU this refers to
             * @param brand_name CPU friendly name
			 * @param physical_cores Number of physical cores
			 * @param logical_processors Number of logical processors (HT)
			 * @param data_caches Caches information
			 * @param features CPU capable features
             */
            struct cpu_info {
                const types::index		            id;
                const types::char8*                 brand_name;
                const types::uint32                 physical_cores;
                const types::uint32                 logical_processors;
                const dynamic_array<cpu_cache>      data_caches;   
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
             * Release memory allocated to store cpu information.
             * 
             * @param cpus Array containing system cpu information
             */
            void release_cpu_info(containers::dynamic_array<cpu_info>& cpus);

			/**
			 * Return the current processor id the current thread is executing from.
			 *
			 * @return Return the current logical processor id.
			 */
			types::index get_current_processor();

        }
    }
}
