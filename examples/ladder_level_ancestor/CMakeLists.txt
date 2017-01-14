



SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/ladder_level_ancestor)


add_executable(test_ladder_level_ancestor test.cpp)

add_test (test_ladder_level_ancestor ${EXECUTABLE_OUTPUT_PATH}/test_ladder_level_ancestor)

