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
#include <bits/stdc++.h>
#include "lz/std_utility.h"


namespace lz {

	namespace KruskalMinimumSpanningTreeKeywords{

/*

disjointSets: DisjointSets concept
vertexIndexMap: readable, the key type should be VertexDescriptor, the value type should be integer type
weightMap: readable , the key type should be EdgeDescriptor.
WeightCompare: the binary function that compare the two the vertex distance
discoverTreeEdge: event visitor function containing one paramaters,
    e the spanning tree edge discriptor


Maybe make the function more generic by make user can user their own sort function


 */


//	LZ_PARAMETER_KEYWORD(tag, sort)

	LZ_PARAMETER_KEYWORD(tag, disjointSets)
	LZ_PARAMETER_KEYWORD(tag, vertexIndexMap)
	LZ_PARAMETER_KEYWORD(tag, weightMap)
	LZ_PARAMETER_KEYWORD(tag, weightCompare)
	LZ_PARAMETER_KEYWORD(tag, discoverTreeEdge)

	} // namespace KruskalMinimumSpanningTreeKeywords


/*

    Must be a VertexListGraph and EdgeListGraph

 */
template<typename G, typename ParamPack>
void kruskalMinimumSpanningTree(const G &g, const ParamPack &params)
{
	namespace Keys = KruskalMinimumSpanningTreeKeywords;

	using VertexDescriptor = typename GraphTraits<G>::VertexDescriptor;
	using EdgeDescriptor = typename GraphTraits<G>::EdgeDescriptor;
	using VertexIterator = typename GraphTraits<G>::VertexIterator;
	using EdgeIterator = typename GraphTraits<G>::EdgeIterator;

	typename GraphTraits<G>::VerticesNumberType n = g.verticesNumber();

	auto indexMap = params[Keys::vertexIndexMap | g.vertexPropertyMap(vertexIndexTag) ];
	auto calculateDisjointSets = [&](){
	    auto parentMap = lz::makeVertexIndexComposeMap<VertexDescriptor>(indexMap, n);
	    return DisjointSets<decltype(parentMap)>(parentMap);
	};
	auto disjointSets = params[Keys::disjointSets || calculateDisjointSets];
	auto weightMap = params[Keys::weightMap | g.edgePropertyMap(edgeWeightTag)];
	using WeightType = typename MapTraits<decltype(weightMap)>::ValueType;
	auto weightLess = params[Keys::weightCompare | std::less<WeightType>()];

    std::pair<EdgeIterator, EdgeIterator> ei = g.edges();
    std::vector<EdgeDescriptor> edges(ei.first, ei.second);

	std::sort(edges.begin(), edges.end(),
			[&](EdgeDescriptor a, EdgeDescriptor b)->bool
			{
				return weightLess(weightMap[a], weightMap[b]) ;
			}
	);

    for(VertexDescriptor u : g.vertices())
    {
        disjointSets.makeSet(u);
    }

	for(EdgeDescriptor e: edges)
	{
		VertexDescriptor a = g.source(e), b = g.target(e);
		VertexDescriptor ra = disjointSets.findSet(a), rb = disjointSets.findSet(b);
		if(ra != rb)
		{
			params[Keys::discoverTreeEdge](e);
			disjointSets.link(ra, rb);
		}
	}
}


}// namesapce lz

#endif /* LZ_KRUSKAL_MINIMUM_SPANNING_TREE_H_ */

