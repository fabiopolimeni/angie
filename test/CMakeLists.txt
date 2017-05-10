include_directories(externals/catch/include)

if(MINGW)
    add_definitions(-DMINGW_HAS_SECURE_API=1)
endif()

# Memory tests
add_executable(angie_memory_tests
        angie/core/memory/memory_tests.cpp)
target_link_libraries(angie_memory_tests angie_core)
add_test(NAME angie_memory_test COMMAND angie_memory_test)
set_target_properties(angie_memory_tests PROPERTIES FOLDER
        "angie/core/memory")

# System tests
add_executable(angie_system_tests
        angie/core/system/system_tests.cpp)
target_link_libraries(angie_system_tests angie_core)
add_test(NAME angie_system_tests COMMAND angie_system_tests)
set_target_properties(angie_system_tests PROPERTIES FOLDER
        "angie/core/system")

# Array tests
add_executable(angie_array_tests
        angie/core/containers/array_tests.cpp)
target_link_libraries(angie_array_tests angie_core)
add_test(NAME angie_array_tests COMMAND angie_array_tests)
set_target_properties(angie_array_tests PROPERTIES FOLDER
        "angie/core/containers")

# CPU tests
add_executable(angie_cpu_tests
        angie/core/system/cpu_tests.cpp)
target_link_libraries(angie_cpu_tests angie_core)
add_test(NAME angie_cpu_tests COMMAND angie_cpu_tests)
set_target_properties(angie_cpu_tests PROPERTIES FOLDER
        "angie/core/system")