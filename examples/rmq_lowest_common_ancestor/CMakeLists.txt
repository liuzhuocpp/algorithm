



SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/rmq_lowest_common_ancestor)

add_executable(testRmqLca testRmqLca.cpp)
add_test (testRmqLca ${EXECUTABLE_OUTPUT_PATH}/testRmqLca.exe)

