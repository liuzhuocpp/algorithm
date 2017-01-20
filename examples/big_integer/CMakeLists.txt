



SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/big_integer)


add_executable(test_big_integer_basic test_basic.cpp)
add_executable(test_big_integer_vector test_vector.cpp)
add_executable(test_big_integer_binary test_binary.cpp)

add_test (test_big_integer_basic ${EXECUTABLE_OUTPUT_PATH}/test_big_integer_basic)
add_test (test_big_integer_vector ${EXECUTABLE_OUTPUT_PATH}/test_big_integer_vector)
add_test (test_big_integer_binary ${EXECUTABLE_OUTPUT_PATH}/test_big_integer_binary)

