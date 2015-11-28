#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H


#include <vector>
#include <tuple>
#include <algorithm>
#include "lz/property.h"

#include "lz/graph_utility.h"
#include "lz/iterator_facade.h"
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



    // vertex data
    template<typename VP>
    struct VertexData: public ChooseDefineProperties<VP>
	{
    	EdgeDescriptor head;
    	VertexData(EdgeDescriptor _head, const VP &vp):head(_head)
    	{
    		ChooseDefineProperties<VP>::set(vp);
    	}
	};

    // edge data
	template<typename EP>
	struct EdgeData:ChooseDefineProperties<EP>
	{
		VertexDescriptor source, target;
		EdgeDescriptor next;

		EdgeData(VertexDescriptor source, VertexDescriptor target, EdgeDescriptor next, const EP & ep)
		:source(source), target(target), next(next)
		{
			ChooseDefineProperties<EP>::set(ep);
		}
	};

    // graph data
    template<typename VP, typename EP, typename GP>
    struct GraphData:public ChooseDefineProperties<GP>
    {
        vector<VertexData<VP> > v;
        vector<EdgeData<EP> > e;
        EdgeDescriptor addEdge(VertexDescriptor a, VertexDescriptor b, const EP &ep = EP())
		{
			this->e.push_back(EdgeData<EP>(a, b, this->v[a].head, ep));
			return this->v[a].head = this->e.size() - 1;
		}
    };



    template<typename G>
    class OutEdgeIterator: public IteratorFacade<
		OutEdgeIterator<G>,
		std::forward_iterator_tag,
		typename GraphTraits<G>::EdgeDescriptor,
		std::ptrdiff_t,
		typename GraphTraits<G>::EdgeDescriptor*,
		typename GraphTraits<G>::EdgeDescriptor >
	{
    	template<typename D, typename VP, typename EP, typename GP> friend class AdjacencyList;
    	const G *g = nullptr;
    	EdgeDescriptor i = -1; // realED
    	OutEdgeIterator(EdgeDescriptor i, const G *g): i(i), g(g){} //AdjacencyList call this function
	public:
    	OutEdgeIterator() = default;

    	OutEdgeIterator& operator++()
		{
    		i = g->e[i].next;
    		return *this;
		}
    	typename OutEdgeIterator::reference operator*() const
    	{
    		return G::R2V(i);
    	}
    	bool operator==(OutEdgeIterator const& o) const
		{
    		return G::R2V(i) == G::R2V(o.i) && g == o.g;
		}
	};

    // realED: in memory
    // virtualED: for user
    // V2R: virtualED to realED
    // R2V: realED to virtualED
    // DistinguishDirectionGraph
    template<typename Direction, typename VP, typename EP, typename GP>
    struct DistinguishDirectionGraph: public GraphData<VP, EP, GP>
    {
    	static EdgeDescriptor V2R(EdgeDescriptor e) { return e; }
    	static EdgeDescriptor R2V(EdgeDescriptor e) { return e; }
    	using GraphData<VP, EP, GP>::addEdge;
    };

    template<typename VP,
			 typename EP,
			 typename GP>
    struct DistinguishDirectionGraph<UndirectedGraphTag, VP, EP, GP>
    	:public GraphData<VP, EP, GP>
    {
    	static EdgeDescriptor V2R(EdgeDescriptor e) { return e << 1; }
    	static EdgeDescriptor R2V(EdgeDescriptor e) { return e >> 1; }
    	using Base = GraphData<VP, EP, GP>;
    	EdgeDescriptor addEdge(VertexDescriptor a, VertexDescriptor b, const EP &ep = EP())
		{
    		Base::addEdge(a, b, ep);
			return Base::addEdge(b, a, ep) >> 1;
		}
    };

    } // AdjacencyListPrivate



template<typename D, typename VP, typename EP, typename GP>
class VertexPropertyMap<AdjacencyList<D, VP, EP, GP>, VertexIndexTag>
	:public IdentityMap<typename GraphTraits<AdjacencyList<D, VP, EP, GP>>::VertexDescriptor>
{
	using G = AdjacencyList<D, VP, EP, GP>;
public:
	VertexPropertyMap() = default;
	VertexPropertyMap(G *_g) {}
	using Type = VertexPropertyMap<G, VertexIndexTag>;
	using ConstType = VertexPropertyMap<const G, VertexIndexTag>;
};

template<typename D, typename VP, typename EP, typename GP>
class EdgePropertyMap<AdjacencyList<D, VP, EP, GP>, EdgeIndexTag>
	:public IdentityMap<typename GraphTraits<AdjacencyList<D, VP, EP, GP>>::EdgeDescriptor>
{
	using G = AdjacencyList<D, VP, EP, GP>;
public:
	EdgePropertyMap() = default;
	EdgePropertyMap(G *_g){}
	using Type = EdgePropertyMap<G, EdgeIndexTag>;
	using ConstType = EdgePropertyMap<const G, EdgeIndexTag>;
};



template<typename Direction, typename VP,
		 typename EP, typename GP>
class AdjacencyList: private AdjacencyListPrivate::DistinguishDirectionGraph<
					 Direction, VP, EP, GP>
{
	template<typename G> friend class AdjacencyListPrivate::OutEdgeIterator;

	using VertexData = AdjacencyListPrivate::VertexData<VP> ;
	using EdgeData = AdjacencyListPrivate::EdgeData<EP> ;
	using G = AdjacencyList;
	using Base = AdjacencyListPrivate::DistinguishDirectionGraph<
				 Direction, VP, EP, GP>;
public:
	using DirectedCategory = Direction;
	using VertexProperties = VP;
	using EdgeProperties = EP;
	using GraphProperties = GP;

	using VertexDescriptor = AdjacencyListPrivate::VertexDescriptor ;
	using EdgeDescriptor = AdjacencyListPrivate::EdgeDescriptor ;
	using SizeType = AdjacencyListPrivate::SizeType;

	using VertexIterator = IntegerIterator<VertexDescriptor>;
	using EdgeIterator = IntegerIterator<EdgeDescriptor>;
	using OutEdgeIterator = AdjacencyListPrivate::OutEdgeIterator<G>;
	template<typename Tag>
	using VertexPropertyMap = lz::VertexPropertyMap<G, Tag>;
	template<typename Tag>
	using EdgePropertyMap = lz::EdgePropertyMap<G, Tag>;

	explicit AdjacencyList(SizeType n = 0, const VP & vp = VP())
	{
		this->v.assign(n, VertexData(-1, vp));
	}
	void clear()
	{
		this->v.clear();
		this->e.clear();
	}
	VertexDescriptor addVertex(const VP &vp = VP())
	{
		this->v.push_back(VertexData(-1, vp));
		return this->v.size() - 1;
	}
	SizeType vertexNumber() const { return this->v.size(); }
	SizeType edgeNumber() const { return this->R2V(this->e.size() - 1) + 1; }
	const GP& graphProperties() const { return this->properties; }
	GP& graphProperties() { return this->properties; }
	VertexDescriptor source(EdgeDescriptor e) const { return this->e[this->V2R(e)].source; }
	VertexDescriptor target(EdgeDescriptor e) const { return this->e[this->V2R(e)].target; }

	VP& vertexProperties(VertexDescriptor u)
	{ return this->v[u].properties; }
	const VP& vertexProperties(VertexDescriptor u) const
	{ return this->v[u].properties; }

	EP& edgeProperties(EdgeDescriptor e)
	{ return this->e[this->V2R(e)].properties; }
	const EP& edgeProperties(EdgeDescriptor e) const
	{ return this->e[this->V2R(e)].properties; }

	EdgeDescriptor addEdge(VertexDescriptor a, VertexDescriptor b, const EP &ep = EP())
	{ return this->Base::addEdge(a, b, ep); }

	template<typename Tag>
	typename VertexPropertyMap<Tag>::Type vertexPropertyMap(Tag tag)
	{ return typename VertexPropertyMap<Tag>::Type(this); }

	template<typename Tag>
	typename VertexPropertyMap<Tag>::ConstType vertexPropertyMap(Tag tag) const
	{ return typename VertexPropertyMap<Tag>::ConstType(this); }

	template<typename Tag>
	typename EdgePropertyMap<Tag>::Type edgePropertyMap(Tag tag)
	{ return typename EdgePropertyMap<Tag>::Type(this); }

	template<typename Tag>
	typename EdgePropertyMap<Tag>::ConstType edgePropertyMap(Tag tag) const
	{ return typename EdgePropertyMap<Tag>::ConstType(this); }

	pair<VertexIterator, VertexIterator> vertices() const
	{
		return make_pair(VertexIterator(0),
						 VertexIterator(this->vertexNumber()));
	}
	pair<EdgeIterator, EdgeIterator> edges() const
	{
		return make_pair(EdgeIterator(0), EdgeIterator(this->edgeNumber()));
	}
	pair<OutEdgeIterator, OutEdgeIterator> outEdges(VertexDescriptor u) const
	{
		return make_pair(OutEdgeIterator(this->v[u].head, this),
						 OutEdgeIterator(-1, this)) ;
	}

};





} // namespace lz


#endif // ADJACENCY_LIST_H
