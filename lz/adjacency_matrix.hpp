#ifndef ADJACENCY_MATRIX_H
#define ADJACENCY_MATRIX_H

#include <vector>
#include "lz/graph_utility.h"
#include "lz/property.h"
//#include "multi_array.h"

namespace lz {

	namespace AdjacencyMatrixPrivate{

	using VertexDescriptor = int;
	using SizeType = int;
	struct EdgeDescriptor: public EqualityComparableFacade<EdgeDescriptor>
	{
		VertexDescriptor source, target;
		EdgeDescriptor(): source(-1), target(-1) {}
		EdgeDescriptor(VertexDescriptor s, VertexDescriptor t): source(s), target(t) {}
		bool operator== (EdgeDescriptor o) const
		{
			return source == o.source && target == o.target;
		}
	};


	template<typename Property>
	struct HasProperty
	{
		Property p;
		void set(const Property &_p) { p = _p; }
	};
	template<> struct HasProperty<NoProperty>
	{ void set(const NoProperty &_p) {  } };


	template<typename VP>
	struct VertexDataList
	{
		std::vector<VP> v;
		void assignVertex(SizeType _n, const VP &vp)
		{
			v.assign(_n, vp);
		}
		SizeType vertexNumber() const
		{
			return v.size();
		}
	};
	template<> struct VertexDataList<NoProperty>
	{
		SizeType n;
		void assignVertex(SizeType _n, const NoProperty &vp)
		{
			n = _n;
		}
		SizeType vertexNumber() const
		{
			return n;
		}
	};
	template<typename T>
	static T progressionSum(T n)
	{
		if(n & 1) return ((n + 1) >> 1) * n;
		else return (n >> 1) * (n + 1) ;
	}
	template<typename EP>
	struct EdgeData: public HasProperty<EP>
	{
		bool exist = 0;
	};

	template<typename VP, typename EP, typename GP>
	struct GraphData: public VertexDataList<VP>,
					  public HasProperty<GP>
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
    };

    template<typename VP, typename EP, typename GP>
	struct DistinguishDirectionGraph<UndirectedGraphTag, VP, EP, GP>: public GraphData<VP, EP, GP>
	{
    	SizeType edToId(EdgeDescriptor e)  const // ensure e.source >= e.target
		{
    		SizeType s = e.source, t = e.target; // return (s + 1) * s / 2 + t
    		if(s < t) std::swap(s, t);
    		return progressionSum(s) + t;
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
			Base::e.assign(Base::edToId(EdgeDescriptor(n - 1, n - 1)) + 1, EdgeData<EP>());
		}
	};




	}


	template<typename Direction = DirectedGraphTag, typename VP = NoProperty,
			 typename EP = NoProperty, typename GP = NoProperty>
	class AdjacencyMatrix:private AdjacencyMatrixPrivate::Graph<
						  Direction, VP, EP, GP>
	{
		using Base = AdjacencyMatrixPrivate::Graph<
					 Direction, VP, EP, GP>;
		using EdgeData = AdjacencyMatrixPrivate::EdgeData<EP>;
		using SizeType = AdjacencyMatrixPrivate::SizeType;

	public:
		using DirectedCategory = Direction;
		using VertexProperties = VP;
		using EdgeProperties = EP;
		using GraphProperties = GP;

		using VertexDescriptor = AdjacencyMatrixPrivate::VertexDescriptor ;
		using EdgeDescriptor = AdjacencyMatrixPrivate::EdgeDescriptor ;

		explicit AdjacencyMatrix(SizeType n = 0, const VP & vp = VP())
		{
			Base::assignVertex(n, vp);
		}
		void assignVertex(SizeType n, const VP &vp = VP())
		{
			Base::assignVertex(n, vp);
		}
		EdgeDescriptor addEdge(VertexDescriptor a, VertexDescriptor b, const EP &ep = EP())
		{
			EdgeDescriptor ed(a, b);
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
			EdgeDescriptor ed(a, b);
			return std::make_pair(ed, this->edgeData(ed).exist);
		}
		VertexDescriptor source(EdgeDescriptor ed) const { return ed.source; }
		VertexDescriptor target(EdgeDescriptor ed) const { return ed.target; }
		const GP& graphProperties() const { return this->p; }
		GP& graphProperties() { return this->p; }


		const VP& vertexProperties(VertexDescriptor u) const { return this->v[u].p; }
		VP& vertexProperties(VertexDescriptor u) { return this->v[u].p; }

		const EP& edgeProperties(EdgeDescriptor ed) const { return this->edgeData(ed).p; }
		EP& edgeProperties(EdgeDescriptor ed) { return this->edgeData(ed).p; }



	};




































} // namespace lz




#endif // ADJACENCY_MATRIX_H
