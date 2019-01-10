# Detect various platforms examples
add_executable(angie_platform_example platform/platform_main.cpp)
target_link_libraries(angie_platform_example angie_core)
set_target_properties(angie_platform_example
    PROPERTIES FOLDER "platform")
