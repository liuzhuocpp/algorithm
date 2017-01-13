



SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/heavy_path_decomposition)


add_executable(test_heavy_path_decomposition test.cpp)

add_test (test_heavy_path_decomposition ${EXECUTABLE_OUTPUT_PATH}/test_heavy_path_decomposition)

