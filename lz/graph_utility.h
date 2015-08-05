#ifndef GRAPH_UTILITY_H
#define GRAPH_UTILITY_H


namespace lz {

/*
 * this header file define some base classes and functions for graph
 */
	namespace GraphUtilityPrivate {
	}

struct Directed {};
struct Undirected {};


enum class Color: unsigned char
{
    White,
    Gray,
    Black,
};

template<typename Graph>
typename Graph::VertexDescriptor 
opposite(const Graph &g, 
         typename Graph::EdgeDescriptor e,
         typename Graph::VertexDescriptor u)
{
    if(g.source(e) != u) return g.source(e);
    return g.target(e);
//    return g.source(e) != u ? g.source(e) : g.target(e);
    
}



}




#endif // GRAPH_UTILITY_H
