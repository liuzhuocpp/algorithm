#ifndef GRAPH_UTILITY_H
#define GRAPH_UTILITY_H
/*
 * this header file define some base classes and functions for graph
 */
//#include "lz/property.h"

#include "lz/utility.h"
#include "lz/map.h"
namespace lz {
	namespace GraphUtilityPrivate {
	}

/*
 * Indicate if a graph is directed
 */
struct DirectedGraphTag {};
struct UndirectedGraphTag {};

/*
 * Some common graph`s vertex or edge property tag
 */
struct VertexIndexTag {};
struct EdgeIndexTag {};





template<typename Graph>
typename Graph::VertexDescriptor
opposite(const Graph &g, 
         typename Graph::EdgeDescriptor e,
         typename Graph::VertexDescriptor u)
{
    if(g.source(e) != u) return g.source(e);
    return g.target(e);
}



template<typename G>
struct GraphTraits
{
	using VertexDescriptor = typename G::VertexDescriptor;
	using EdgeDescriptor = typename G::EdgeDescriptor;

	using VertexIterator = typename G::VertexIterator;
	using EdgeIterator = typename G::EdgeIterator;

	using DirectedCategory = typename G::DirectedCategory;

	using OutEdgeIterator = typename G::OutEdgeIterator;


	template<typename Tag>
	using VertexPropertyMap = typename G::template VertexPropertyMap<Tag>;

	template<typename Tag>
	using EdgePropertyMap = typename G::template EdgePropertyMap<Tag>;

};







// for graph param
template<typename G, typename VertexIndexMapName>
using ChooseVertexIndexMap = ChooseParamReturnType<GeneralParamReturnType<VertexIndexMapName>,
												   decltype( G().vertexPropertyMap(VertexIndexTag()))  >;


template<typename GeneralParamName, typename VertexIndexMap, typename ValueType>
using ChooseVertexIndexComposeMap = ChooseParamReturnType<
		GeneralParamReturnType<GeneralParamName>,
		ComposeMap<VertexIndexMap, IteratorMap<ValueType*> >
>;


template<typename DefaultValueType, typename ParamReturnType, typename VertexIndexMap, typename VertexNumberType>
auto
chooseVertexIndexComposeMap(ParamReturnType paramMap, VertexIndexMap indexMap, VertexNumberType n)
->decltype(paramMap)
{
	return paramMap;
}

template<typename DefaultValueType, typename VertexIndexMap, typename VertexNumberType>
auto
chooseVertexIndexComposeMap( ParamNotFound, VertexIndexMap indexMap, VertexNumberType n)
->decltype(makeComposeMap(indexMap, makeIteratorMap(new DefaultValueType[n])))
{
	return makeComposeMap(indexMap, makeIteratorMap(new DefaultValueType[n]));
}

template<typename Map, typename ParamRetrunType>
void deleteVertexIndexComposeMap(Map map, ParamRetrunType )
{

}
template<typename Map>
void deleteVertexIndexComposeMap(Map map, ParamNotFound )
{
//	cout << "YEEE" << endl;
	delete[] map.secondMap().iterator();
}



//template<typename ParamColorMap>
//void deleteColorMap(ParamColorMap) {}
//
//void deleteColorMap(DefaultColorMap)
//{
//	delete[] colorMap.secondMap().iterator();
//}





} // namespace lz




#endif // GRAPH_UTILITY_H

