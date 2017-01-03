



SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/lowest_common_ancestor)


add_executable(testLca testLca.cpp)


include(CTest)
enable_testing ()
add_test (testLca ${EXECUTABLE_OUTPUT_PATH}/testLca)

