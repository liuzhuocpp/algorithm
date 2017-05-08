



SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/multi_range_index_descriptor)


add_executable(test_multi_range_index_descriptor test.cpp)

add_test (test_multi_range_index_descriptor ${EXECUTABLE_OUTPUT_PATH}/test_multi_range_index_descriptor.exe)

