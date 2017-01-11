



SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/biconnected_components_of_vertex)
add_executable(test_biconnected_components_of_vertex test.cpp)


add_test (test_biconnected_components_of_vertex ${EXECUTABLE_OUTPUT_PATH}/test_biconnected_components_of_vertex)

