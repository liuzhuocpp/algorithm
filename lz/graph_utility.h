#ifndef GRAPH_UTILITY_H
#define GRAPH_UTILITY_H
/*
 * this header file define some base classes and functions for graph
 */
#include <lz/property.h>
namespace lz {
	namespace GraphUtilityPrivate {
	}

/*
 * Indicate if a graph is directed
 */
struct DirectedGraphTag {};
struct UndirectedGraphTag {};

/*
 * Some common property tag
 */
struct VertexIndexTag {};
struct EdgeIndexTag {};


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


template<typename G>
struct GraphTraits
{
	using VertexDescriptor = typename G::VertexDescriptor;
	using EdgeDescriptor = typename G::EdgeDescriptor;

//	typedef typename G::VertexDescriptor VertexDescriptor;
};







} // namespace lz




#endif // GRAPH_UTILITY_H
