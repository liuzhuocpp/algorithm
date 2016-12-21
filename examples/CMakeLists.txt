cmake_minimum_required(VERSION 3.7)


set(CMAKE_CXX_FLAGS "  -target x86_64-pc-windows-gnu -std=c++14 -ID:\\code\\github\\algorithm -ID:\\code\\lib\\boost_1_58_0")  
set(CMAKE_CXX_COMPILER "clang++") 

project (Exmaple)

MESSAGE(STATUS "This is  source dir " ${PROJECT_SOURCE_DIR})
MESSAGE(STATUS "This is  binary dir " ${PROJECT_BINARY_DIR})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include_directories("D:\\code\\github\\algorithm")


add_subdirectory(adjacency_list)
add_subdirectory(depth_first_search)
add_subdirectory(heavy_path_decomposition)



#add_executable(hello main.cpp)
#add_executable(hello2 main2.cpp)



include(CTest)
enable_testing ()

#add_test (MyTest ${EXECUTABLE_OUTPUT_PATH}/hello)
#add_test (MyTest2 ${EXECUTABLE_OUTPUT_PATH}/hello2)
