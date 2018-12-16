// Copyright (c) 2017 Fabio Polimeni
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "angie/core/defines.hpp"

#if defined(ANGIE_CPU_x86)
#include "cpu_features_x86.cpp"
#elif defined(ANGIE_CPU_ARM)
#include "cpu_features_arm.cpp"
#elif defined(ANGIE_CPU_MIPS)
#include "cpu_features_mips.cpp"
#elif defined(ANGIE_CPU_PPC)
#include "cpu_features_ppc.cpp"
#else
#error cpu_features does not support the given CPU architecture
#endif
