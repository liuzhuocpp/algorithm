



SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/heavy_path_decomposition)


add_executable(testHeavyPath test.cpp)


include(CTest)
enable_testing ()
add_test (testHeavyPath ${EXECUTABLE_OUTPUT_PATH}/testHeavyPath)

