



SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/lowest_common_ancestor)


add_executable(testLca testLca.cpp)

add_test (testLca ${EXECUTABLE_OUTPUT_PATH}/testLca.exe)

