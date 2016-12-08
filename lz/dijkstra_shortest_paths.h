#ifndef DIJKSTRA_SHORTEST_PATHS_H
#define DIJKSTRA_SHORTEST_PATHS_H

#include <vector>
#include <algorithm>
#include <tuple>

#include "lz/heap.h"
#include "lz/parameter.h"
#include "lz/graph_utility.h"
namespace lz {

namespace DijkstraShortestPathsKeywords {

	LZ_PARAMETER_KEYWORD(tag, heap)

	LZ_PARAMETER_KEYWORD(tag, weightMap)
	LZ_PARAMETER_KEYWORD(tag, distanceMap)
	LZ_PARAMETER_KEYWORD(tag, distanceCombine)
	LZ_PARAMETER_KEYWORD(tag, distanceCompare)
	LZ_PARAMETER_KEYWORD(tag, distanceInf)
	LZ_PARAMETER_KEYWORD(tag, distanceZero)

	LZ_PARAMETER_KEYWORD(tag, vertexIndexMap)
	LZ_PARAMETER_KEYWORD(tag, startVertex)
	LZ_PARAMETER_KEYWORD(tag, colorMap)
	LZ_PARAMETER_KEYWORD(tag, black)
	LZ_PARAMETER_KEYWORD(tag, white)

}

/*
 *
 * G must be a VertexListGraph and IncidenceGraph
 */
template<typename G, typename Params>
void dijkstraShortestPaths(const G &g, const Params &params)
{
	namespace Keys = DijkstraShortestPathsKeywords;

	using VertexDescriptor = typename GraphTraits<G>::VertexDescriptor;
	using EdgeDescriptor = typename GraphTraits<G>::EdgeDescriptor;
	using VertexIterator = typename GraphTraits<G>::VertexIterator;
	using OutEdgeIterator = typename GraphTraits<G>::OutEdgeIterator;

	auto vertexIndexMap = params[Keys::vertexIndexMap];


//	auto heap = params[Keys::heap];
//	auto colorMap = params[Keys::colorMap];
//	auto black = params[Keys::black];
//	auto white = params[Keys::white];
//	auto weightMap = params[Keys::weightMap];
//	auto startVertex = params[Keys::startVertex];
//
//
//
//	auto distanceMap = params[Keys::distanceMap];
//	auto distanceCombine = params[Keys::distanceCombine];
//	auto distanceLess = params[Keys::distanceCompare];
//	auto distanceInf = params[Keys::distanceInf];
//	auto distanceZero = params[Keys::distanceZero];
//
//	std::pair<VertexIterator, VertexIterator> vi = g.vertices();
//
//	for(;vi.first != vi.second; vi.first ++)
//	{
//		colorMap[*vi.first] = white();
//		distanceMap[*vi.first] = distanceInf;
//	}
//	distanceMap[startVertex] = distanceZero;
//	colorMap[startVertex] = black();
//	heap.push(startVetex);
//	while(!heap.empty())
//	{
//		VertexDescriptor u = heap.top();
//		heap.pop();
//		std::pair<OutEdgeIterator, OutEdgeIterator> oei = g.outEdges(u);
//		for(OutEdgeIterator ei = oei.first; ei != oei.second; ++ei)
//		{
//			EdgeDescriptor e = *ei;
//			VertexDescriptor source = u, target = lz::opposite(g, e, u);
//
//			auto distanceTmp = distanceCombine(distanceMap[source], weightMap[e]);
//			if(distanceLess(distanceTmp, distanceMap[target]))
//			{
//				if(colorMap[target] == white())
//				{
//					colorMap[target] = black();
//					heap.push(target);
//				}
//				else
//				{
//					heap.decrease(target, distanceTmp);
//				}
//			}
//		}
//	}






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
