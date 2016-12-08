#ifndef GRAPH_UTILITY_H
#define GRAPH_UTILITY_H
/*
 * this header file define some base classes and functions for graph
 */
//#include "lz/property.h"

//#include "lz/utility.h"
#include "lz/map.h"
#include <memory>
//#include "lz/adjacency_list.h"

namespace lz {
	namespace GraphUtilityPrivate {
	}

/*
 * Graph concept classification:
 *
 * Graph :
 *   GraphTraits<G>::VertexDescriptor
 *   GraphTraits<G>::Edgedescriptor
 *   GraphTraits<G>::DirectedCategory
 *
 * IncidenceGraph refines Graph:
 *   GraphTraits<G>::outEdgeIterator
 *   g.outEdges(u)
 *   g.source(e)
 *   g.target(e)
 *
 * VertexListGraph refines Graph
 *   GraphTraits<G>::VertexIterator
 *   GraphTraits<G>::VerticesNumberType
 *   g.vertices() :pair<VertexIterator, VertexIterator>
 *   g.verticesNumber()
 *
 * EdgeListGraph refines Graph
 *   GraphTraits<G>::EdgeIterator
 *   GraphTraits<G>::EdgesNumberType
 *   g.edges() :pair<EdgeIterator, EdgeIterator>
 *   g.edgesNumber() :EdgesNumberType
 *   g.source(e) :VertexDescriptor
 *   g.target(e) :VertexDescriptor
 *
 * AdjacencyMatrix refines Graph:
 *   g.edge(u, v) :EdgeDescriptor
 *
 * PropertyGraph refines Graph:
 *   GraphTraits<G>::VertexPropertyMap
 *   GraphTraits<G>::EdgePropertyMap
 *   g.vertexPropertyMap(tag)
 *   g.edgePropertyMap(tag)
 *
 *
 */



/*
 * Indicate if a graph is directed
 */
struct DirectedGraphTag {};
struct UndirectedGraphTag {};

//struct AcyclicGraphTag {};
//
//struct DirectedAcyclicGraphTag: DirectedGraphTag, AcyclicGraphTag {};
//struct UndirectedAcyclicGraphTag: UndirectedGraphTag, AcyclicGraphTag {};
//using ForestTag = UndirectedAcyclicGraphTag;

/*
 * Some common graph's vertex or edge property tag
 */
struct VertexIndexTag { } vertexIndexTag;
struct EdgeIndexTag { } edgeIndexTag;
struct EdgeWeightTag { } edgeWeightTag;





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

	using VerticesNumberType = typename G::VerticesNumberType;
	using EdgesNumberType = typename G::EdgesNumberType;

	template<typename Tag>
	using VertexPropertyMap = typename G::template VertexPropertyMap<Tag>;

	template<typename Tag>
	using EdgePropertyMap = typename G::template EdgePropertyMap<Tag>;


    using VertexProperties = typename G::VertexProperties; // deprected
    using EdgeProperties = typename G::EdgeProperties;// deprected


};







template<typename G, typename Tag>
class VertexPropertyMap
{
//	using _ValueType = std::decay_t<decltype(typename GraphTraits<G>::VertexProperties()[Tag()])>;

	using _ValueType = typename std::decay<decltype(typename GraphTraits<G>::VertexProperties()[Tag()])>::type;
	G *g = nullptr;
public:
	using KeyType = typename GraphTraits<G>::VertexDescriptor;
	using ValueType = _ValueType;

	VertexPropertyMap() = default;
	VertexPropertyMap(G &_g):g(&_g) { }
	auto operator[](KeyType u) const->decltype(g->vertexProperties(u)[Tag()])
	{
		return g->vertexProperties(u)[Tag()];
	}

	using Type = VertexPropertyMap<G, Tag>;

	struct ConstType
	{
		using KeyType = typename GraphTraits<G>::VertexDescriptor;
		using ValueType = _ValueType;
		const G *g;
		ConstType():g(nullptr){}
		ConstType(const G &g):g(&g){}
		auto operator[](KeyType u) const->decltype(g->vertexProperties(u)[Tag()])
		{
			return g->vertexProperties(u)[Tag()];
		}
	};
};


template<typename G, typename Tag>
class EdgePropertyMap
{
//	using _ValueType = typename std::decay<decltype(typename GraphTraits<G>::EdgeProperties()[Tag()])>::type;
	using _ValueType = typename std::decay<decltype(typename GraphTraits<G>::EdgeProperties()[Tag()])>::type;
	G *g = nullptr;
public:
	using KeyType = typename GraphTraits<G>::EdgeDescriptor;
	using ValueType = _ValueType;

	EdgePropertyMap() = default;
	EdgePropertyMap(G &_g):g(&_g) { }
	ValueType operator[](KeyType u) const
	{
		return g->edgeProperties(u)[Tag()];
	}

	using Type = EdgePropertyMap<G, Tag>;

	struct ConstType
	{
	private:
		const G *g;
	public:
		using KeyType = typename GraphTraits<G>::EdgeDescriptor;
		using ValueType = const _ValueType &;

		ConstType():g(nullptr){}
		ConstType(const G &g):g(&g){}
		ValueType operator[](KeyType u) const
		{
			return g->edgeProperties(u)[Tag()];
		}
	};
};


//template<typename G, typename Tag>
//class EdgePropertyMap: public MapFacade<typename GraphTraits<G>::EdgeDescriptor,
//										decltype(typename GraphTraits<G>::EdgeProperties()[Tag()])>
//{
//	G *g = nullptr;
//public :
//	EdgePropertyMap() = default;
//	EdgePropertyMap(G &_g):g(&_g) { }
//
//	using Type = EdgePropertyMap<G, Tag>;
//	using ConstType = EdgePropertyMap<const G, Tag>;
//
//	auto operator[](typename GraphTraits<G>::EdgeDescriptor e) const
//	{
//		return g->edgeProperties(e)[Tag()];
//	}
//};



template<typename G, typename Tag>
typename VertexPropertyMap<G, Tag>::Type
makeVertexPropertyMap(G &g, Tag)
{
	return typename VertexPropertyMap<G, Tag>::Type(g);
}

template<typename G, typename Tag>
typename VertexPropertyMap<G, Tag>::ConstType
makeVertexPropertyMap(const G &g, Tag tag)

{
	return typename VertexPropertyMap<G, Tag>::ConstType(g);
}

template<typename G, typename Tag>
auto makeEdgePropertyMap( G &g, Tag tag)
->decltype(typename EdgePropertyMap<G, Tag>::Type(g))
{
	return typename EdgePropertyMap<G, Tag>::Type(g);
}

template<typename G, typename Tag>
auto makeEdgePropertyMap(const G &g, Tag tag)
->decltype(typename EdgePropertyMap<G, Tag>::ConstType(g))
{
	return typename EdgePropertyMap<G, Tag>::ConstType(g);
}







//
//auto lazyDiscoverTimeMap = [&]() {
//		auto d_array = std::unique_ptr<std::size_t[]>(new std::size_t[g.vertexNumber()]);
//
//		return makeComposeMap(_vertexIndexMap,
//							  std::move(makeUniqueArrayMap( d_array )    ) );
//	};

//template<typename ValueType, typename VertexIndexMap, typename VertexNumberType>
//auto
//makeVertexIndexComposeMap(VertexIndexMap indexMap, VertexNumberType n)
//->decltype(makeComposeMap(indexMap,
//		  std::move(makeUniqueArrayMap(std::unique_ptr<ValueType[]>(new ValueType[n])))	))
//
//{
////	auto value_array = ;
//	return makeComposeMap(indexMap,
//						  std::move(makeUniqueArrayMap(std::unique_ptr<ValueType[]>(new ValueType[n])))	);
//}


template<typename ValueType, typename VertexIndexMap, typename VertexNumberType>
auto
makeVertexIndexComposeMap(VertexIndexMap indexMap, VertexNumberType n)
->decltype(makeComposeMap(indexMap, SharedArrayMap<ValueType>(n)) )

{

	return makeComposeMap(indexMap, SharedArrayMap<ValueType>(n) );
}




//// for graph param
//template<typename G, typename VertexIndexMapName>
//using ChooseVertexIndexMap = ChooseParamReturnType<MemberFunctionReturnType<VertexIndexMapName>,
//		                                           typename GraphTraits<G>::template VertexPropertyMap<VertexIndexTag>::ConstType
//												   >;
//
//
//template<typename GeneralParamName, typename VertexIndexMap, typename ValueType>
//using ChooseVertexIndexComposeMap = ChooseParamReturnType<
//		MemberFunctionReturnType<GeneralParamName>,
//		ComposeMap<VertexIndexMap, IteratorMap<ValueType*> >
//>;
//
//
//template<typename DefaultValueType, typename ParamReturnType, typename VertexIndexMap, typename VertexNumberType>
//auto
//chooseVertexIndexComposeMap(ParamReturnType paramMap, VertexIndexMap indexMap, VertexNumberType n)
////->decltype(paramMap)
//{
//	return paramMap;
//}
//
//template<typename DefaultValueType, typename VertexIndexMap, typename VertexNumberType>
//auto
//chooseVertexIndexComposeMap( ParamNotFound, VertexIndexMap indexMap, VertexNumberType n)
////->decltype(makeComposeMap(indexMap, makeIteratorMap(new DefaultValueType[n])))
//{
//	return makeComposeMap(indexMap, makeIteratorMap(
//									new DefaultValueType[n]
//								 ));
//}
//
//template<typename Map, typename ParamRetrunType>
//void deleteVertexIndexComposeMap(Map map, ParamRetrunType ) { }
//
//template<typename Map>
//void deleteVertexIndexComposeMap(Map map, ParamNotFound )
//{
//	delete[] map.secondMap().iterator();
//}


} // namespace lz




#endif // GRAPH_UTILITY_H

