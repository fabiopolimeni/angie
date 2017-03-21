#include_directories(${CMAKE_SOURCE_DIR}/include)
#message(STATUS "CMAKE_SOURCE_DIR: " ${CMAKE_SOURCE_DIR})

# Shared resources
set(HEADER_FILES
        angie/core/system/system.hpp
        angie/core/config.hpp
        angie/core/defines.hpp
        angie/core/version.hpp)

set(SOURCE_FILES
        memory/global.cpp
        system/system.cpp)

add_library(angie_core STATIC ${SOURCE_FILES})

# Include external directories
include_directories(../../externals)

# Memory - ltalloc
if (angie_memory_global_ltalloc)
    set(SOURCE_MEMORY_FILES
            ../../externals/ltalloc/ltalloc.cc
            memory/impl/ltalloc/global_ltalloc.cpp)

    set_source_files_properties(../../externals/ltalloc/ltalloc.cc
            PROPERTIES
            COMPILE_DEFINITIONS LTALLOC_DISABLE_OPERATOR_NEW_OVERRIDE)

    target_sources(angie_core PRIVATE ${SOURCE_MEMORY_FILES})
endif()

# System - plibsys
if (angie_system_plibsys)
    set(SOURCE_SYSTEM_FILES
            system/impl/plibsys/system_plibsys.cpp)

    target_sources(angie_core PRIVATE ${SOURCE_SYSTEM_FILES})

    set(PLIBSYS_DIR ../../externals/plibsys)
    option(PLIBSYS_TESTS "" OFF)
    option(PLIBSYS_TESTS_STATIC "" OFF)
    option(PLIBSYS_BUILD_STATIC "" ON)
    add_subdirectory(${PLIBSYS_DIR} plibsys EXCLUDE_FROM_ALL)
    #include(${PLIBSYS_DIR}/CMakeLists.txt)

    #message(STATUS
    #        "\nBINARY_DIR: " ${CMAKE_BINARY_DIR}
    #        "\nCURRENT_BINARY_DIR: " ${CMAKE_CURRENT_BINARY_DIR})
    include_directories(${PLIBSYS_DIR}/src ${CMAKE_BINARY_DIR})
    target_link_libraries(angie_core plibsysstatic)
endif()

set_target_properties(angie_core PROPERTIES FOLDER "angie")
install(TARGETS angie_core DESTINATION lib)