// Copyright (c) 2017 Fabio Polimeni
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "angie/core/system/cpu.hpp"
#include "angie/core/containers/dynamic_array.hpp"
#include "cpuinfo.h"

namespace {
	using namespace angie::core::system;
	using namespace angie::core::types;
	
	const char8* cpuinfo_vendor_to_string(cpuinfo_vendor vendor) {
		return nullptr;
	}

	const char8* cpuinfo_arch_to_string(cpuinfo_uarch arch) {
		return nullptr;
	}

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
	void cpuinfo_get_features(dynamic_array<boolean>& features) {
		angie::core::containers::init(features, x86_cpu_feature::COUNT);
		angie::core::containers::resize(features, x86_cpu_feature::COUNT);

		features[cpu_feature::ABM] 			= cpuinfo_has_x86_lzcnt()
												&& cpuinfo_has_x86_popcnt();
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
	void cpuinfo_get_features(dynamic_array<boolean>& features) {
		// @todo:...
	}
#elif defined(ANGIE_CPU_MIPS)
	void cpuinfo_get_features(dynamic_array<boolean>& features) {
		// @todo:...
	}
#elif defined(ANGIE_CPU_PPC)
	void cpuinfo_get_features(dynamic_array<boolean>& features) {
		// @todo:...
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
                if (!cpuinfo_initialize())
                    return false;

				// cpuinfo is an advanced cpu detector, it distinguishes between
				// clusters (for example big.LITTLE heterogeneous architectures)
				// and CPUs on different physical packages. In order to keep the
				// api simple, we don't distinguish between the two, therefore,
				// their cluster structures, represents our single CPU. This will
				// still allow us to distinguish between "faster/slower" CPUs.
                const struct cpuinfo_cluster* clusters_begin = 
					cpuinfo_get_clusters();
					
                const auto num_of_clusters = cpuinfo_get_clusters_count();

                if (containers::init(cpus, num_of_clusters)) {
					for (types::index clid = 0; clid < num_of_clusters; ++clid) {
                    	const struct cpuinfo_cluster* cluster = clusters_begin;

						containers::dynamic_array<cpu_cache> data_caches;
						
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
						cpuinfo_get_features(features);

						// Add the cpu to the list
						containers::push(cpus, {
							clid,
							cpuinfo_vendor_to_string(cluster->vendor),
							cpuinfo_arch_to_string(cluster->uarch),
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

			types::boolean get_current_cpu_core(
				types::index& cpu, types::index& core) {
				return false;
			}
		}
	}
}
