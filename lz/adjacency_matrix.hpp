#ifndef ADJACENCY_MATRIX_H
#define ADJACENCY_MATRIX_H

#include <lz/iterator.h>
#include <vector>
#include "lz/graph_utility.h"
#include "lz/property.h"
namespace lz {

	namespace AdjacencyMatrixPrivate{

	using VertexDescriptor = int;
	using SizeType = int;
	struct EdgeDescriptor: public EqualityComparableFacade<EdgeDescriptor>
	{
	private:
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
		SizeType vertexNumber() const { return v.size(); }
	};
	template<> struct VertexDataList<NoProperty>
	{
		SizeType n;
		void assignVertex(SizeType _n, const NoProperty &vp) { n = _n; }
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
	:public IdentityMap<typename GraphTraits<AdjacencyMatrix<D, VP, EP, GP>>::VertexDescriptor>
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
	using Base = AdjacencyMatrixPrivate::DistinguishDirectionGraph<
				 Direction, VP, EP, GP>;
	using EdgeData = AdjacencyMatrixPrivate::EdgeData<EP>;

	using G = AdjacencyMatrix;
	using Base::edToId;
	using Base::makeEdgeDescriptor;
	using Base::e;
public:
	using DirectedCategory = Direction;
	using VertexProperties = VP;
	using EdgeProperties = EP;
	using GraphProperties = GP;

	using VertexDescriptor = AdjacencyMatrixPrivate::VertexDescriptor ;
	using EdgeDescriptor = AdjacencyMatrixPrivate::EdgeDescriptor ;
	using SizeType = AdjacencyMatrixPrivate::SizeType;


	using VertexIterator = IntegerIterator<VertexDescriptor>;
	template<typename Tag>
	using VertexPropertyMap = lz::VertexPropertyMap<G, Tag>;
	template<typename Tag>
	using EdgePropertyMap = lz::EdgePropertyMap<G, Tag>;




	explicit AdjacencyMatrix(SizeType n = 0, const VP & vp = VP())
	{
		assignVertex(n, vp);
	}
	void assignVertex(SizeType n, const VP &vp = VP())
	{
		Base::assignVertex(n, vp);
		e.assign(edToId(makeEdgeDescriptor(n - 1, n - 1)) + 1, EdgeData());
	}
	SizeType vertexNumber() const
	{ return Base::vertexNumber(); }

	std::pair<VertexIterator, VertexIterator> vertices() const
	{
		return std::make_pair(VertexIterator(0), VertexIterator(vertexNumber()));
	}


	// Be careful to call this function,
	// the time complexity of this function is O(V * V)
	SizeType edgeNumber() const
	{
		SizeType edge_num = 0;
		for(auto i = e.begin(); i != e.end(); ++ i)
		{
			if(i->exist) edge_num ++;
		}
		return edge_num;
	}


	EdgeDescriptor addEdge(VertexDescriptor a, VertexDescriptor b, const EP &ep = EP())
	{
		auto ed = makeEdgeDescriptor(a, b);
		auto id = edToId(ed);
		e[id].exist = 1;
		e[id].set(ep);
		return ed;
	}
	void removeEdge(EdgeDescriptor ed)
	{
		e[edToId(ed)].exist = 0;
	}
	std::pair<EdgeDescriptor, bool> edge(VertexDescriptor a, VertexDescriptor b) const
	{
		auto ed = makeEdgeDescriptor(a, b);
		return std::make_pair(ed, e[edToId(ed)].exist);
	}
	VertexDescriptor source(EdgeDescriptor ed) const { return ed.source; }
	VertexDescriptor target(EdgeDescriptor ed) const { return ed.target; }


	const GP& graphProperties() const { return this->properties; }
	GP& graphProperties() { return this->properties; }

	const VP& vertexProperties(VertexDescriptor u) const { return this->v[u]; }
	VP& vertexProperties(VertexDescriptor u) { return this->v[u]; }

	const EP& edgeProperties(EdgeDescriptor ed) const { return e[edToId(ed)].properties; }
	EP& edgeProperties(EdgeDescriptor ed) { return e[edToId(ed)].properties; }
};






































} // namespace lz




#endif // ADJACENCY_MATRIX_H
