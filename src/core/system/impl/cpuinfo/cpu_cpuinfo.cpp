// Copyright (c) 2017 Fabio Polimeni
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "angie/core/system/cpu.hpp"
#include "angie/core/containers/dynamic_array.hpp"
#include "cpuinfo.h"

#if defined(ANGIE_OS_WIN)
#include "cpu_cpuinfo_win.cpp"
#endif

namespace {
	using namespace angie::core::system;
	using namespace angie::core::types;

	cpu_cache cpuinfo_to_cpu_cache(const cpuinfo_cache* cache, uint8 cache_id) {
		return { 
			cache_id,
			cache->size,
			cache->line_size,
			cache_usage::DATA,
			cache->flags & CPUINFO_CACHE_UNIFIED
				? cache_access::UNIFIED
				: cache_access::DEDICATED
		};
	}

#if defined(ANGIE_CPU_x86)
	void cpuinfo_load_features(dynamic_array<boolean>& features) {
		features[cpu_feature::ABM] 			= cpuinfo_has_x86_lzcnt() && cpuinfo_has_x86_popcnt();
		features[cpu_feature::AES] 			= cpuinfo_has_x86_aes();
		features[cpu_feature::SHA_NI] 		= cpuinfo_has_x86_sha();
		features[cpu_feature::RDRAND] 		= cpuinfo_has_x86_rdrand();
		features[cpu_feature::RDSEED] 		= cpuinfo_has_x86_rdseed();
		features[cpu_feature::RDTSCP] 		= cpuinfo_has_x86_rdtscp();
		features[cpu_feature::RDTSC] 		= cpuinfo_has_x86_rdtsc();
		features[cpu_feature::F16C] 		= cpuinfo_has_x86_f16c();
		features[cpu_feature::FMA3] 		= cpuinfo_has_x86_fma3();
		features[cpu_feature::FMA4] 		= cpuinfo_has_x86_fma4();
		features[cpu_feature::CX16] 		= cpuinfo_has_x86_cmpxchg16b();
		features[cpu_feature::CX8] 			= cpuinfo_has_x86_cmpxchg8b();

		features[cpu_feature::SSE] 			= cpuinfo_has_x86_sse();
		features[cpu_feature::SSE2] 		= cpuinfo_has_x86_sse2();
		features[cpu_feature::SSSE3] 		= cpuinfo_has_x86_ssse3();
		features[cpu_feature::SSE4_1] 		= cpuinfo_has_x86_sse4_1();
		features[cpu_feature::SSE4_2] 		= cpuinfo_has_x86_sse4_2();
		features[cpu_feature::SSE4A] 		= cpuinfo_has_x86_sse4a();
		
		features[cpu_feature::AVX] 			= cpuinfo_has_x86_avx();
		features[cpu_feature::AVX2] 		= cpuinfo_has_x86_avx2();
		features[cpu_feature::AVX512F] 		= cpuinfo_has_x86_avx512f();
		features[cpu_feature::AVX512DQ] 	= cpuinfo_has_x86_avx512dq();
		features[cpu_feature::AVX512PF] 	= cpuinfo_has_x86_avx512pf();
		features[cpu_feature::AVX512ER] 	= cpuinfo_has_x86_avx512er();
		features[cpu_feature::AVX512CD] 	= cpuinfo_has_x86_avx512cd();
		features[cpu_feature::AVX512BW] 	= cpuinfo_has_x86_avx512bw();
		features[cpu_feature::AVX512VL] 	= cpuinfo_has_x86_avx512vl();
	}
#elif defined(ANGIE_CPU_ARM)
	void cpuinfo_load_features(dynamic_array<boolean>& features) {
		features[cpu_feature::AES]				= cpuinfo_has_arm_aes();
		features[cpu_feature::ATOMICS]			= cpuinfo_has_arm_atomics();
		features[cpu_feature::CRC32]			= cpuinfo_has_arm_crc32();
		features[cpu_feature::FCMA]				= cpuinfo_has_arm_fcma();
		features[cpu_feature::FP16_ARITH]		= cpuinfo_has_arm_fp16_arith();
		features[cpu_feature::JSCVT]			= cpuinfo_has_arm_jscvt();
		features[cpu_feature::IDIV]				= cpuinfo_has_arm_idiv();
		features[cpu_feature::NEON]				= cpuinfo_has_arm_neon();
		features[cpu_feature::NEON_FMA]			= cpuinfo_has_arm_neon_fma();
		features[cpu_feature::NEON_FP16]		= cpuinfo_has_arm_neon_fp16();
		features[cpu_feature::NEON_FP16_ARITH]	= cpuinfo_has_arm_neon_fp16_arith();
		features[cpu_feature::NEON_RDM]			= cpuinfo_has_arm_neon_rdm();
		features[cpu_feature::PMULL]			= cpuinfo_has_arm_pmull();
		features[cpu_feature::SHA1]				= cpuinfo_has_arm_sha1();
		features[cpu_feature::SHA2]				= cpuinfo_has_arm_sha2();
		features[cpu_feature::THUMB]			= cpuinfo_has_arm_thumb();
		features[cpu_feature::THUMB2]			= cpuinfo_has_arm_thumb2();
		features[cpu_feature::VFPV2]			= cpuinfo_has_arm_vfpv2();
		features[cpu_feature::VFPV3]			= cpuinfo_has_arm_vfpv3();
		features[cpu_feature::VFPV3_D32]		= cpuinfo_has_arm_vfpv3_d32();
		features[cpu_feature::VFPV3_FP16]		= cpuinfo_has_arm_vfpv3_fp16();
		features[cpu_feature::VFPV3_FP16_D32]	= cpuinfo_has_arm_vfpv3_fp16_d32();
		features[cpu_feature::VFPV4]			= cpuinfo_has_arm_vfpv4();
		features[cpu_feature::VFPV4_D32]		= cpuinfo_has_arm_vfpv4_d32();
		features[cpu_feature::WMMX]				= cpuinfo_has_arm_wmmx();
		features[cpu_feature::WMMX2]			= cpuinfo_has_arm_wmmx2();
	}
#else
#	error "CPU features not supported for the given architecture"
#endif
}

namespace angie {
	namespace core {
		namespace system {

			types::boolean query_cpu_info(
				containers::dynamic_array<cpu_info>& cpus) {
				const auto cpuinfo_ok = cpuinfo_initialize();
                if (!cpuinfo_ok)
                    return false;

				// cpuinfo is an advanced cpu detector, it distinguishes between
				// clusters, or groups (e.g. big.LITTLE heterogeneous architectures)
				// and CPUs on different physical packages. In order to keep the
				// API simple, we don't distinguish between the two, therefore,
				// their cluster structures, represents our single CPU. This will
				// still allow us to distinguish between "faster/slower" CPUs.
                const struct cpuinfo_cluster* clusters_begin = cpuinfo_get_clusters();
					
                const auto num_of_clusters = cpuinfo_get_clusters_count();
				const auto cpuarray_initialized = containers::init(cpus, num_of_clusters);
                if (cpuarray_initialized) {
					for (types::index clid = 0; clid < num_of_clusters; ++clid) {
                    	const struct cpuinfo_cluster* cluster = clusters_begin;

						containers::dynamic_array<cpu_cache> data_caches;
						containers::reserve(data_caches, 4);

						// cpu_cache mock_cache = {
						// 	1, 64000, 32, cache_usage::INSTRUCTION, cache_access::DEDICATED
						// };

						// containers::push(data_caches, mock_cache);
						// return true;
						
						// We assume that a cluster shares the same
						// cache caracteristis throughout the whole
						// logical processors. Therefore, simply
						// querying the first logical processor in
						// the cluster will return the cache info.
						const auto* cache_l1d = cpuinfo_get_l1d_cache(
							cluster->processor_start);

						if (cache_l1d) {
							containers::push(
								data_caches, cpuinfo_to_cpu_cache(cache_l1d, 1));
						}

						const auto* cache_l2 = cpuinfo_get_l2_cache(
							cluster->processor_start);

						if (cache_l2) {
							containers::push(
								data_caches, cpuinfo_to_cpu_cache(cache_l2, 2));
						}

						const auto* cache_l3 = cpuinfo_get_l3_cache(
							cluster->processor_start);

						if (cache_l3) {
							containers::push(
								data_caches, cpuinfo_to_cpu_cache(cache_l3, 3));
						}

						const auto* cache_l4 = cpuinfo_get_l4_cache(
							cluster->processor_start);

						if (cache_l4) {
							containers::push(
								data_caches, cpuinfo_to_cpu_cache(cache_l4, 4));
						}

						// Features identification
						dynamic_array<types::boolean> features;
						containers::init(features, cpu_feature::COUNT);
						containers::resize(features, cpu_feature::COUNT);
						containers::set(features, false, 0, cpu_feature::COUNT);
						cpuinfo_load_features(features);

						// Add the cpu to the list
						containers::push(cpus, {
							clid,
							cluster->package->name,
							cluster->core_count,
							cluster->processor_count,
							data_caches,
							features
						});
					}

					return true;
                }

				return false;
			}

			void release_cpu_info(containers::dynamic_array<cpu_info>& cpus) {
				// Release memory allocaed to get cpu information
				cpuinfo_deinitialize();
				containers::release(cpus);
			}

			types::index get_current_processor() {
				#if defined(ANGIE_OS_WIN)
				return win::get_current_processor();
				#else
				const auto* processor = cpuinfo_get_current_processor();
				if (processor) {
					return processor->smt_id;
				}
				#endif

				return invalid_index;
			}
		}
	}
}
