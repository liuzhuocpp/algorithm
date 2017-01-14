



SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/jump_pointers_level_ancestor)


add_executable(test_jump_pointers_level_ancestor test.cpp)

add_test (test_jump_pointers_level_ancestor ${EXECUTABLE_OUTPUT_PATH}/test_jump_pointers_level_ancestor)

