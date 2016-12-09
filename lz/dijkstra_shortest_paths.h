#ifndef DIJKSTRA_SHORTEST_PATHS_H
#define DIJKSTRA_SHORTEST_PATHS_H

#include <vector>
#include <algorithm>
#include <tuple>

#include "lz/heap.h"
#include "lz/parameter.h"
#include "lz/graph_utility.h"
#include "lz/std_utility.h"

namespace lz {

using namespace std;
namespace DijkstraShortestPathsKeywords {

    LZ_PARAMETER_KEYWORD(tag, weightMap)
    LZ_PARAMETER_KEYWORD(tag, vertexIndexMap)

	LZ_PARAMETER_KEYWORD(tag, distanceMap)
	LZ_PARAMETER_KEYWORD(tag, distanceCombine)
	LZ_PARAMETER_KEYWORD(tag, distanceCompare)
	LZ_PARAMETER_KEYWORD(tag, distanceInf)
	LZ_PARAMETER_KEYWORD(tag, distanceZero)

    LZ_PARAMETER_KEYWORD(tag, heap)
    LZ_PARAMETER_KEYWORD(tag, edgeRelaxed)
}

/*
 *
 * G must be a VertexListGraph and IncidenceGraph
 */
template<typename G, typename Params = EmptyParamPack >
void dijkstraShortestPaths(const G &g, typename G::VertexDescriptor startVertex, const Params &params = EmptyParamPack())
{
	namespace Keys = DijkstraShortestPathsKeywords;

	using VertexDescriptor = typename GraphTraits<G>::VertexDescriptor;
	using EdgeDescriptor = typename GraphTraits<G>::EdgeDescriptor;
	using VertexIterator = typename GraphTraits<G>::VertexIterator;
	using OutEdgeIterator = typename GraphTraits<G>::OutEdgeIterator;

	auto indexMap = params[Keys::vertexIndexMap | g.vertexPropertyMap(vertexIndexTag)];
	auto weightMap = params[Keys::weightMap | g.edgePropertyMap(edgeWeightTag)];

	using WeightType = typename MapTraits<decltype(weightMap)>::ValueType;
	typename G::VerticesNumberType n = g.verticesNumber();

    auto distanceMap = params[Keys::distanceMap || calculateVertexIndexComposeMap<WeightType>(indexMap, n)];
    using DistanceType = typename MapTraits<decltype(distanceMap)>::ValueType;

    auto distanceCombine = params[Keys::distanceCombine | std::plus<DistanceType>()];
    auto distanceLess = params[Keys::distanceCompare | std::less<DistanceType>()];
    auto distanceInf = params[Keys::distanceInf | std::numeric_limits<DistanceType>::max()];
    auto distanceZero = params[Keys::distanceZero | DistanceType()];

    std::pair<VertexIterator, VertexIterator> vertices = g.vertices();

    auto calculateDefaultHeap = [&]() {
        auto heapIndexMap = makeComposeMap(indexMap, SharedArrayMap<std::size_t>(n, -1));

        auto heapLess = [&](const VertexDescriptor &x, const VertexDescriptor &y) {
            return distanceLess(distanceMap[y], distanceMap[x]);
        };
        using DefaultHeap = lz::IndexableHeap<VertexDescriptor, decltype(heapIndexMap),  std::size_t(-1), decltype(heapLess)>;
        DefaultHeap heap(heapIndexMap, heapLess);
        return std::move(heap);
    };

    auto heap = params[Keys::heap || calculateDefaultHeap];
    auto edgeRelexed = params[Keys::edgeRelaxed | [&](auto ...){
    }];


    for(auto u: vertices) distanceMap[u] = distanceInf;

	distanceMap[startVertex] = distanceZero;
	heap.push(startVertex);

	while(!heap.isEmpty())
	{
		VertexDescriptor u = heap.top();
		heap.pop();

		for(auto e: g.outEdges(u))
		{
		    VertexDescriptor source = u, target = lz::opposite(g, e, u);
            auto distanceTmp = distanceCombine(distanceMap[source], weightMap[e]);
            if(distanceLess(distanceTmp, distanceMap[target]))
            {
                distanceMap[target] = distanceTmp;
                edgeRelexed(e, u);
                if(!heap.contains(target))
                {
                    heap.push(target);
                }
                else
                {
                    heap.decrease(target);
                }
            }
		}
	}






}




//using std::vector;
//using std::get;
//template<typename Graph, typename Dist, int EdgeKey,
//		 typename Plus, typename Less = std::less<Dist>  >
//class DijkstraShortestPaths
//{
//	vector<int> p;
//	vector<Dist> d;
//	int s;
//	struct More
//	{
//		bool operator()(const Dist &a, const Dist &b)
//		{
//			return Less()(b, a);
//		}
//	};
//public:
//	DijkstraShortestPaths(){}
//	DijkstraShortestPaths(const Graph & g, int source, const Dist &zero_dist, const Dist &inf_dist)
//		{ calculate(g, source, zero_dist, inf_dist); }
//
//	void calculate(const Graph & g, int source, const Dist &zero_dist, const Dist &inf_dist)
//	{
//		s = source;
//		int n = g.vertexNumber();
//		p.assign(n, -1);
//		d.assign(n, inf_dist);
//		d[source] = zero_dist;
//		Heap<Dist, More> h(d);
//		for(int t = 1; t < n; ++ t)
//		{
//			int u = h.heapIdToInputId(0);
//			if(!Less()(d[u], inf_dist)) return ;
//			h.pop();
//			typename Graph::ConstIterator eit;
//			for(eit = g.begin(u);eit != g.end(u); ++ eit)
//			{
//				int to = eit->end;
//				Dist tmp = Plus()(d[u], get<EdgeKey>(eit->property));
//				if(h.inputIdToHeapId(to) != -1 && Less()(tmp, d[to]))
//				{
//					p[to] = u;
//					d[to] = tmp;
//					h.modify(to, tmp);
//				}
//			}
//		}
//	}
//	inline int predecessor(int u) const { return p[u]; }
//	inline Dist distance(int u) const { return d[u]; }
//	inline int source() const { return s; }
//	vector<int> getPath(int u)
//	{
//		vector<int> r;
//		for(;u != -1; u = p[u]) r.push_back(u);
//		std::reverse(r.begin(), r.end());
//		return r;
//	}
//
//};



}

#endif // DIJKSTRA_SHORTEST_PATHS_H
