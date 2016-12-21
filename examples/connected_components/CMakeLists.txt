
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/connected_components)
add_executable(test_connected_components test.cpp)


include(CTest)
enable_testing ()
add_test (test_connected_components ${EXECUTABLE_OUTPUT_PATH}/test_connected_components)

