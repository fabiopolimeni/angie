include_directories(externals/catch/include)

# Test files
add_executable(angie_tests_system
        angie/core/system/tests_system.cpp)
target_link_libraries(angie_tests_system angie_core)
add_test(NAME angie_tests_system COMMAND angie_tests_system)
set_target_properties(angie_tests_system PROPERTIES FOLDER
        "angie/tests/angie/core/system")