


MESSAGE(STATUS "before set adj: " ${EXECUTABLE_OUTPUT_PATH})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/adjacency_list)
MESSAGE(STATUS "after set adj:  " ${EXECUTABLE_OUTPUT_PATH})


add_executable(testEP testEP.cpp)
add_executable(testVP testVP.cpp)
add_executable(testAdjacencyVertex testAdjacencyVertex.cpp)
add_executable(testUndirectedGraph testUndirectedGraph.cpp)


include(CTest)
enable_testing ()

add_test (testEP ${EXECUTABLE_OUTPUT_PATH}/testEP)
add_test (testVP ${EXECUTABLE_OUTPUT_PATH}/testVP)
add_test (testAdjacencyVertex ${EXECUTABLE_OUTPUT_PATH}/testAdjacencyVertex)
add_test (testUndirectedGraph ${EXECUTABLE_OUTPUT_PATH}/testUndirectedGraph)