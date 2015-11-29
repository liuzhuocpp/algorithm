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

	using VertexProperties = typename G::VertexProperties;
	using EdgeProperties = typename G::EdgeProperties;

	template<typename Tag>
	using VertexPropertyMap = typename G::template VertexPropertyMap<Tag>;

	template<typename Tag>
	using EdgePropertyMap = typename G::template EdgePropertyMap<Tag>;

};







template<typename G, typename Tag>
class VertexPropertyMap: public MapFacade<typename GraphTraits<G>::VertexDescriptor,
						 	 	 	 	  decltype(typename GraphTraits<G>::VertexProperties()[Tag()])>
{
	G *g = nullptr;
public:
	VertexPropertyMap() = default;
	VertexPropertyMap(G &_g):g(&_g) { }

	using Type = VertexPropertyMap<G, Tag>;
	using ConstType = VertexPropertyMap<const G, Tag>;

	auto operator[](typename GraphTraits<G>::VertexDescriptor u) const
	{
		return g->vertexProperties(u)[Tag()];
	}
};

template<typename G, typename Tag>
class EdgePropertyMap: public MapFacade<typename GraphTraits<G>::EdgeDescriptor,
										decltype(typename GraphTraits<G>::EdgeProperties()[Tag()])>
{
	G *g = nullptr;
public :
	EdgePropertyMap() = default;
	EdgePropertyMap(G &_g):g(&_g) { }

	using Type = EdgePropertyMap<G, Tag>;
	using ConstType = EdgePropertyMap<const G, Tag>;

	auto operator[](typename GraphTraits<G>::EdgeDescriptor e) const
	{
		return g->edgeProperties(e)[Tag()];
	}
};

template<typename G, typename Tag>
auto makeVertexPropertyMap(G &g, Tag)
{
	return typename VertexPropertyMap<G, Tag>::Type(g);
}

template<typename G, typename Tag>
auto makeVertexPropertyMap(const G &g, Tag tag)
{
	return typename VertexPropertyMap<G, Tag>::ConstType(g);
}

template<typename G, typename Tag>
auto makeEdgePropertyMap( G &g, Tag tag)
{
	return typename EdgePropertyMap<G, Tag>::Type(g);
}

template<typename G, typename Tag>
auto makeEdgePropertyMap(const G &g, Tag tag)
{
	return typename EdgePropertyMap<G, Tag>::ConstType(g);
}










// for graph param
template<typename G, typename VertexIndexMapName>
using ChooseVertexIndexMap = ChooseParamReturnType<MemberFunctionReturnType<VertexIndexMapName>,
		                                           typename GraphTraits<G>::template VertexPropertyMap<VertexIndexTag>::ConstType
												   >;


template<typename GeneralParamName, typename VertexIndexMap, typename ValueType>
using ChooseVertexIndexComposeMap = ChooseParamReturnType<
		MemberFunctionReturnType<GeneralParamName>,
		ComposeMap<VertexIndexMap, IteratorMap<ValueType*> >
>;


template<typename DefaultValueType, typename ParamReturnType, typename VertexIndexMap, typename VertexNumberType>
auto
chooseVertexIndexComposeMap(ParamReturnType paramMap, VertexIndexMap indexMap, VertexNumberType n)
//->decltype(paramMap)
{
	return paramMap;
}

template<typename DefaultValueType, typename VertexIndexMap, typename VertexNumberType>
auto
chooseVertexIndexComposeMap( ParamNotFound, VertexIndexMap indexMap, VertexNumberType n)
//->decltype(makeComposeMap(indexMap, makeIteratorMap(new DefaultValueType[n])))
{
	return makeComposeMap(indexMap, makeIteratorMap(
									new DefaultValueType[n]
								 ));
}

template<typename Map, typename ParamRetrunType>
void deleteVertexIndexComposeMap(Map map, ParamRetrunType ) { }

template<typename Map>
void deleteVertexIndexComposeMap(Map map, ParamNotFound )
{
	delete[] map.secondMap().iterator();
}


} // namespace lz




#endif // GRAPH_UTILITY_H

