



SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/adjacency_matrix)



add_executable(test_adjacency_matrix test.cpp)




add_test (test_adjacency_matrix ${EXECUTABLE_OUTPUT_PATH}/test_adjacency_matrix)
