cmake_minimum_required(VERSION 3.7)


#set(CMAKE_CXX_FLAGS "  -target x86_64-pc-windows-gnu -std=c++1z -ID:\\code\\github\\algorithm -ID:\\code\\lib\\boost_1_58_0")  
set(CMAKE_CXX_FLAGS "  -target x86_64-pc-windows-gnu -std=c++1z -I" ${PROJECT_SOURCE_DIR}\\.. )
set(CMAKE_CXX_COMPILER "clang++") 

project (Exmaple)

MESSAGE(STATUS "This is  source dir " ${PROJECT_SOURCE_DIR})
MESSAGE(STATUS "This is  binary dir " ${PROJECT_BINARY_DIR})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

#include_directories("D:\\code\\github\\algorithm")
include_directories(${PROJECT_SOURCE_DIR}\\..)


include(CTest)
enable_testing ()

add_subdirectory(adjacency_list)
add_subdirectory(depth_first_search)
add_subdirectory(heavy_path_decomposition)
add_subdirectory(prim_minimum_spanning_tree)
add_subdirectory(dijkstra_shortest_paths)
add_subdirectory(strongly_connected_components)
add_subdirectory(connected_components)
add_subdirectory(lowest_common_ancestor)
add_subdirectory(sparse_table_range_minimum_query)
add_subdirectory(rmq_lowest_common_ancestor)
add_subdirectory(biconnected_components_of_vertex)
add_subdirectory(biconnected_components_of_edge)
add_subdirectory(jump_pointers_lowest_common_ancestor)
add_subdirectory(iterator_range)
add_subdirectory(adjacency_matrix)
add_subdirectory(floyd_shortest_paths)
add_subdirectory(dag_shortest_paths)
add_subdirectory(bellman_ford_shortest_paths)
add_subdirectory(jump_pointers_level_ancestor)
add_subdirectory(ladder_level_ancestor)
add_subdirectory(big_integer)
add_subdirectory(compiler)
add_subdirectory(multi_range_index_descriptor)











