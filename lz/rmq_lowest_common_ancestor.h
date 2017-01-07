/*
 * rmq_lowest_common_ancestor.h
 *
 *  Created on: 2017年1月7日
 *      Author: LZ
 */

#ifndef LZ_RMQ_LOWEST_COMMON_ANCESTOR_H_
#define LZ_RMQ_LOWEST_COMMON_ANCESTOR_H_

#include "lz/parameter.h"
#include "lz/utility.h"
#include "lz/std_utility.h"
#include "lz/graph_utility.h"
#include "lz/depth_first_search.h"
#include <lz/sparse_table_range_minimum_query.h>

namespace lz {

    namespace RmqLowestCommonAncestorKeywords {

    LZ_PARAMETER_KEYWORD(tag, vertexIndexMap)
    LZ_PARAMETER_KEYWORD(tag, rootVertex)
    LZ_PARAMETER_KEYWORD(tag, eulerTourMap)
    LZ_PARAMETER_KEYWORD(tag, eulerTour)
    LZ_PARAMETER_KEYWORD(tag, depthMap)
    LZ_PARAMETER_KEYWORD(tag, rmqAdapter)

    } // namespace RmqLowestCommonAncestorKeywords

    namespace RmqLowestCommonAncestorPrivate {

    struct RmqAdapter
    {
        template<typename RandomIterator, typename Less>
        auto operator()(RandomIterator begin, RandomIterator end, Less less)
        {
            return sparseTableRangeMinimumQuery(begin, end, less);
        }
    };



    }

template<typename Tree, typename Params = EmptyParamPack>
auto rmqLowestCommonAncestor(const Tree& tree, const Params& params = Params())
{
    using VertexDescriptor = typename GraphTraits<Tree>::VertexDescriptor;
    namespace Keys = RmqLowestCommonAncestorKeywords;

    auto n = verticesNumber(tree);
    auto indexMap = params[Keys::vertexIndexMap | vertexPropertyMap(tree, vertexIndexTag)];
    auto rootVertex = params[Keys::rootVertex | *vertices(tree).first];
    auto rmpAdapter = params[Keys::rmqAdapter | RmqLowestCommonAncestorPrivate::RmqAdapter()];
    auto eulerTourMap = params[Keys::eulerTourMap || calculateVertexIndexComposeMap<size_t>(indexMap, n)];
    auto depthMap = params[Keys::depthMap || calculateVertexIndexComposeMap<size_t>(indexMap, n)];

    vector<VertexDescriptor> eulerTour;
    depthMap[rootVertex] = 0;
    eulerTourMap[rootVertex] = eulerTour.size();
    eulerTour.push_back(rootVertex);

    adjacencyTreeDFS(tree, (
        DepthFirstSearchKeywords::enterVertex = rootVertex,
        DepthFirstSearchKeywords::treeEdge =
            [&](VertexDescriptor u, VertexDescriptor to)
            {
                depthMap[to] = depthMap[u] + 1;

                eulerTourMap[to] = eulerTour.size();
                eulerTour.push_back(to);
            },
        DepthFirstSearchKeywords::treeEdgeReturn =
            [&](VertexDescriptor u, VertexDescriptor to)
            {
                eulerTour.push_back(u);
            }
    ));

    auto _vertices = vertices(tree);
    auto less = [=](VertexDescriptor a, VertexDescriptor b) {
            return depthMap[a] < depthMap[b];
        };
    auto rmqQuery = rmpAdapter(eulerTour.begin(), eulerTour.end(), less);
    return [=](VertexDescriptor a, VertexDescriptor b) {

        auto ida = eulerTourMap[a];
        auto idb = eulerTourMap[b];
        if(ida > idb) std::swap(ida, idb);
        return rmqQuery(ida, idb + 1);

    };



}

} // namespace lz

#endif /* LZ_RMQ_LOWEST_COMMON_ANCESTOR_H_ */
