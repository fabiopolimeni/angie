// Copyright (c) 2017 Fabio Polimeni
// Created on: 12/04/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "angie/core/base.hpp"

#ifdef ANGIE_DEBUG_TOOLS
// TODO: Consider create our own defines
#define DBG_TOOLS_ASSERT_ENABLE
#include "dbgtools/assert.h"
#define angie_assert ASSERT
#define angie_verify VERIFY
#undef DBG_TOOLS_ASSERT_ENABLE
#else
#define angie_assert(cond, ...)
#define angie_verify(cond, ...) (cond)
#endif
