#ifndef ADJACENCY_MATRIX_H
#define ADJACENCY_MATRIX_H

#include <lz/iterator.h>
#include <vector>
#include "lz/graph_utility.h"
#include "lz/property.h"
namespace lz {

	namespace AdjacencyMatrixPrivate {

	using VertexDescriptor = int;
	using SizeType = int;
	struct EdgeDescriptor: public EqualityComparableFacade<EdgeDescriptor>
	{
		template<typename Direction, typename VP, typename EP, typename GP>
		friend class DistinguishDirectionGraph;

		VertexDescriptor source, target;
		EdgeDescriptor(VertexDescriptor s, VertexDescriptor t): source(s), target(t) {}
	public:
		EdgeDescriptor(): source(-1), target(-1) {}
		bool operator== (EdgeDescriptor o) const
		{
			return source == o.source && target == o.target;
		}

	};



	template<typename VP>
	struct VertexDataList
	{
		std::vector<VP> v;
		void assignVertex(SizeType _n, const VP &vp) { v.assign(_n, vp); }
		VertexDescriptor addVertex(const VP &vp = VP())
		{
		    v.push_back(vp);
		    return v.size() - 1;
		}
		SizeType vertexNumber() const { return v.size(); }
	};
	template<> struct VertexDataList<NoProperty>
	{
		SizeType n;
		void assignVertex(SizeType _n, const NoProperty &vp) { n = _n; }
		VertexDescriptor addVertex(const NoProperty &vp = NoProperty())
        {
            n ++;
            return n - 1;
        }
		SizeType vertexNumber() const { return n; }
	};
	template<typename EP>
	struct EdgeData: public ChooseDefineProperties<EP>
	{
		bool exist = 0;
	};

	template<typename VP, typename EP, typename GP>
	struct GraphData: public VertexDataList<VP>,
					  public ChooseDefineProperties<GP>
	{
		std::vector<EdgeData<EP> > e;
	};

	template<typename T>
	static T progressionSum(T n)
	{
		if(n & 1) return ((n + 1) >> 1) * n;
		else return (n >> 1) * (n + 1) ;
	}

    // DistinguishDirectionGraph
    template<typename Direction, typename VP, typename EP, typename GP>
    struct DistinguishDirectionGraph: public GraphData<VP, EP, GP>
    {
        using Base = GraphData<VP, EP, GP>;

        SizeType totalEdges() const
        {
            auto n = Base::vertexNumber();
            return n * n;
        }


    	SizeType edToId(EdgeDescriptor e) const
    	{
    		SizeType n = this->vertexNumber();
    		return n * e.source + e.target;
    	}
    	EdgeDescriptor makeEdgeDescriptor(VertexDescriptor a, VertexDescriptor b) const
    	{
    		return EdgeDescriptor(a, b);
    	}
    };

    template<typename VP, typename EP, typename GP>
	struct DistinguishDirectionGraph<UndirectedGraphTag, VP, EP, GP>: public GraphData<VP, EP, GP>
	{
        using Base = GraphData<VP, EP, GP>;
        SizeType totalEdges() const
        {
            auto n = Base::vertexNumber();
            return progressionSum(n);
        }

    	SizeType edToId(EdgeDescriptor e)  const // ensure e.source >= e.target
		{
    		SizeType s = e.source, t = e.target; // return (s + 1) * s / 2 + t
    		return progressionSum(s) + t;
		}
    	EdgeDescriptor makeEdgeDescriptor(VertexDescriptor a, VertexDescriptor b) const
		{
    		if(a < b) std::swap(a, b);
			return EdgeDescriptor(a, b);
		}

	};


	} // namespace AdjacencyMatrixPrivate


template<typename Direction = DirectedGraphTag, typename VP = NoProperty,
		 typename EP = NoProperty, typename GP = NoProperty>
class AdjacencyMatrix;

template<typename D, typename VP, typename EP, typename GP>
class VertexPropertyMap<AdjacencyMatrix<D, VP, EP, GP>, VertexIndexTag>
	: public IdentityMap<typename GraphTraits<AdjacencyMatrix<D, VP, EP, GP>>::VertexDescriptor>
{
	using G = AdjacencyMatrix<D, VP, EP, GP>;
public:
	VertexPropertyMap() = default;
	VertexPropertyMap(const G &_g) {}
	using Type = VertexPropertyMap;
	using ConstType = Type;
};



template<typename Direction, typename VP, typename EP, typename GP>

class AdjacencyMatrix:private AdjacencyMatrixPrivate::DistinguishDirectionGraph<Direction, VP, EP, GP>
{
	using Base = AdjacencyMatrixPrivate::DistinguishDirectionGraph<Direction, VP, EP, GP>;
	using EdgeData = AdjacencyMatrixPrivate::EdgeData<EP>;
	using G = AdjacencyMatrix;

public:
	using DirectedCategory = Direction;
	using VertexProperties = VP;
	using EdgeProperties = EP;
	using GraphProperties = GP;

	using VertexDescriptor = AdjacencyMatrixPrivate::VertexDescriptor ;
	using EdgeDescriptor = AdjacencyMatrixPrivate::EdgeDescriptor ;
	using VerticesNumberType = AdjacencyMatrixPrivate::SizeType;
	using VertexIterator = IntegerIterator<VertexDescriptor>;

    using EdgeIterator = void;
    using OutEdgeIterator = void;
    using AdjacencyVertexIterator = void;
    using EdgesNumberType = void;



    template<typename Tag>
    using VertexPropertyMap = typename lz::VertexPropertyMap<G, Tag>::Type;
    template<typename Tag>
    using ConstVertexPropertyMap = typename lz::VertexPropertyMap<G, Tag>::ConstType;

    template<typename Tag>
    using EdgePropertyMap = typename lz::EdgePropertyMap<G, Tag>::Type;
    template<typename Tag>
    using ConstEdgePropertyMap = typename lz::EdgePropertyMap<G, Tag>::ConstType;

	static const VertexDescriptor nullVertex()
	{
	    return -1;
	}

	static const EdgeDescriptor nullEdge()
    {
        return EdgeDescriptor(-1, -1);
    }
private :
    EdgeData& edgeData(EdgeDescriptor e)
    {
        return this->e[this->edToId(e)];
    }
    const EdgeData& edgeData(EdgeDescriptor e) const
    {
        return this->e[this->edToId(e)];
    }
public:
	explicit AdjacencyMatrix(VerticesNumberType n = 0, const VP & vp = VP())
	{
		assignVertex(n, vp);
	}
	void assignVertex(VerticesNumberType n, const VP &vp = VP())
	{
		Base::assignVertex(n, vp);
		this->e.assign(this->totalEdges(), EdgeData());
	}


	const GP& graphProperties() const { return this->properties; }
	GP& graphProperties() { return this->properties; }

	const VP& vertexProperties(VertexDescriptor u) const { return this->v[u]; }
	VP& vertexProperties(VertexDescriptor u) { return this->v[u]; }

	const EP& edgeProperties(EdgeDescriptor e) const { return this->edgeData(e).properties; }
	EP& edgeProperties(EdgeDescriptor e) { return this->edgeData(e).properties; }


//	VertexListGraph
	friend VerticesNumberType verticesNumber(const G &g)
	{
	    return g.vertexNumber();
	}

    friend std::pair<VertexIterator, VertexIterator> vertices(const G &g)
    {
        return std::make_pair(VertexIterator(0), VertexIterator(verticesNumber(g)));
    }

//  AdjacencyMatrix
    friend EdgeDescriptor edge(const G &g, VertexDescriptor a, VertexDescriptor b)
    {
        auto ed = g.makeEdgeDescriptor(a, b);
        if(g.edgeData(ed).exist)
        {
            return ed;
        }
        else
        {
            return nullEdge();
        }
    }

    friend VertexDescriptor source(const G &g, EdgeDescriptor ed) { return ed.source; }
    friend VertexDescriptor target(const G &g, EdgeDescriptor ed) { return ed.target; }

//  PropertyGraph
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


//  MutableGraph
    friend VertexDescriptor addVertex( G &g, const VP &vp = VP())
    {
        VertexDescriptor ans = g.addVertex(vp);
        g.e.resize(g.totalEdges());
        return ans;

    }

    friend EdgeDescriptor addEdge(G &g, VertexDescriptor a, VertexDescriptor b, const EP &ep = EP())
    {
        auto ed = g.makeEdgeDescriptor(a, b);

        EdgeData& edgeData = g.edgeData(ed);
        edgeData.exist = true;
        edgeData.set(ep);
        return ed;
    }
    void removeEdge(const G &g, EdgeDescriptor ed)
    {
        g.edgeData(ed).exist = false;
    }


};






































} // namespace lz



namespace std {
  template <class Char, class Traits>
  std::basic_ostream<Char, Traits>&
  operator<<(std::basic_ostream<Char, Traits>& os,
             const lz::AdjacencyMatrixPrivate::EdgeDescriptor&  e)
  {
    return os << "(" << e.source << "," << e.target << ")";
  }
}






#endif // ADJACENCY_MATRIX_H
