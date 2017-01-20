



SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/big_integer)


add_executable(big_integer_test_basic test_basic.cpp)
add_executable(big_integer_test_vector test_vector.cpp)
add_executable(big_integer_test_binary test_binary.cpp)
add_executable(big_integer_test_unsigned_big_integer test_unsigned_big_integer.cpp)

add_test (big_integer_test_basic ${EXECUTABLE_OUTPUT_PATH}/big_integer_test_basic)
add_test (big_integer_test_vector ${EXECUTABLE_OUTPUT_PATH}/big_integer_test_vector)
add_test (big_integer_test_binary ${EXECUTABLE_OUTPUT_PATH}/big_integer_test_binary)
add_test (big_integer_test_unsigned_big_integer ${EXECUTABLE_OUTPUT_PATH}/big_integer_test_unsigned_big_integer)

