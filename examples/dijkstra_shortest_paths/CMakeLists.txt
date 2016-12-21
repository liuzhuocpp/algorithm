



SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/dijkstra_shortest_paths)


add_executable(test_dijkstra_save_mark test.cpp)


include(CTest)
enable_testing ()
add_test (test_dijkstra_save_mark ${EXECUTABLE_OUTPUT_PATH}/test_dijkstra_save_mark)

