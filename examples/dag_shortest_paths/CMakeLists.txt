



SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/dag_shortest_paths)



add_executable(test_dag_shortest_paths test.cpp)



add_test (test_dag_shortest_paths ${EXECUTABLE_OUTPUT_PATH}/test_dag_shortest_paths)

