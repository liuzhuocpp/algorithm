/*
 * kruskal_minimum_spanning_tree.h
 *
 *  Created on: 2015Äê12ÔÂ23ÈÕ
 *      Author: LZ
 */

#ifndef LZ_KRUSKAL_MINIMUM_SPANNING_TREE_H_
#define LZ_KRUSKAL_MINIMUM_SPANNING_TREE_H_

#include "lz/parameter.h"
#include "lz/disjoint_sets.h"
#include "lz/utility.h"
#include "lz/graph_utility.h"

#include <bits/stdc++.h>



namespace lz {

	namespace KruskalMinimumSpanningTreeKeywords{

	LZ_PARAMETER_KEYWORD(tag, vertexIndexMap)
	LZ_PARAMETER_KEYWORD(tag, weightMap)
	LZ_PARAMETER_KEYWORD(tag, disjointSets)
	LZ_PARAMETER_KEYWORD(tag, weightCompare)




	} // namespace KruskalMinimumSpanningTreeKeywords



template<typename G, typename Function, typename ParamPack = EmptyParamPack>
void kruskalMinimumSpanningTree(const G &g, Function found_tree, const ParamPack &p = EmptyParamPack())
{


	namespace k = KruskalMinimumSpanningTreeKeywords;
	using Vertex = typename GraphTraits<G>::VertexDescriptor;
	using Edge = typename GraphTraits<G>::EdgeDescriptor;
	auto n = g.vertexNumber();

	auto indexMap = p[k::vertexIndexMap | g.vertexPropertyMap(VertexIndexTag()) ];

	auto disjointSets = p[k::disjointSets || std::bind(

			[&]()
			{
				return DisjointSets<decltype(makeVertexIndexComposeMap<Vertex>(indexMap, n))>
						(makeVertexIndexComposeMap<Vertex>(indexMap, n));
			}
	)];
	auto weightMap = p[k::weightMap | g.edgePropertyMap(EdgeWeightTag())];

	using WeightType = typename MapTraits<decltype(weightMap)>::ValueType;
	auto weightCompare = p[k::weightCompare | std::less<WeightType>()  ];

	auto vi = g.vertices();
	for(;vi.first != vi.second; vi.first ++)
		disjointSets.makeSet(*vi.first);
	auto ei = g.edges();

	std::vector<Edge> edges;
	edges.reserve(g.edgeNumber());
	for(auto e = ei.first; e != ei.second; ++ e)
	{
		edges.push_back(*e);
	}

	std::sort(edges.begin(), edges.end(),
			[&](Edge a, Edge b)->bool
			{
				return weightCompare(weightMap[a], weightMap[b]) ;
			}
	);

	for(auto e: edges)
	{
		Vertex a = g.source(e), b = g.target(e);
		Vertex ra = disjointSets.findSet(a), rb = disjointSets.findSet(b);

		if(ra != rb)
		{
			found_tree(e);
			disjointSets.link(ra, rb);
		}
	}
}


}// namesapce lz

#endif /* LZ_KRUSKAL_MINIMUM_SPANNING_TREE_H_ */

