



SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/prim_minimum_spanning_tree)


add_executable(test_Prim_NoMarker  test.cpp)


add_test (test_Prim_NoMarker ${EXECUTABLE_OUTPUT_PATH}/test_Prim_NoMarker)

