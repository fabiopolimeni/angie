// Copyright (c) 2017 Fabio Polimeni
// Created on: 18/03/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

/**
 * Configuration settings.
 *
 * DEBUG: Project built with debug information
 * RELEASE: Project built with optimisation
 * PROFILE: Like RELEASE but with profile information available
 */
#ifdef _DEBUG
#define ANGIE_DEBUG
#elif defined(_PROFILE)
#define ANGIE_PROFILE
#else
#define ANGIE_RELEASE
#endif

/**
 * If ANGIE_DEBUG_TOOLS, programmatic debug functions are available
 *
 * Utility functions are under namespace angie::core::debug
 */
#ifdef _DEBUG_TOOLS
#define ANGIE_DEBUG_TOOLS
#endif

/**
 * Define a default alignment if not passed by the build system
 */
#ifndef ANGIE_DEFAULT_MEMORY_ALIGNMENT
#define ANGIE_DEFAULT_MEMORY_ALIGNMENT 16
#endif

/**
 * Maximum allocable size in one call from the system.
 *
 * This is not a system limitation, though it can be useful
 * to catch as early as possible certain type of memory
 * corruption bugs.
 */
#ifndef ANGIE_MAX_ALLOCATION_SIZE
#define ANGIE_MAX_ALLOCATION_SIZE (1 << 30)
#endif

/**
 * Maximum number of characters of a formatted string
 */
#ifndef ANGIE_MAX_FORMAT_CHARS
#define ANGIE_MAX_FORMAT_CHARS 1024
#endif // ANGIE_MAX_FORMAT_CHARS