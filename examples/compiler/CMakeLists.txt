
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/compiler)
add_executable(compiler_test_grammar test_grammar.cpp)



add_test (compiler_test_grammar ${EXECUTABLE_OUTPUT_PATH}/compiler_test_grammar)

