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
void kruskalMinimumSpanningTree(const G &g, const ParamPack &params)
{
	namespace Keys = KruskalMinimumSpanningTreeKeywords;
	using VertexDescriptor = typename GraphTraits<G>::VertexDescriptor;
	using EdgeDescriptor = typename GraphTraits<G>::EdgeDescriptor;
	using VertexIterator = typename GraphTraits<G>::VertexIterator;
	using EdgeIterator = typename GraphTraits<G>::EdgeIterator;

	typename GraphTraits<G>::VertexSizeType n = g.vertexNumber();

	auto indexMap = params[Keys::vertexIndexMap | g.vertexPropertyMap(VertexIndexTag()) ];

	auto disjointSets = params[Keys::disjointSets || std::bind(
			[&]()
			{
				return DisjointSets<
						decltype(makeVertexIndexComposeMap<VertexDescriptor>(indexMap, n))>
						(makeVertexIndexComposeMap<VertexDescriptor>(indexMap, n))  ;
			}
	)];

	using DisjointSets = std::decay_t<decltype(disjointSets)>;

//	auto disjointSetsImplement = p[Keys::disjointSetsImplement | std::declval<DisjointSetsImplement<DisjointSets>>() ];
	auto disjointSetsImplement = params[Keys::disjointSetsImplement | DisjointSetsImplement<DisjointSets>() ];

	auto weightMap = params[Keys::weightMap | g.edgePropertyMap(EdgeWeightTag())];

	using WeightType = typename MapTraits<decltype(weightMap)>::ValueType;
	auto weightCompare = params[Keys::weightCompare | std::less<WeightType>()  ];

	std::pair<VertexIterator, VertexIterator> vi = g.vertices();
	for(;vi.first != vi.second; vi.first ++)
		disjointSetsImplement.makeSet(disjointSets, *vi.first);
	std::pair<EdgeIterator, EdgeIterator> ei = g.edges();
	std::vector<EdgeDescriptor> edges(ei.first, ei.second);

	std::sort(edges.begin(), edges.end(),
			[&](EdgeDescriptor a, EdgeDescriptor b)->bool
			{
				return weightCompare(weightMap[a], weightMap[b]) ;
			}
	);

	for(EdgeDescriptor e: edges)
	{
		VertexDescriptor a = g.source(e), b = g.target(e);
		VertexDescriptor ra = disjointSetsImplement.findSet(disjointSets, a), rb = disjointSetsImplement.findSet(disjointSets, b);

		if(ra != rb)
		{
			params[Keys::discoverTreeEdge](e);
			disjointSetsImplement.link(disjointSets, ra, rb);
		}
	}
}


}// namesapce lz

#endif /* LZ_KRUSKAL_MINIMUM_SPANNING_TREE_H_ */

