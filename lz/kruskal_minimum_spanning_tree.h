/*
 * kruskal_minimum_spanning_tree.h
 *
 *  Created on: 2015��12��23��
 *      Author: LZ
 */

#ifndef LZ_KRUSKAL_MINIMUM_SPANNING_TREE_H_
#define LZ_KRUSKAL_MINIMUM_SPANNING_TREE_H_

#include "lz/parameter.h"
#include "lz/disjoint_sets.h"
#include "lz/utility.h"
#include "lz/graph_utility.h"
//#include<>
#include <bits/stdc++.h>



namespace lz {

	namespace KruskalMinimumSpanningTreeKeywords{


	LZ_PARAMETER_KEYWORD(tag, disjointSets)
	LZ_PARAMETER_KEYWORD(tag, disjointSetsImplement)
	LZ_PARAMETER_KEYWORD(tag, vertexIndexMap)
	LZ_PARAMETER_KEYWORD(tag, weightMap)
	LZ_PARAMETER_KEYWORD(tag, weightCompare)
	LZ_PARAMETER_KEYWORD(tag, discoverTreeEdge)

	} // namespace KruskalMinimumSpanningTreeKeywords



template<typename G, typename ParamPack>
void kruskalMinimumSpanningTree(const G &g, const ParamPack &p)
{
	namespace Keys = KruskalMinimumSpanningTreeKeywords;

	using Vertex = typename GraphTraits<G>::VertexDescriptor;
	using Edge = typename GraphTraits<G>::EdgeDescriptor;
	auto n = g.vertexNumber();

	auto indexMap = p[Keys::vertexIndexMap | g.vertexPropertyMap(VertexIndexTag()) ];

	auto disjointSets = p[Keys::disjointSets || std::bind(

			[&]()
			{
				return DisjointSets<
						decltype(makeVertexIndexComposeMap<Vertex>(indexMap, n))>
						(makeVertexIndexComposeMap<Vertex>(indexMap, n))  ;
			}
	)];

	using DisjointSets = typename std::decay<decltype(disjointSets)>::type;

//	auto disjointSetsImplement = p[Keys::disjointSetsImplement | std::declval<DisjointSetsImplement<DisjointSets>>() ];
	auto disjointSetsImplement = p[Keys::disjointSetsImplement | DisjointSetsImplement<DisjointSets>() ];

	auto weightMap = p[Keys::weightMap | g.edgePropertyMap(EdgeWeightTag())];

	using WeightType = typename MapTraits<decltype(weightMap)>::ValueType;
	auto weightCompare = p[Keys::weightCompare | std::less<WeightType>()  ];

	auto vi = g.vertices();
	for(;vi.first != vi.second; vi.first ++)
		disjointSetsImplement.makeSet(disjointSets, *vi.first);
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
		Vertex ra = disjointSetsImplement.findSet(disjointSets, a), rb = disjointSetsImplement.findSet(disjointSets, b);

		if(ra != rb)
		{
			p[Keys::discoverTreeEdge](e);
			disjointSetsImplement.link(disjointSets, ra, rb);
		}
	}
}


}// namesapce lz

#endif /* LZ_KRUSKAL_MINIMUM_SPANNING_TREE_H_ */

