#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H


#include <lz/iterator.h>
#include <vector>
#include <tuple>
#include <algorithm>
#include "lz/property.h"

#include "lz/graph_utility.h"
#include "lz/map.h"


namespace lz {


using std::vector;
using std::cout;
using std::endl;
using std::pair;
using std::make_pair;

template<typename DirectedCategory = DirectedGraphTag,
         typename VP = NoProperty,
         typename EP = NoProperty,
         typename GP = NoProperty>
class AdjacencyList;

    namespace AdjacencyListPrivate {

    using VertexDescriptor = int;
    using EdgeDescriptor = int;
    using SizeType = int;



    template<typename G>
    class OutEdgeIterator: public IteratorFacade<
        OutEdgeIterator<G>,
        std::forward_iterator_tag,
        typename GraphTraits<G>::EdgeDescriptor,
        std::ptrdiff_t,
        typename GraphTraits<G>::EdgeDescriptor*,
        typename GraphTraits<G>::EdgeDescriptor >
    {
        friend G;
        EdgeDescriptor i; // realED
        const G *g;
    protected:
        OutEdgeIterator(EdgeDescriptor i, const G *g): i(i), g(g) {} //AdjacencyList call this function

    public:
        OutEdgeIterator():i(-1), g(nullptr) {}

        OutEdgeIterator& operator++()
        {
            i = g->next(i);
            return *this;
        }
        typename OutEdgeIterator::reference operator*() const
        {
            return G::R2V(i);
        }
        bool operator==(OutEdgeIterator const& o) const
        {
            return i == o.i && g == o.g;
        }
    };


    template<typename G>
    class AdjacencyVertexIterator: public IteratorFacade<
        AdjacencyVertexIterator<G>,
        std::forward_iterator_tag,
        VertexDescriptor,
        std::ptrdiff_t,
        VertexDescriptor*,
        VertexDescriptor >
    {
        friend G;
        VertexDescriptor source;
        EdgeDescriptor i; // realED
        const G *g;
    protected:
        AdjacencyVertexIterator(VertexDescriptor source, EdgeDescriptor i, const G *g):
            source(source), i(i), g(g) {} //AdjacencyList call this function

    public:
        AdjacencyVertexIterator():source(-1), i(-1), g(nullptr) {}

        AdjacencyVertexIterator& operator++()
        {
            i = g->next(i);
            return *this;
        }
        typename AdjacencyVertexIterator::reference operator*() const
        {
            return opposite(*g, G::R2V(i), source);
        }
        bool operator==(AdjacencyVertexIterator const& o) const
        {
            return i == o.i && g == o.g;
        }
    };




    template<typename P>
    struct HasPropertiesBase
    {
        P properties;
        HasPropertiesBase(const P &p = P()):properties(p) {}
    };
    template<> struct HasPropertiesBase<NoProperty>
    {
        HasPropertiesBase(const NoProperty &p = NoProperty()) {}
    };

    // vertex data
    template<typename VP>
    struct VertexData: public HasPropertiesBase<VP>
	{
    	EdgeDescriptor head;
    	VertexData(EdgeDescriptor _head, const VP &vp):
    	    head(_head), HasPropertiesBase<VP>(vp) {}
	};

    // edge data
    template<typename EP>
    struct EdgeDataBase: HasPropertiesBase<EP>
    {
        VertexDescriptor source, target;
        EdgeDataBase(VertexDescriptor source, VertexDescriptor target, const EP &ep):
            source(source), target(target), HasPropertiesBase<EP>(ep) {}
    };

    template<typename DirectedCategory, typename EP>
    struct EdgeData: EdgeDataBase<EP>
    {
        EdgeDescriptor next;
        EdgeData(VertexDescriptor source, VertexDescriptor target, const EP &ep, EdgeDescriptor next):
            EdgeDataBase<EP>(source, target, ep), next(next) {}
    };

    template<typename EP>
    struct EdgeData<UndirectedGraphTag, EP>: EdgeDataBase<EP>
    {
        EdgeDescriptor next[2];
        EdgeData(VertexDescriptor source, VertexDescriptor target, const EP &ep,
            EdgeDescriptor firstNext, EdgeDescriptor secondNext):
                EdgeDataBase<EP>(source, target, ep), next{firstNext, secondNext} {}

    };


    // graph data
    template<typename DirectedCategory, typename VP, typename EP, typename GP>
    struct GraphData:public HasPropertiesBase<GP>
    {
        vector<VertexData<VP> > v;
        vector<EdgeData<DirectedCategory, EP> > e;
    };





    // realED: in memory [0, edgesNumber() * 2 )
    // virtualED: for user [0, edgesNumber() )
    // V2R: virtualED to realED
    // R2V: realED to virtualED
    // DistinguishDirectionGraph
    template<typename Direction, typename VP, typename EP, typename GP>
    struct DistinguishDirectionGraph: public GraphData<Direction, VP, EP, GP>
    {
    	static EdgeDescriptor R2V(EdgeDescriptor e) { return e; }

    	EdgeDescriptor next(EdgeDescriptor e) const
    	{
    	    return this->e[e].next;
    	}

    	EdgeDescriptor addEdge(VertexDescriptor a, VertexDescriptor b, const EP &ep = EP())
    	{
    	    this->e.push_back(EdgeData<Direction, EP>(a, b, ep, this->v[a].head));
    	    return this->v[a].head = this->e.size() - 1;
    	}
    };

    template<typename VP,
			 typename EP,
			 typename GP>
    struct DistinguishDirectionGraph<UndirectedGraphTag, VP, EP, GP>
    	:public GraphData<UndirectedGraphTag, VP, EP, GP>
    {
    	static EdgeDescriptor R2V(EdgeDescriptor e) { return e >> 1; }

        EdgeDescriptor next(EdgeDescriptor e) const
        {
            return this->e[e >> 1].next[e & 1];
        }

    	EdgeDescriptor addEdge(VertexDescriptor a, VertexDescriptor b, const EP &ep = EP())
		{
    	    EdgeDescriptor first = this->e.size() << 1;
    	    EdgeDescriptor second = first + 1;
            this->e.push_back(EdgeData<UndirectedGraphTag, EP>(a, b, ep, this->v[a].head, this->v[b].head));
            if(a == b)
            {
                this->e.back().next[1] = first;
            }

            this->v[a].head = first;
            this->v[b].head = second;

            return this->e.size() - 1;

		}
    };

    } // AdjacencyListPrivate




template<typename D, typename VP, typename EP, typename GP>
class VertexPropertyMap< AdjacencyList<D, VP, EP, GP>, VertexIndexTag>
	:public IdentityMap<typename GraphTraits< AdjacencyList<D, VP, EP, GP> >::VertexDescriptor>
{
public:
	VertexPropertyMap() = default;
	VertexPropertyMap(const AdjacencyList<D, VP, EP, GP> &_g){}
	using Type = VertexPropertyMap;
	using ConstType = Type;
};

template<typename D, typename VP, typename EP, typename GP>
class EdgePropertyMap<AdjacencyList<D, VP, EP, GP>, EdgeIndexTag>
	:public IdentityMap<typename GraphTraits<AdjacencyList<D, VP, EP, GP>>::EdgeDescriptor>
{
public:
	EdgePropertyMap() = default;
	EdgePropertyMap(const AdjacencyList<D, VP, EP, GP> &_g){}
	using Type = EdgePropertyMap;
	using ConstType = Type;
};

template<typename Direction, typename VP,
		 typename EP, typename GP>
class AdjacencyList: private AdjacencyListPrivate::DistinguishDirectionGraph<
					 Direction, VP, EP, GP>
{
	template<typename G> friend class AdjacencyListPrivate::OutEdgeIterator;
	template<typename G> friend class AdjacencyListPrivate::AdjacencyVertexIterator;

	using VertexData = AdjacencyListPrivate::VertexData<VP> ;
	using EdgeData = AdjacencyListPrivate::EdgeData<Direction, EP>;
	using G = AdjacencyList;
	using Base = AdjacencyListPrivate::DistinguishDirectionGraph<
				 Direction, VP, EP, GP>;
public:

    using VertexDescriptor = AdjacencyListPrivate::VertexDescriptor ;
    using EdgeDescriptor = AdjacencyListPrivate::EdgeDescriptor ;
    using DirectedCategory = Direction;

    using OutEdgeIterator = AdjacencyListPrivate::OutEdgeIterator<G>;
    using AdjacencyVertexIterator = AdjacencyListPrivate::AdjacencyVertexIterator<G>;

    using VerticesNumberType = AdjacencyListPrivate::SizeType;
    using VertexIterator = IntegerIterator<VertexDescriptor>;

    using EdgesNumberType = AdjacencyListPrivate::SizeType;
    using EdgeIterator = IntegerIterator<EdgeDescriptor>;

    template<typename Tag>
    using VertexPropertyMap = typename lz::VertexPropertyMap<G, Tag>::Type;
    template<typename Tag>
    using ConstVertexPropertyMap = typename lz::VertexPropertyMap<G, Tag>::ConstType;

    template<typename Tag>
    using EdgePropertyMap = typename lz::EdgePropertyMap<G, Tag>::Type;
    template<typename Tag>
    using ConstEdgePropertyMap = typename lz::EdgePropertyMap<G, Tag>::ConstType;

    static VertexDescriptor nullVertex() { return -1; }
    static EdgeDescriptor nullEdge() { return -1; }

	using VertexProperties = VP;
	using EdgeProperties = EP;
	using GraphProperties = GP;

	explicit AdjacencyList(VerticesNumberType n = 0, const VP& vp = VP())
	{
		this->v.assign(n, VertexData(-1, vp));
	}

	void clear()
	{
		this->v.clear();
		this->e.clear();
	}

    VP& vertexProperties(VertexDescriptor u)
    { return this->v[u].properties; }
    const VP& vertexProperties(VertexDescriptor u) const
    { return this->v[u].properties; }

    EP& edgeProperties(EdgeDescriptor e)
    { return this->e[e].properties; }
    const EP& edgeProperties(EdgeDescriptor e) const
    { return this->e[e].properties; }

    GP& graphProperties() { return this->properties; }
    const GP& graphProperties() const { return this->properties; }


// Below is friend free function

    // IncidenceGraph
	friend std::pair<OutEdgeIterator, OutEdgeIterator> outEdges(const G &g, VertexDescriptor u)
    {
	    return std::make_pair(OutEdgeIterator(g.v[u].head, &g), OutEdgeIterator(-1, &g)) ;
    }

	friend VertexDescriptor source(const G &g, EdgeDescriptor e)
    {
	    return g.e[e].source;
    }
	friend VertexDescriptor target(const G &g, EdgeDescriptor e)
    {
        return g.e[e].target;
    }

	// AdjacencyGraph
	friend std::pair<AdjacencyVertexIterator, AdjacencyVertexIterator> adjacencyVertices(const G &g, VertexDescriptor u)
	{
	    return std::make_pair(AdjacencyVertexIterator(u, g.v[u].head, &g), AdjacencyVertexIterator(u, -1, &g)) ;
	}


	// VertexListGraph
    friend VerticesNumberType verticesNumber(const G &g)
    {
        return g.v.size();
    }

	friend std::pair<VertexIterator, VertexIterator> vertices(const G &g)
    {
        return std::make_pair(VertexIterator(0), VertexIterator(verticesNumber(g)));
    }

	// EdgeListGraph
    friend std::pair<EdgeIterator, EdgeIterator> edges(const G &g)
    {
        return std::make_pair(EdgeIterator(0), EdgeIterator(edgesNumber(g)));
    }

    friend EdgesNumberType edgesNumber(const G &g)
    {
        return g.e.size();
    }

    // PropertyGraph
    template<typename Tag>
    friend VertexPropertyMap<Tag> vertexPropertyMap(G &g, Tag tag)
    {
        return makeVertexPropertyMap<G, Tag>(g, tag);
    }

    template<typename Tag>
    friend ConstVertexPropertyMap<Tag> vertexPropertyMap(const G &g, Tag tag)
    {
        return makeVertexPropertyMap<G, Tag>(g, tag);
    }

    template<typename Tag>
    friend EdgePropertyMap<Tag> edgePropertyMap(G &g, Tag tag)
    {
        return makeEdgePropertyMap<G, Tag>(g, tag);
    }

    template<typename Tag>
    friend ConstEdgePropertyMap<Tag> edgePropertyMap(const G &g, Tag tag)
    {
        return makeEdgePropertyMap<G, Tag>(g, tag);
    }

    // MutableGraph
    friend VertexDescriptor addVertex(G &g, const VP &vp = VP())
    {
        g.v.push_back(VertexData(-1, vp));
        return g.v.size() - 1;
    }

    friend EdgeDescriptor addEdge(G &g, VertexDescriptor a, VertexDescriptor b,  const EP &ep = EP())
    {
        return g.addEdge(a, b, ep);
    }














// below is orignal member function, should be deprecated

//  VertexDescriptor addVertex(const VP &vp = VP())
//  {
//      this->v.push_back(VertexData(-1, vp));
//      return this->v.size() - 1;
//  }
//  VerticesNumberType verticesNumber() const { return this->v.size(); }
//  EdgesNumberType edgesNumber() const { return this->R2V(this->e.size() - 1) + 1; }

//  VertexDescriptor source(EdgeDescriptor e) const { return this->e[this->V2R(e)].source; }
//  VertexDescriptor target(EdgeDescriptor e) const { return this->e[this->V2R(e)].target; }


//  EdgeDescriptor addEdge(VertexDescriptor a, VertexDescriptor b, const EP &ep = EP())
//  { return this->Base::addEdge(a, b, ep); }

//  template<typename Tag>
//  typename VertexPropertyMap<Tag>::Type
//  vertexPropertyMap(Tag tag)
//  {
//      return makeVertexPropertyMap<G, Tag>(*this, tag);
//  }

//  template<typename Tag>
//  typename VertexPropertyMap<Tag>::ConstType
//  vertexPropertyMap(Tag tag) const
//  {
//      return makeVertexPropertyMap<G, Tag>(*this, tag);
//
//// I dont know why wrong with below
////        return makeVertexPropertyMap(*this, tag);
//  }



//  template<typename Tag>
//  typename EdgePropertyMap<Tag>::Type
//  edgePropertyMap(Tag tag)
//  { return makeEdgePropertyMap<G, Tag>(*this, tag); }
//
//  template<typename Tag>
//  typename EdgePropertyMap<Tag>::ConstType
//  edgePropertyMap(Tag tag) const
//  {
//      return makeEdgePropertyMap<G, Tag>(*this, tag);
//  }

//  pair<VertexIterator, VertexIterator> vertices() const
//  {
//      return make_pair(VertexIterator(0),
//                       VertexIterator(this->verticesNumber()));
//  }
//
//  pair<EdgeIterator, EdgeIterator> edges() const
//  {
//      return make_pair(EdgeIterator(0), EdgeIterator(this->edgesNumber()));
//  }
//
//  pair<OutEdgeIterator, OutEdgeIterator> outEdges(VertexDescriptor u) const
//  {
//      return make_pair(OutEdgeIterator(this->v[u].head, this),
//                       OutEdgeIterator(-1, this)) ;
//  }









};



// // IncidenceGraph
//template<typename D, typename V, typename E, typename G>
//std::pair<AdjacencyList<D, V, E, G>::OutEdgeIterator, AdjacencyList<D, V, E, G>::OutEdgeIterator>
//outEdges(const AdjacencyList<D, V, E, G> &g, AdjacencyList<D, V, E, G>::VertexDescriptor u);
//
//VertexDescriptor source(const G &g, EdgeDescriptor e);
//VertexDescriptor target(const G &g, EdgeDescriptor e);
//
//// AdjacencyGraph
//std::pair<AdjacencyVertexIterator, AdjacencyVertexIterator> adjacencyVertices(const G &g, VertexDescriptor u);
//
//// VertexListGraph
//VerticesNumberType verticesNumber(const G &g);
//
//std::pair<VertexIterator, VertexIterator> vertices(const G &g);
//
//// EdgeListGraph
//std::pair<EdgeIterator, EdgeIterator> edges(const G &g);
//
//EdgesNumberType edgesNumber(const G &g);
//
//// PropertyGraph
//template<typename Tag>
//VertexPropertyMap<Tag> vertexPropertyMap(G &g, Tag tag);
//
//template<typename Tag>
//ConstVertexPropertyMap<Tag> vertexPropertyMap(const G &g, Tag tag);
//
//template<typename Tag>
//EdgePropertyMap<Tag> edgePropertyMap(G &g, Tag tag);
//
//template<typename Tag>
//ConstEdgePropertyMap<Tag> edgePropertyMap(const G &g, Tag tag);
//
//// MutableGraph
//VertexDescriptor addVertex(G &g, const VP &vp = VP());
//EdgeDescriptor addEdge(G &g, VertexDescriptor a, VertexDescriptor b,  const EP &ep = EP());









} // namespace lz


#endif // ADJACENCY_LIST_H
