



SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/iterator_range)


add_executable(test_iterator_range test.cpp)

add_test (test_iterator_range ${EXECUTABLE_OUTPUT_PATH}/test_iterator_range)

