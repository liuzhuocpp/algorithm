
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/compiler)
add_executable(compiler_test_slr1_grammar test_slr1_grammar.cpp)
add_executable(compiler_test_ll1_grammar test_ll1_grammar.cpp)



add_test (compiler_test_slr1_grammar ${EXECUTABLE_OUTPUT_PATH}/compiler_test_slr1_grammar)
add_test (compiler_test_ll1_grammar ${EXECUTABLE_OUTPUT_PATH}/compiler_test_ll1_grammar)