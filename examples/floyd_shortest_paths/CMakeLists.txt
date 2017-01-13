



SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/floyd_shortest_paths)
add_executable(test_floyd_shortest_paths test.cpp)


add_test (test_floyd_shortest_paths ${EXECUTABLE_OUTPUT_PATH}/test_floyd_shortest_paths)

