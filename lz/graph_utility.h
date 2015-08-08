#ifndef GRAPH_UTILITY_H
#define GRAPH_UTILITY_H
/*
 * this header file define some base classes and functions for graph
 */

namespace lz {
	namespace GraphUtilityPrivate {
	}



struct Directed {};
struct Undirected {};
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















} // namespace lz




#endif // GRAPH_UTILITY_H
