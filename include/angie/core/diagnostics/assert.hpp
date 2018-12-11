// Copyright (c) 2018 Fabio Polimeni
// Created on: 09/12/2018
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT
#pragma once

#include "angie/core/config.hpp"
#include "angie/core/defines.hpp"

#if defined ANGIE_DEBUG_TOOLS
#define DBG_TOOLS_ASSERT_ENABLE
#include <dbgtools/assert.h>

#define angie_assert ASSERT
#define angie_verify(cond) VERIFY(cond)

#undef DBG_TOOLS_ASSERT_ENABLE
#else
#include <assert.h>

#define angie_assert(cond, ...) assert(cond)

#   if defined ANGIE_DEBUG
#   define angie_verify(cond) ((void)((!(cond)) && assert(cond)))
#   else
#   define angie_verify(cond) ((void)(cond))
#   endif

#endif
