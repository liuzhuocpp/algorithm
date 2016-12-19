


MESSAGE(STATUS "before set dfs: " ${EXECUTABLE_OUTPUT_PATH})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/depth_first_search)
MESSAGE(STATUS "after set dfs:  " ${EXECUTABLE_OUTPUT_PATH})


add_executable(testUndirectedDFS testUndirectedDFS.cpp)


include(CTest)
enable_testing ()

add_test (testUndirectedDFS ${EXECUTABLE_OUTPUT_PATH}/testUndirectedDFS)

