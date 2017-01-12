



SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/biconnected_components_of_edge)
add_executable(test_biconnected_components_of_edge test.cpp)


add_test (test_biconnected_components_of_edge ${EXECUTABLE_OUTPUT_PATH}/test_biconnected_components_of_edge)

