



SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/big_integer)


add_executable(test_big_integer_simple_core test_simple_core.cpp)
add_executable(test_big_integer_vector_core test_vector_core.cpp)

add_test (test_big_integer_simple_core ${EXECUTABLE_OUTPUT_PATH}/test_big_integer_simple_core)
add_test (test_big_integer_vector_core ${EXECUTABLE_OUTPUT_PATH}/test_big_integer_vector_core)

