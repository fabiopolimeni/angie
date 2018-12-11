#include_directories(${CMAKE_SOURCE_DIR}/include)
#message(STATUS "CMAKE_SOURCE_DIR: " ${CMAKE_SOURCE_DIR})

add_library(angie_core STATIC "")

# Shared resources
set(HEADER_FILES
	${ANGIE_INCLUDE_DIR}/angie/core/base.hpp
	${ANGIE_INCLUDE_DIR}/angie/core/types.hpp
	${ANGIE_INCLUDE_DIR}/angie/core/config.hpp
	${ANGIE_INCLUDE_DIR}/angie/core/defines.hpp
	${ANGIE_INCLUDE_DIR}/angie/core/version.hpp
	${ANGIE_INCLUDE_DIR}/angie/core/utils.hpp
	${ANGIE_INCLUDE_DIR}/angie/core/algorithm.hpp)

set(INTERFACE_FILES
	${ANGIE_INCLUDE_DIR}/angie/core/diagnostics/assert.hpp
	${ANGIE_INCLUDE_DIR}/angie/core/diagnostics/callstack.hpp
	${ANGIE_INCLUDE_DIR}/angie/core/diagnostics/debugger.hpp
	${ANGIE_INCLUDE_DIR}/angie/core/diagnostics/hardware_break.hpp
	${ANGIE_INCLUDE_DIR}/angie/core/memory/global.hpp
	${ANGIE_INCLUDE_DIR}/angie/core/memory/manipulation.hpp
	${ANGIE_INCLUDE_DIR}/angie/core/memory/allocator.hpp
	${ANGIE_INCLUDE_DIR}/angie/core/containers/common.hpp
	${ANGIE_INCLUDE_DIR}/angie/core/containers/dynamic_array.hpp
	${ANGIE_INCLUDE_DIR}/angie/core/system/system.hpp
	${ANGIE_INCLUDE_DIR}/angie/core/system/cpu.hpp)

set(SOURCE_FILES
	memory/global.cpp
	memory/manipulation.cpp
	memory/allocator.cpp
	system/system.cpp
	system/cpu.cpp)

set(IMPLEMENTATION_FILES
	memory/impl/global_impl.hpp
	system/impl/system_impl.hpp
	system/impl/cpu_impl.hpp)

# Debug
set(SOURCE_DEBUG_FILES "")
if (angie_debug_tools)
    set(DBGTOOLS_DIR ../../externals/dbgtools)
    set(DBGTOOLS_DIR_INC ${DBGTOOLS_DIR}/include/dbgtools)
    set(DBGTOOLS_DIR_SRC ${DBGTOOLS_DIR}/src)

    target_include_directories(angie_core PRIVATE ${DBGTOOLS_DIR}/include)

	# callstack symbol
	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -rdynamic")
	elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	# using GCC
	elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
	# using Intel C++
	elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
		target_link_libraries(angie_core Dbghelp.lib)
	endif()

    set_source_files_properties(${DBGTOOLS_DIR_SRC}/assert.cpp
		PROPERTIES
		COMPILE_DEFINITIONS DBG_TOOLS_ASSERT_ENABLE
		LANGUAGE CXX)

    list(APPEND SOURCE_DEBUG_FILES
		${DBGTOOLS_DIR_INC}/assert.h
		${DBGTOOLS_DIR_SRC}/assert.cpp
		${DBGTOOLS_DIR_INC}/callstack.h
		${DBGTOOLS_DIR_SRC}/callstack.cpp
		${DBGTOOLS_DIR_INC}/debugger.h
		${DBGTOOLS_DIR_SRC}/debugger.cpp
		${DBGTOOLS_DIR_INC}/hw_breakpoint.h
		${DBGTOOLS_DIR_SRC}/hw_breakpoint.cpp)

	list(APPEND SOURCE_DEBUG_FILES
		diagnostics/callstack.cpp)
endif()

# Memory
set(SOURCE_MEMORY_FILES "")
if (angie_memory_global_ltalloc) # ltalloc
    list(APPEND SOURCE_MEMORY_FILES
            ../../externals/ltalloc/ltalloc.cc
            memory/impl/ltalloc/global_ltalloc.cpp)

    # set_source_files_properties(../../externals/ltalloc/ltalloc.cc
    #         PROPERTIES
    #         COMPILE_DEFINITIONS LTALLOC_DISABLE_OPERATOR_NEW_OVERRIDE
	# 		LANGUAGE CXX)
			
	target_compile_definitions(angie_core PRIVATE LTALLOC_DISABLE_OPERATOR_NEW_OVERRIDE)
	target_compile_definitions(angie_core PRIVATE LTALLOC_DISABLE_EXCEPTIONS)

    message(STATUS "Memory manager: ltalloc")
else() # default system alloctor
    list(APPEND SOURCE_MEMORY_FILES
            memory/impl/default/global_default.cpp)

    message(STATUS "Memory manager: system")
endif()

# System - plibsys
set(SOURCE_SYSTEM_FILES "")
if (angie_system_plibsys)
    list(APPEND SOURCE_SYSTEM_FILES
            system/impl/plibsys/system_plibsys.cpp)

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

    message(STATUS "System: plibsys")
endif()

# CPU - 
set(SOURCE_CPU_FILES "")
if (angie_cpu_libcpuid)
	list(APPEND SOURCE_CPU_FILES system/impl/libcpuid/cpu_libcpuid.cpp)

    set(LIBCPUID_DIR ../../externals/libcpuid)
	include_directories(${LIBCPUID_DIR}/libcpuid ${CMAKE_BINARY_DIR})
	message(STATUS "CPU: libcpuid")
endif()


# Add source files
target_sources(angie_core PUBLIC ${HEADER_FILES})
target_sources(angie_core PUBLIC ${INTERFACE_FILES})
target_sources(angie_core PRIVATE ${SOURCE_FILES})
target_sources(angie_core PRIVATE ${IMPLEMENTATION_FILES})

target_sources(angie_core PRIVATE ${SOURCE_DEBUG_FILES})
target_sources(angie_core PRIVATE ${SOURCE_MEMORY_FILES})
target_sources(angie_core PRIVATE ${SOURCE_SYSTEM_FILES})
target_sources(angie_core PRIVATE ${SOURCE_CPU_FILES})

set_target_properties(angie_core PROPERTIES FOLDER "angie/core")
install(TARGETS angie_core DESTINATION lib)