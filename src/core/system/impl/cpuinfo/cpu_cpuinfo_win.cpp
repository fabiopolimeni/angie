// Copyright (c) 2017 Fabio Polimeni
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "angie/core/types.hpp"

#include <immintrin.h>
#include <intrin.h>  // For __cpuidex()

namespace win {

    typedef struct {
        uint32_t eax, ebx, ecx, edx;
    } leaf_t;

    leaf_t get_cpuid(uint32_t func_id) {
        leaf_t leaf;
        int32_t data[4];
    
        __cpuid(data, func_id);
        leaf.eax = data[0];
        leaf.ebx = data[1];
        leaf.ecx = data[2];
        leaf.edx = data[3];

        return leaf;
    }

    using namespace angie::core;
    types::index get_current_processor() {
        auto leaf = get_cpuid(1);

        // EBX:31-24 bits return the current executing
        // processor the cpuid has been called from.
        return leaf.ebx & 0x000000FF;
    }

}
