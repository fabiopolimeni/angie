cmake_minimum_required(VERSION 3.6)
project(angie)

set(CMAKE_CXX_STANDARD 11)

set(CPACK_PACKAGE_VERSION_MAJOR 0)
set(CPACK_PACKAGE_VERSION_MINOR 0)
set(CPACK_PACKAGE_VERSION_PATCH 1)

include_directories(include)

# Set of options
option(angie_config_profile "Use profile configuration" OFF)
option(angie_memory_global_ltalloc "Use ltalloc for global memory managment" ON)
option(angie_system_plibsys "Use plibsys as base system library" ON)

# Profile configuration
if (angie_config_profile)
    add_definitions(-D_PROFILE)
endif()

# Add lib core directory
add_subdirectory(src/core)

# Add testing files
add_subdirectory(test)