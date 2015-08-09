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
struct Directed {};
struct Undirected {};

/*
 * Some common property tag
 */
struct VertexIndexTag {};
struct EdgeIndexTag {};

/*
 * Map attached to vertex and edge for a graph
 */
template<typename Graph, typename Iterator, typename Tag>
struct VertexMap
{
	Iterator first;
	auto operator[](typename Graph::VertexDescriptor i) -> decltype(get<Tag>(first[i]))
	{
		return get<Tag>(first[i]);
	}
};









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















} // namespace lz




#endif // GRAPH_UTILITY_H
