



SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/strongly_connected_components)
add_executable(test_strongly_components test.cpp)


include(CTest)
enable_testing ()
add_test (test_strongly_components ${EXECUTABLE_OUTPUT_PATH}/test_strongly_components)

