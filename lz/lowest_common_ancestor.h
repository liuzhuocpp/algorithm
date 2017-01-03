/*
 * lowest_common_ancestor.h
 *
 *  Created on: 2017年1月3日
 *      Author: LZ
 */

#ifndef LZ_LOWEST_COMMON_ANCESTOR_H_
#define LZ_LOWEST_COMMON_ANCESTOR_H_

#include "lz/parameter.h"
#include "lz/utility.h"
#include "lz/std_utility.h"
#include "lz/graph_utility.h"
#include "lz/depth_first_search.h"
#include "lz/disjoint_sets.h"

namespace lz {



    namespace LowestCommonAncestorKeywords {

    LZ_PARAMETER_KEYWORD(tag, marker)
    LZ_PARAMETER_KEYWORD(tag, vertexIndexMap)
    LZ_PARAMETER_KEYWORD(tag, disjointSets)
    LZ_PARAMETER_KEYWORD(tag, rootVertex)
//    LZ_PARAMETER_KEYWORD(tag, lcaMap)


    } // namespace LowestCommonAncestor



template<typename Tree, typename QueryGraph, typename LcaMap, typename Params = EmptyParamPack>
void lowestCommonAncestor(const Tree& tree, const QueryGraph &queryGraph, const LcaMap& lcaMap, const Params& params = Params())
{
    using VertexDescriptor = typename GraphTraits<Tree>::VertexDescriptor;
    namespace Keys = LowestCommonAncestorKeywords;

    auto n = verticesNumber(tree);
    auto indexMap = params[Keys::vertexIndexMap | vertexPropertyMap(tree, vertexIndexTag)];

    auto rootVertex = params[Keys::rootVertex | *vertices(tree).first];

    decltype(auto) marker = params[Keys::marker || [&]() {
        return IndexMarker<decltype(indexMap)>(indexMap, n);
    }];

    decltype(auto) disjointSets = params[Keys::disjointSets || [&]() {
        auto parentMap = lz::makeVertexIndexComposeMap<VertexDescriptor>(indexMap, n);
        auto _vertices = vertices(tree);
        return DisjointSets<decltype(parentMap)>(parentMap, _vertices.first, _vertices.second);
    }];

    adjacencyDFS(tree, (

        DepthFirstSearchKeywords::enterVertex = rootVertex,
        DepthFirstSearchKeywords::treeEdgeReturn = [&](VertexDescriptor u, VertexDescriptor to) {
            disjointSets.link(to, u);
        },

        DepthFirstSearchKeywords::finishVertex = [&](VertexDescriptor u) {
            marker.mark(u);
            using QueryVertexDescriptor = typename GraphTraits<QueryGraph>::VertexDescriptor;

            for(auto queryEdge: outEdges(queryGraph, u))
            {
                QueryVertexDescriptor to = opposite(queryGraph, queryEdge, u);
                if(marker.isMark(to))
                {
                    lcaMap[queryEdge] = disjointSets.findSet(to);
                }
            }

        }));


}









}// namesapce lz

#endif /* LZ_LOWEST_COMMON_ANCESTOR_H_ */
