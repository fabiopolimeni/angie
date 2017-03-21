// Copyright (c) 2017 Fabio Polimeni
// Creation date: 18/03/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

/**
 * Configuration settings.
 *
 * DEBUG: Project built with debug information
 * RELEASE: Project built with optimisations
 * PROFILE: Like RELEASE but with profile information available
 */
#ifdef _DEBUG
#define ANGIE_DEBUG
#elif _PROFILE
#define AGNIE_PROFILE
#else
#define ANGIE_RELEASE
#endif

/**
 * Define a default alignment if not passed by the build system
 */
#ifndef ANGIE_DEFAULT_MEMORY_ALIGNMENT
#define ANGIE_DEFAULT_MEMORY_ALIGNMENT 16
#endif