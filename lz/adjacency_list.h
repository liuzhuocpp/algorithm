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

    // vertex data
    template<typename VP>
    struct VertexData
    {
        int head;
        VP vp;
        VertexData():head(-1){}
        VertexData(int head, const VP &vp):head(head), vp(vp) {}
    };
    template<>
    struct VertexData<NoProperty>
    {
        int head;
        VertexData():head(-1){}
        VertexData(int head, const NoProperty &vp):head(head) {}
    };

    // edge data
    template<typename EP>
    struct EdgeData
    {
        int source, target, next;
        EP ep;
        EdgeData() = default;
        EdgeData(int source, int target, int next, const EP & ep)
            :source(source), target(target), next(next), ep(ep){}
    };
    template<>
    struct EdgeData<NoProperty>
    {
        int source, target, next;
        EdgeData() = default;
        EdgeData(int source, int target, int next, const NoProperty & ep)
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


    	template<typename Graph, typename DirectedCategory, typename VP, typename EP, typename GP>
		friend class AdjacencyListBase;

    	const G *g = nullptr;
    	typename GraphTraits<G>::EdgeDescriptor i;
    	using Derived = OutEdgeIterator;

    	OutEdgeIterator(typename GraphTraits<G>::EdgeDescriptor i, const G *g): i(i), g(g){}
	public:
    	OutEdgeIterator() = default;

    	Derived& operator++()
		{
    		i = g->edgeData(i).next;
    		return *this;
		}
    	typename Derived::reference operator*() const
    	{
    		return i;
    	}
    	bool operator==(Derived const& o) const
		{
    		return i == o.i && g == o.g;
		}
	};

    template<typename G, typename Tag>
	class VertexPropertyMap
	{
    	template<typename Graph, typename DirectedCategory, typename VP, typename EP, typename GP>
		friend class AdjacencyListBase;

		G *g = nullptr;
		VertexPropertyMap(G *_g):g(_g){}
	public:
		using Type = VertexPropertyMap<G, Tag>;
		using ConstType = VertexPropertyMap<const G, Tag>;

		VertexPropertyMap() = default;
		auto operator[](typename GraphTraits<G>::VertexDescriptor u) const
		->decltype(get(g->vertexData(u).vp, Tag()))
		{
			return get(g->vertexData(u).vp, Tag());
		}
	};

    template<typename G, typename Tag>
    class EdgePropertyMap
   	{
    	template<typename Graph, typename DirectedCategory, typename VP, typename EP, typename GP>
		friend class AdjacencyListBase;

       	G *g = nullptr;
       	EdgePropertyMap(G *_g):g(_g){}
   	public :
		using Type = EdgePropertyMap<G, Tag>;
		using ConstType = EdgePropertyMap<const G, Tag>;

       	EdgePropertyMap() = default;
       	auto operator[](typename GraphTraits<G>::EdgeDescriptor e) const
		->decltype(get(g->edgeData(e).ep, Tag()))
   		{
       		return get(g->edgeData(e).ep, Tag());
   		}
   	};






    template<typename G,
			 typename Direction,
             typename VP,
             typename EP,
             typename GP>
    class AdjacencyListBase: public AdjacencyListPrivate::GraphData<VP, EP, GP>
    {
    	G& derived()
    	{
    		return *static_cast<G*>(this);
    	}
    	const G& derived() const
		{
			return *static_cast<G const*>(this);
		}




//        template<typename Graph> friend class AdjacencyListPrivate::OutEdgeIterator;

        template<typename Graph, typename Tag> friend class AdjacencyListPrivate::VertexPropertyMap;

    protected:
        using VertexData = AdjacencyListPrivate::VertexData<VP> ;
        using EdgeData = AdjacencyListPrivate::EdgeData<EP> ;
    public:
        using DirectedCategory = Direction;

        using VertexProperties = VP;
        using EdgeProperties = EP;
        using GraphProperties = GP;

        using VertexDescriptor = int ;
        using EdgeDescriptor = int ;

        using VertexIterator = IntegerIterator<VertexDescriptor>;
        using EdgeIterator = IntegerIterator<EdgeDescriptor>;
        using OutEdgeIterator = AdjacencyListPrivate::OutEdgeIterator<G>;

        template<typename Tag>
        using VertexPropertyMap = AdjacencyListPrivate::VertexPropertyMap<G, Tag>;

        template<typename Tag>
        using EdgePropertyMap = AdjacencyListPrivate::EdgePropertyMap<G, Tag>;

    private:
        VertexData& vertexData(VertexDescriptor u) { return this->v[u]; }
        const VertexData& vertexData(VertexDescriptor u) const { return this->v[u]; }

    public:

        explicit AdjacencyListBase(int n, const VP & vp)
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

        const VP& vertexProperties(int u) const { return this->derived().vertexData(u).vp; }
        VP& vertexProperties(int u) { return this->derived().vertexData(u).vp; }

        const EP& edgeProperties(int e) const { return this->derived().edgeData(e).ep; }
        EP& edgeProperties(int e) { return this->derived().edgeData(e).ep; }

        const GP& graphProperties() const { return this->gp; }
        GP& graphProperties() { return this->gp; }


        VertexDescriptor source(EdgeDescriptor e) const { return this->derived().edgeData(e).source; }
        VertexDescriptor target(EdgeDescriptor e) const { return this->derived().edgeData(e).target; }

        int vertexNumber() const { return this->v.size(); }
        int edgeNumber() const { return this->e.size(); }

        template<typename Tag>
        typename VertexPropertyMap<Tag>::Type vertexPropertyMap(Tag tag)
        {
        	return typename VertexPropertyMap<Tag>::Type(&this->derived());
        }
        template<typename Tag>
        typename VertexPropertyMap<Tag>::ConstType vertexPropertyMap(Tag tag) const
        {
        	return typename VertexPropertyMap<Tag>::ConstType(&this->derived());
        }

        template<typename Tag>
        typename EdgePropertyMap<Tag>::Type edgePropertyMap(Tag tag)
        {
        	return typename EdgePropertyMap<Tag>::Type(&this->derived());
        }
        template<typename Tag>
        typename EdgePropertyMap<Tag>::ConstType edgePropertyMap(Tag tag) const
        {
        	return typename EdgePropertyMap<Tag>::ConstType(&this->derived());
        }

        pair<VertexIterator, VertexIterator> vertices() const
    	{
        	return make_pair(VertexIterator(0),
        					 VertexIterator(this->vertexNumber()));
    	}
        pair<EdgeIterator, EdgeIterator> edges() const
    	{
    		return make_pair(EdgeIterator(0),
    						 EdgeIterator(this->edgeNumber()));
    	}
        pair<OutEdgeIterator, OutEdgeIterator> outEdges(VertexDescriptor u) const
        {
            return make_pair(OutEdgeIterator(this->derived().vertexData(u).head, &this->derived()),
                             OutEdgeIterator(-1, &this->derived())) ;
        }
    protected:
        EdgeDescriptor addSingleEdge(VertexDescriptor a, VertexDescriptor b, const EP &ep = EP())
        {
            this->e.push_back(EdgeData(a, b, this->derived().vertexData(a).head, ep));
            EdgeDescriptor ed = int(this->e.size()) - 1;
            this->derived().vertexData(a).head = ed;
            return ed;
        }
    };






    } // AdjacencyListPrivate

template<typename DirectedCategory,
		 typename VP,
		 typename EP,
		 typename GP>
class AdjacencyList: public AdjacencyListPrivate::AdjacencyListBase<
					 AdjacencyList<DirectedCategory, VP, EP, GP>,
					 DirectedCategory,
					 VP,
					 EP,
					 GP>
{
	using Base = AdjacencyListPrivate::AdjacencyListBase<
				 AdjacencyList<DirectedCategory, VP, EP, GP>,
				 DirectedCategory,
				 VP,
				 EP,
				 GP>;

	template<typename Graph, typename D, typename V, typename E, typename G>
	friend class AdjacencyListPrivate::AdjacencyListBase;

	template<typename Graph, typename Tag>
	friend class AdjacencyListPrivate::EdgePropertyMap;

	template<typename Graph> friend class AdjacencyListPrivate::OutEdgeIterator;

	typename Base::EdgeData& edgeData(typename Base::EdgeDescriptor e) { return this->e[e]; }
	const typename Base::EdgeData& edgeData(typename Base::EdgeDescriptor e) const { return this->e[e]; }
public:
	AdjacencyList(int n = 0, const VP &vp = VP()):Base(n, vp) {}
	typename Base::EdgeDescriptor addEdge(typename Base::VertexDescriptor a, typename Base::VertexDescriptor b, const EP &ep = EP())
	{
		return this->addSingleEdge(a, b, ep);
	}
};

//template<typename Directed,
//         typename VP,
//         typename EP,
//         typename GP>
//class AdjacencyList: public AdjacencyListPrivate::GraphData<VP, EP, GP>
//{
//	using G = AdjacencyList<Directed, VP, EP, GP>;
//    using VertexData = AdjacencyListPrivate::VertexData<VP> ;
//    using EdgeData = AdjacencyListPrivate::EdgeData<EP> ;
//
//    template<typename G> friend class AdjacencyListPrivate::OutEdgeIterator;
//
//    template<typename G, typename Tag> friend class AdjacencyListPrivate::VertexPropertyMap;
//    template<typename G, typename Tag> friend class AdjacencyListPrivate::EdgePropertyMap;
//public:
//    using VertexProperties = VP;
//    using EdgeProperties = EP;
//    using GraphProperties = GP;
//
//    using VertexDescriptor = int ;
//    using EdgeDescriptor = int ;
//
//    using VertexIterator = IntegerIterator<VertexDescriptor>;
//    using EdgeIterator = IntegerIterator<EdgeDescriptor>;
//    using OutEdgeIterator = AdjacencyListPrivate::OutEdgeIterator<G>;
//
//    template<typename Tag>
//    using VertexPropertyMap = AdjacencyListPrivate::VertexPropertyMap<G, Tag>;
//
//    template<typename Tag>
//    using EdgePropertyMap = AdjacencyListPrivate::EdgePropertyMap<G, Tag>;
//
//private:
//    VertexData& vertexData(VertexDescriptor u) { return this->v[u]; }
//    const VertexData& vertexData(VertexDescriptor u) const { return this->v[u]; }
//
//    EdgeData& edgeData(EdgeDescriptor e) { return this->e[e];}
//    const EdgeData& edgeData(EdgeDescriptor e) const { return this->e[e];}
//
//public:
//
//
//    explicit AdjacencyList(int n = 0, const VP & vp = VP())
//    {
//        this->v.assign(n, VertexData(-1, vp));
//    }
//    void clear()
//    {
//        this->v.clear();
//        this->e.clear();
//    }
//    int addVertex(const VP &vp = VP())
//    {
//        this->v.push_back(VertexData(-1, vp));
//        return this->v.size() - 1;
//    }
//
//    const VP& vertexProperties(int u) const { return this->vertexData(u).vp; }
//    VP& vertexProperties(int u) { return this->vertexData(u).vp; }
//
//    const EP& edgeProperties(int e) const { return this->edgeData(e).ep; }
//    EP& edgeProperties(int e) { return this->edgeData(e).ep; }
//
//    const GP& graphProperties() const { return this->gp; }
//    GP& graphProperties() { return this->gp; }
//
//
//    VertexDescriptor source(EdgeDescriptor e) const { return this->e[e].source; }
//    VertexDescriptor target(EdgeDescriptor e) const { return this->e[e].target; }
//
//    int vertexNumber() const { return this->v.size(); }
//    int edgeNumber() const { return this->e.size(); }
//
//    template<typename Tag>
//    VertexPropertyMap<Tag> vertexPropertyMap(Tag tag)
//	{
//    	return VertexPropertyMap<Tag>(this);
//	}
//    template<typename Tag>
//    EdgePropertyMap<Tag> edgePropertyMap(Tag tag)
//	{
//		return EdgePropertyMap<Tag>(this);
//	}
//
//    pair<VertexIterator, VertexIterator> vertices() const
//	{
//    	return make_pair(VertexIterator(0),
//    					 VertexIterator(this->vertexNumber()));
//	}
//    pair<EdgeIterator, EdgeIterator> edges() const
//	{
//		return make_pair(EdgeIterator(0),
//						 EdgeIterator(this->edgeNumber()));
//	}
//    pair<OutEdgeIterator, OutEdgeIterator> outEdges(VertexDescriptor u) const
//    {
//        return make_pair(OutEdgeIterator(this->v[u].head, this),
//                         OutEdgeIterator(-1, this )) ;
//    }
//    EdgeDescriptor addEdge(int a, int b, const EP &ep = EP())
//    {
//        this->e.push_back(EdgeData(a, b, this->v[a].head, ep));
//        this->v[a].head = int(this->e.size()) - 1;
//        return int(this->e.size()) - 1;
//    }
//};








//template<typename VP, typename EP, typename GP>
//class AdjacencyList<Undirected, VP, EP, GP>: private AdjacencyListPrivate::GraphData<VP, EP, GP>
//{
//    typedef AdjacencyListPrivate::VertexData<VP> VertexData;
//    typedef AdjacencyListPrivate::EdgeData<EP> EdgeData;
//
//public:
//    typedef VP VertexProperties;
//    typedef EP EdgeProperties;
//    typedef GP GraphProperties;
//
//    typedef int EdgeDescriptor;
//    typedef int VertexDescriptor;
//
//
//    explicit AdjacencyList(int n = 0, const VP & vp = VP())
//    {
//        this->v.assign(n, VertexData(-1, vp));
//    }
//    void clear()
//    {
//        this->v.clear();
//        this->e.clear();
//    }
//    // void assignVertex(int n = 0, const VP & vp = VP())
//    // {
//    //     this->v.assign(n, VD(-1, vp));
//    // }
//    int addVertex(const VP &vp = VP())
//    {
//        this->v.push_back(VertexData(-1, vp));
//        return this->v.size() - 1;
//    }
//
//    const VP& vertexProperties(int u) const { return this->v[u].vp; }
//    VP& vertexProperties(int u) { return this->v[u].vp; }
//
//    const GP& graphProperties() const { return this->gp; }
//    GP& graphProperties() { return this->gp; }
//
//    int source(int e) const { return this->e[e << 1].source; }
//    int target(int e) const { return this->e[e << 1].target; }
//    const EP& edgeProperties(int e) const { return this->e[e << 1].ep; }
//    EP& edgeProperties(int e) { return this->e[e << 1].ep; }
//
//    int vertexNumber() const { return this->v.size(); }
//    int edgeNumber() const { return this->e.size() >> 1; }
//
//    class OutEdgeIterator
//    {
//        friend class AdjacencyList;
//        int i;
//        const vector<EdgeData> *e;
//
//        OutEdgeIterator(int i, const vector<EdgeData> *e): i(i), e(e) {}
//    public:
//        OutEdgeIterator():i(-1), e(nullptr) {}
//
//        int operator*() const
//        {
//        	if(i == -1) return -1;
//        	return i >> 1;
//        }
//        OutEdgeIterator& operator++() { i = (*e)[i].next;  return *this; }
//        OutEdgeIterator operator++(int) { OutEdgeIterator t(*this); ++*this;  return t; }
//        bool operator==(const OutEdgeIterator &o) const
//    	{
//        	return i == o.i && e == o.e;
//    	}
//        bool operator!=(const OutEdgeIterator &o) const { return !(*this == o); }
//    };
//    pair<OutEdgeIterator, OutEdgeIterator> outEdges(int u) const
//    {
//        return make_pair(OutEdgeIterator(this->v[u].head, &this->e),
//                         OutEdgeIterator(-1, &this->e ) ) ;
//    }
//    void addEdge(int a, int b, const EP &ep = EP())
//    {
//    	_addEdge(a, b, ep);
//    	_addEdge(b, a, ep);
//    }
//private:
//    void _addEdge(int a, int b, const EP &ep = EP())
//    {
//        this->e.push_back(EdgeData(a, b, this->v[a].head, ep));
//        this->v[a].head = int(this->e.size()) - 1;
//    }
//};
//


















} // namespace lz


#endif // ADJACENCY_LIST_H
