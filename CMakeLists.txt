cmake_minimum_required(VERSION 3.6)
project(angie)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_C_EXTENSIONS OFF) #...without compiler extensions like gnu11

if (MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
endif()

set(BUILD_TIME "")
string(TIMESTAMP BUILD_TIME "%Y%m%d%H%M%S")

set(CPACK_PACKAGE_VERSION_MAJOR 0)
set(CPACK_PACKAGE_VERSION_MINOR 0)
set(CPACK_PACKAGE_VERSION_PATCH 1)
set(CPACK_PACKAGE_VERSION_BUILD ${BUILD_TIME})

# Set of options
option(angie_config_profile "Use profile configuration" OFF)
option(angie_memory_global_ltalloc "Use ltalloc for global memory" ON)
option(angie_system_plibsys "Use plibsys as base system library" OFF)
option(angie_debug_tools "Use programmatic debug tools" ON)
option(angie_cpu_libcpuid "Use libcpuid to detect cpu info" OFF)

# Profile configuration
if (angie_config_profile)
    add_definitions(-D_PROFILE)
endif()

# Define some useful variables
set(ANGIE_PROJECT_DIR ${CMAKE_SOURCE_DIR})
set(ANGIE_INCLUDE_DIR ${ANGIE_PROJECT_DIR}/include)
set(ANGIE_SOURCE_DIR ${ANGIE_PROJECT_DIR}/src)

# Include directories of dependencies
include_directories(include)
include_directories(externals)

# Debug tools
# Normally you want diagnostic programmatic tools only for debug configuration,
# although, you can switch to any build type you like, or combine them.
# E.g. "Debug|Profile"
message(STATUS "CMAKE_BUILD_TYPE: " ${CMAKE_BUILD_TYPE})
if (CMAKE_BUILD_TYPE MATCHES "Debug")
    set(angie_debug_tools ON)
    include_directories(externals/dbgtools/include)
    add_definitions(-D_DEBUG_TOOLS)
endif()

function (angie_detect_os_arch result)
	if (CMAKE_SIZEOF_VOID_P EQUAL 8)
		set (ANGIE_OS_ARCH 64)
	else()
		set (ANGIE_OS_ARCH 32)
	endif()

	set (${result} ${ANGIE_OS_ARCH} PARENT_SCOPE)
endfunction (angie_detect_os_arch)

function (angie_detect_cpu_arch result)
	if (CMAKE_SYSTEM_PROCESSOR MATCHES "(i[1-9]86)|(x86_64)")
		if (CMAKE_CROSSCOMPILING)
			if (CMAKE_SYSTEM_PROCESSOR MATCHES "i[1-9]86")
				set (ANGIE_CPU_ARCH "x86")
			else()
				set (ANGIE_CPU_ARCH "x64")
			endif()
		else()
			angie_detect_os_arch(ANGIE_OS_BITS)
			if (ANGIE_OS_BITS STREQUAL "32")
				set (ANGIE_CPU_ARCH "x86")
			else()
				set (ANGIE_CPU_ARCH "x64")
			endif()
		endif()
	else()
			set (ANGIE_CPU_ARCH ${CMAKE_SYSTEM_PROCESSOR})
	endif()

	set (${result} ${ANGIE_CPU_ARCH} PARENT_SCOPE)
endfunction (angie_detect_cpu_arch)

# OS bits possible values 32|64
set(ANGIE_OS_BITS "")
angie_detect_os_arch(ANGIE_OS_BITS)
message(STATUS "ANGIE_OS_BITS: " ${ANGIE_OS_BITS})

# CPU architecture possible values x86|x64|ARM|ARM64
set(ANGIE_CPU_ARCH "")
angie_detect_cpu_arch(ANGIE_CPU_ARCH)
message(STATUS "ANGIE_CPU_ARCH: " ${ANGIE_CPU_ARCH})

# Add lib core directory
add_subdirectory(src/core)

# Add testing files
include(CTest)
add_subdirectory(test)