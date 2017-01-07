



SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/sparse_table_range_minimum_query)

add_executable(testSTRmq testSTRmq.cpp)
add_test (testSTRmq ${EXECUTABLE_OUTPUT_PATH}/testSTRmq.exe)

