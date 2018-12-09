cmake_minimum_required(VERSION 3.6)
project(angie)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_C_EXTENSIONS OFF) #...without compiler extensions like gnu11

set(BUILD_TIME "")
string(TIMESTAMP BUILD_TIME "%Y%m%d%H%M%S")

set(CPACK_PACKAGE_VERSION_MAJOR 0)
set(CPACK_PACKAGE_VERSION_MINOR 0)
set(CPACK_PACKAGE_VERSION_PATCH 1)
set(CPACK_PACKAGE_VERSION_BUILD ${BUILD_TIME})

# Set of options
option(angie_config_profile "Use profile configuration" OFF)
option(angie_memory_global_ltalloc "Use ltalloc for global memory" ON)
option(angie_system_plibsys "Use plibsys as base system library" ON)
option(angie_debug_tools "Use programmatic debug tools" OFF)

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
# Normally you want debug programmatic tools only on debug,
# although, you can switch to any build type you like,
# or combine them E.g. "Debug|Profile"
message(STATUS "CMAKE_BUILD_TYPE: " ${CMAKE_BUILD_TYPE})
if (CMAKE_BUILD_TYPE MATCHES "Debug")
    set(angie_debug_tools ON)
    include_directories(externals/dbgtools/include)
    add_definitions(-D_DEBUG_TOOLS)
endif()

# Add lib core directory
add_subdirectory(src/core)

# Add testing files
include(CTest)
add_subdirectory(test)