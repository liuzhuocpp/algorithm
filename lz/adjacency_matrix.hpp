#ifndef ADJACENCY_MATRIX_H
#define ADJACENCY_MATRIX_H

#include <vector>
#include "lz/graph_utility.h"
#include "lz/property.h"
#include "lz/iterator_facade.h"
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
	template<typename T>
	static T progressionSum(T n)
	{
		if(n & 1) return ((n + 1) >> 1) * n;
		else return (n >> 1) * (n + 1) ;
	}
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

    template<typename Direction, typename VP, typename EP, typename GP>
    struct Graph: public DistinguishDirectionGraph<Direction, VP, EP, GP>
	{
    	const EdgeData<EP>& edgeData(EdgeDescriptor ed) const { return this->e[this->edToId(ed)]; }
    	EdgeData<EP>& edgeData(EdgeDescriptor ed) { return this->e[this->edToId(ed)]; }

    	using Base = DistinguishDirectionGraph<Direction, VP, EP, GP>;
		void assignVertex(SizeType n, VP vp)
		{
			Base::assignVertex(n, vp);
			Base::e.assign(Base::edToId(this->makeEdgeDescriptor(n - 1, n - 1)) + 1, EdgeData<EP>());
		}
	};
	}

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
	VertexPropertyMap(G *_g) {}
	using Type = VertexPropertyMap<G, VertexIndexTag>;
	using ConstType = VertexPropertyMap<const G, VertexIndexTag>;
};



template<typename Direction, typename VP, typename EP, typename GP>

class AdjacencyMatrix:private AdjacencyMatrixPrivate::Graph<Direction, VP, EP, GP>
{
	using Base = AdjacencyMatrixPrivate::Graph<
				 Direction, VP, EP, GP>;
	using EdgeData = AdjacencyMatrixPrivate::EdgeData<EP>;

	using G = AdjacencyMatrix;
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
		Base::assignVertex(n, vp);
	}
	void assignVertex(SizeType n, const VP &vp = VP())
	{
		Base::assignVertex(n, vp);
	}
	SizeType vertexNumber() const
	{ return Base::vertexNumber(); }

	std::pair<VertexIterator, VertexIterator> vertices() const
	{
		return std::make_pair(0, vertexNumber());
	}


	// Be careful to call this function,
	// the time complexity of this function is O(E)
	SizeType edgeNumber() const
	{
		SizeType edge_num = 0;
		for(auto i = this->e.begin(); i != this->e.end(); ++ i)
		{
			if(i->exist) edge_num ++;
		}
		return edge_num;
	}


	EdgeDescriptor addEdge(VertexDescriptor a, VertexDescriptor b, const EP &ep = EP())
	{
		auto ed = this->makeEdgeDescriptor(a, b);
		this->edgeData(ed).exist = 1;
		this->edgeData(ed).set(ep);
		return ed;
	}
	void removeEdge(EdgeDescriptor ed)
	{
		this->edgeData(ed).exist = 0;
	}
	std::pair<EdgeDescriptor, bool> edge(VertexDescriptor a, VertexDescriptor b) const
	{
		auto ed = this->makeEdgeDescriptor(a, b);
		return std::make_pair(ed, this->edgeData(ed).exist);
	}
	VertexDescriptor source(EdgeDescriptor ed) const { return ed.source; }
	VertexDescriptor target(EdgeDescriptor ed) const { return ed.target; }
	const GP& graphProperties() const { return this->properties; }
	GP& graphProperties() { return this->properties; }


	const VP& vertexProperties(VertexDescriptor u) const { return this->v[u]; }
	VP& vertexProperties(VertexDescriptor u) { return this->v[u]; }

	const EP& edgeProperties(EdgeDescriptor ed) const { return this->edgeData(ed).properties; }
	EP& edgeProperties(EdgeDescriptor ed) { return this->edgeData(ed).properties; }
};






































} // namespace lz




#endif // ADJACENCY_MATRIX_H
