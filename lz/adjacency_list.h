#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H


#include <vector>
#include <tuple>
#include <algorithm>
#include "lz/property.h"

#include "lz/graph_utility.h"
#include "lz/iterator.h"

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

    // vertex data
    template<typename VP>
    struct VertexData
    {
    	EdgeDescriptor head;
        VP vp;
        VertexData():head(-1){}
        VertexData(int head, const VP &vp):head(head), vp(vp) {}
    };
    template<>
    struct VertexData<NoProperty>
    {
    	EdgeDescriptor head;
        VertexData():head(-1){}
        VertexData(int head, const NoProperty &vp):head(head) {}
    };

    // edge data
    template<typename EP>
    struct EdgeData
    {
    	VertexDescriptor source, target;
        EdgeDescriptor next;
        EP ep;
        EdgeData() = default;
        EdgeData(VertexDescriptor source, VertexDescriptor target, EdgeDescriptor next, const EP & ep)
            :source(source), target(target), next(next), ep(ep){}
    };
    template<>
    struct EdgeData<NoProperty>
    {
    	VertexDescriptor source, target;
        EdgeDescriptor next;
        EdgeData() = default;
        EdgeData(VertexDescriptor source, VertexDescriptor target, EdgeDescriptor next, const NoProperty & ep)
            :source(source), target(target), next(next){}
    };

    // graph data
    template<typename VP, typename EP, typename GP>
    struct GraphData
    {
    	vector<VertexData<VP> > v;
        vector<EdgeData<EP> > e;
        GraphData gp;
    };

    template<typename VP, typename EP>
    struct GraphData<VP, EP, NoProperty>
    {
        vector<VertexData<VP> > v;
        vector<EdgeData<EP> > e;
    };


    template<typename G>
    class OutEdgeIterator:
    		public IteratorFacade<
				OutEdgeIterator<G>,
				std::forward_iterator_tag,
				typename GraphTraits<G>::EdgeDescriptor,
				std::ptrdiff_t,
				typename GraphTraits<G>::EdgeDescriptor*,
				typename GraphTraits<G>::EdgeDescriptor
				>
	{
    	template<typename D, typename VP, typename EP, typename GP> friend class AdjacencyList;
    	const G *g = nullptr;
    	EdgeDescriptor i; // realED
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

    template<typename G, typename Tag>
	class VertexPropertyMap
	{
    	template<typename D, typename VP, typename EP, typename GP> friend class AdjacencyList;
		G *g = nullptr;
		VertexPropertyMap(G *_g):g(_g){}
	public:
		using Type = VertexPropertyMap<G, Tag>;
		using ConstType = VertexPropertyMap<const G, Tag>;

		VertexPropertyMap() = default;
		auto operator[](typename GraphTraits<G>::VertexDescriptor u) const
		->decltype(get(g->v[u].vp, Tag()))
		{
			return get(g->v[u].vp, Tag());
		}
	};

    template<typename G, typename Tag>
    class EdgePropertyMap
   	{
    	template<typename D, typename VP, typename EP, typename GP> friend class AdjacencyList;


       	G *g = nullptr;
       	EdgePropertyMap(G *_g):g(_g){}
   	public :
		using Type = EdgePropertyMap<G, Tag>;
		using ConstType = EdgePropertyMap<const G, Tag>;

       	EdgePropertyMap() = default;
       	auto operator[](typename GraphTraits<G>::EdgeDescriptor e) const
		->decltype(get(g->e[G::V2R(e)].ep, Tag()))
   		{
       		return get(g->e[G::V2R(e)].ep, Tag());
   		}
   	};

    template<typename VP, typename EP, typename GP>
    struct GraphDataWrapper: public GraphData<VP, EP, GP>
    {
    	EdgeDescriptor addSingleEdge(VertexDescriptor a, VertexDescriptor b, const EP &ep = EP())
		{
			this->e.push_back(EdgeData<EP>(a, b, this->v[a].head, ep));
			return this->v[a].head = this->e.size() - 1;
		}
    };



    // realED: in memory
    // virtualED: for user
    // V2R: virtualED to realED
    // R2V: realED to virtualED
    // DistinguishDirectionGraph
    template<typename Direction,
             typename VP,
             typename EP,
             typename GP>
    struct DistinguishDirectionGraph: public GraphDataWrapper<VP, EP, GP>
    {
    	static EdgeDescriptor V2R(EdgeDescriptor e) { return e; }
    	static EdgeDescriptor R2V(EdgeDescriptor e) { return e; }
    	EdgeDescriptor addEdge(VertexDescriptor a, VertexDescriptor b, const EP &ep = EP())
    	{
    		return this->addSingleEdge(a, b, ep);
    	}
    };

    template<typename VP,
			 typename EP,
			 typename GP>
    struct DistinguishDirectionGraph<UndirectedGraphTag, VP, EP, GP>: public GraphDataWrapper<VP, EP, GP>
    {
    	static EdgeDescriptor V2R(EdgeDescriptor e) { return e << 1; }
    	static EdgeDescriptor R2V(EdgeDescriptor e) { return e >> 1; }
    	EdgeDescriptor addEdge(VertexDescriptor a, VertexDescriptor b, const EP &ep = EP())
		{
    		this->addSingleEdge(a, b, ep);
			return this->addSingleEdge(b, a, ep) >> 1;
		}
    };

    } // AdjacencyListPrivate


template<typename Direction,
		 typename VP,
		 typename EP,
		 typename GP>
class AdjacencyList: private AdjacencyListPrivate::DistinguishDirectionGraph<
					 Direction,
					 VP,
					 EP,
					 GP>
{
	template<typename G> friend class AdjacencyListPrivate::OutEdgeIterator;
	template<typename G, typename Tag> friend class AdjacencyListPrivate::VertexPropertyMap;
	template<typename G, typename Tag> friend class AdjacencyListPrivate::EdgePropertyMap;

	using VertexData = AdjacencyListPrivate::VertexData<VP> ;
	using EdgeData = AdjacencyListPrivate::EdgeData<EP> ;
	using G = AdjacencyList;
	using Base = AdjacencyListPrivate::DistinguishDirectionGraph<
				 Direction,
				 VP,
				 EP,
				 GP>;
public:
	using DirectedCategory = Direction;
	using VertexProperties = VP;
	using EdgeProperties = EP;
	using GraphProperties = GP;

	using VertexDescriptor = AdjacencyListPrivate::VertexDescriptor ;
	using EdgeDescriptor = AdjacencyListPrivate::EdgeDescriptor ;

	using VertexIterator = IntegerIterator<VertexDescriptor>;
	using EdgeIterator = IntegerIterator<EdgeDescriptor>;
	using OutEdgeIterator = AdjacencyListPrivate::OutEdgeIterator<G>;

	template<typename Tag>
	using VertexPropertyMap = AdjacencyListPrivate::VertexPropertyMap<G, Tag>;

	template<typename Tag>
	using EdgePropertyMap = AdjacencyListPrivate::EdgePropertyMap<G, Tag>;

	explicit AdjacencyList(int n = 0, const VP & vp = VP())
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
	int vertexNumber() const { return this->v.size(); }
	int edgeNumber() const { return this->R2V(this->e.size() - 1) + 1; }
	const GP& graphProperties() const { return this->gp; }
	GP& graphProperties() { return this->gp; }
	VertexDescriptor source(EdgeDescriptor e) const { return this->e[this->V2R(e)].source; }
	VertexDescriptor target(EdgeDescriptor e) const { return this->e[this->V2R(e)].target; }

	VP& vertexProperties(VertexDescriptor u)
	{ return this->v[u].vp; }
	const VP& vertexProperties(VertexDescriptor u) const
	{ return this->v[u].vp; }

	EP& edgeProperties(EdgeDescriptor e)
	{ return this->e[e].ep; }
	const EP& edgeProperties(EdgeDescriptor e) const
	{ return this->e[e].ep; }

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
