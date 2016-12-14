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

 Graph concept classification:

  Graph :
    GraphTraits<G>::VertexDescriptor
    GraphTraits<G>::Edgedescriptor
    GraphTraits<G>::DirectedCategory
    GraphTraits<G>::nullVertex(): VertexDescriptor
    GraphTraits<G>::nullEdge(): Edgedescriptor


  IncidenceGraph refines Graph:
    GraphTraits<G>::outEdgeIterator
    g.outEdges(u): pair<outEdgeIterator,outEdgeIterator>    outEdges(g, u)
    g.source(e): VertexDescriptor  source(g, e)
    g.target(e): VertexDescriptor  target(g, e)

  VertexListGraph refines Graph
    GraphTraits<G>::VertexIterator
    GraphTraits<G>::VerticesNumberType
    g.vertices(): pair<VertexIterator, VertexIterator>  vertices(g)
    g.verticesNumber():VerticesNumberType   vertiecsNumber(g)

  EdgeListGraph refines Graph
    GraphTraits<G>::EdgeIterator
    GraphTraits<G>::EdgesNumberType
    g.edges(): pair<EdgeIterator, EdgeIterator>  edges(g)
    g.edgesNumber(): EdgesNumberType    edgesNumber(g)
    g.source(e): VertexDescriptor   source(g, e)
    g.target(e): VertexDescriptor   target(g, e)

  AdjacencyMatrix refines Graph:
    g.edge(u, v): EdgeDescriptor    edge(g, u, v)

  PropertyGraph refines Graph:
    template<Tag> GraphTraits<G>::VertexPropertyMap
    template<Tag> GraphTraits<G>::ConstVertexPropertyMap
    template<Tag> GraphTraits<G>::EdgePropertyMap
    template<Tag> GraphTraits<G>::ConstEdgePropertyMap
    g.vertexPropertyMap(tag): VertexPropertyMap<Tag> or ConstVertexPropertyMap<Tag>
    g.edgePropertyMap(tag): EdgePropertyMap<Tag> or ConstEdgePropertyMap<Tag>





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
	using DirectedCategory = typename G::DirectedCategory;

	using VertexIterator = typename G::VertexIterator;
	using EdgeIterator = typename G::EdgeIterator;
	using OutEdgeIterator = typename G::OutEdgeIterator;

	using VerticesNumberType = typename G::VerticesNumberType;
	using EdgesNumberType = typename G::EdgesNumberType;

	template<typename Tag>
	using VertexPropertyMap = typename G::template VertexPropertyMap<Tag>;
	template<typename Tag>
	using EdgePropertyMap = typename G::template EdgePropertyMap<Tag>;

	static VertexDescriptor nullVertex() { return G::nullVertex(); }
	static VertexDescriptor nullEdge() { return G::nullEdge(); }




//	template<typename Tag>
//	using ConstVertexPropertyMap;

//	template<typename Tag>
//    using ConstEdgePropertyMap;


    using VertexProperties = typename G::VertexProperties; // deprected
    using EdgeProperties = typename G::EdgeProperties;// deprected

};

template<typename G, typename Tag, typename Descriptor, typename GetProperties, GetProperties getProperties>
class VertexOrEdgePropertyMap
{
    using _ValueType = std::decay_t<decltype((G().*getProperties)[Tag()])>;
protected:
    G *g = nullptr;
public:
    using KeyType = Descriptor;
    using ValueType = _ValueType;

    VertexOrEdgePropertyMap() = default;
    VertexOrEdgePropertyMap(G &_g):g(&_g) { }
    auto operator[](KeyType u) const->decltype((g->*getProperties)(u)[Tag()])
    {
        return (g->*getProperties)(u)[Tag()];
    }
//    using Type = VertexOrEdgePropertyMap;
//    struct ConstType
//    {
//        using KeyType = Descriptor;
//        using ValueType = _ValueType;
//        const G *g;
//        ConstType():g(nullptr){}
//        ConstType(const G &g):g(&g){}
//        const ValueType& operator[](KeyType u) const
//        {
//            return (g->*getProperties)(u)[Tag()];
//        }
//    };

};

//template<typename G, typename Tag>
//struct VertexPropertyMap
////public
////    VertexOrEdgePropertyMap<G, Tag, typename G::VertexDescriptor, decltype(&G::vertexProperties), &G::vertexProperties>
//{
////    static constexpr auto getProperties()
////    {
////        return (typename G::VertexProperties& (G::*) (typename G::VertexDescriptor) ) &G::vertexProperties;
////    }
//    using VertexProperties = typename G::VertexProperties;
//    using VertexDescriptor = typename G::VertexDescriptor;
//
//    using Type = VertexOrEdgePropertyMap<G, Tag, typename G::VertexDescriptor,
//            decltype(
//            (VertexProperties& (G::*) (VertexDescriptor)   ) &G::vertexProperties   )
//            ,
//            (VertexProperties& (G::*) (VertexDescriptor)   ) &G::vertexProperties
//            >;
//
//    using ConstType = Type;
//
//
////    using ConstType = VertexOrEdgePropertyMap<G, Tag, typename G::VertexDescriptor,
////                decltype( (typename G::VertexProperties (G::*)(typename G::VertexDescriptor) )&G::vertexProperties   )
////                ,
////                (typename G::VertexProperties (G::*)(typename G::VertexDescriptor) )&G::vertexProperties
////                >;
//};

//template<typename G, typename Tag>
//struct EdgePropertyMap: public VertexOrEdgePropertyMap<G, Tag,
//    typename G::EdgeDescriptor, decltype(&G::edgeProperties), &G::edgeProperties>
//{
//
//};





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
	using _ValueType = typename std::decay<decltype(typename GraphTraits<G>::EdgeProperties()[Tag()])>::type;
	G *g = nullptr;
public:
	using KeyType = typename GraphTraits<G>::EdgeDescriptor;
	using ValueType = _ValueType;

	EdgePropertyMap() = default;
	EdgePropertyMap(G &_g):g(&_g) { }
	ValueType& operator[](KeyType u) const
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
		using ValueType = _ValueType;

		ConstType():g(nullptr){}
		ConstType(const G &g):g(&g){}
		ValueType operator[](KeyType u) const
		{
			return g->edgeProperties(u)[Tag()];
		}
	};
};




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




template<typename ValueType, typename VertexIndexMap, typename VertexNumberType>
auto
makeVertexIndexComposeMap(VertexIndexMap indexMap, VertexNumberType n, const ValueType &val = ValueType())
    ->decltype(makeComposeMap(indexMap, SharedArrayMap<ValueType>(n, val)) )

{
	return makeComposeMap(indexMap, SharedArrayMap<ValueType>(n, val) );
}


template<typename ValueType, typename VertexIndexMap, typename VertexNumberType>
auto
calculateVertexIndexComposeMap(VertexIndexMap indexMap, VertexNumberType n, const ValueType &val = ValueType())
    ->decltype(std::bind(makeVertexIndexComposeMap<ValueType, VertexIndexMap, VertexNumberType>, indexMap, n, val))
{
    return std::bind(makeVertexIndexComposeMap<ValueType, VertexIndexMap, VertexNumberType>, indexMap, n, val);
}





} // namespace lz




#endif // GRAPH_UTILITY_H

