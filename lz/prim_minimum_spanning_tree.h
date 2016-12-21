/*
 * prim_minimum_spanning_tree.h
 *
 *  Created on: 2016年12月9日
 *      Author: LZ
 */

#ifndef LZ_PRIM_MINIMUM_SPANNING_TREE_H_
#define LZ_PRIM_MINIMUM_SPANNING_TREE_H_

#include "lz/std_utility.h"
#include "lz/graph_utility.h"
#include "lz/utility.h"
#include "lz/map.h"

#include "lz/parameter.h"
#include "lz/breadth_first_search.h"
#include "lz/heap.h"

namespace lz {

namespace PrimMininumSpanningTreePrivate {

} // namesapce PrimMininumSpanningTreePrivate


namespace PrimMininumSpanningTreeKeywords {

    LZ_PARAMETER_KEYWORD(tag, weightMap)
    LZ_PARAMETER_KEYWORD(tag, vertexIndexMap)
    LZ_PARAMETER_KEYWORD(tag, treeEdgeMap)


    LZ_PARAMETER_KEYWORD(tag, weightCompare)

    LZ_PARAMETER_KEYWORD(tag, heap)
    LZ_PARAMETER_KEYWORD(tag, marker)
    LZ_PARAMETER_KEYWORD(tag, rootVertex)


} // namesapce PrimMininumSpanningTreeKeywords


template<typename G, typename Params>
void primMininumSpanningTree(const G &g, const Params &params)
{
    namespace Keys = PrimMininumSpanningTreeKeywords;

    using VertexDescriptor = typename GraphTraits<G>::VertexDescriptor;
    using EdgeDescriptor = typename GraphTraits<G>::EdgeDescriptor;

    auto weightMap = params[Keys::weightMap | edgePropertyMap(g, lz::edgeWeightTag)];
    auto indexMap = params[Keys::vertexIndexMap | vertexPropertyMap(g, lz::vertexIndexTag)];

    using WeightType = typename MapTraits<decltype(weightMap)>::ValueType;

    auto n = verticesNumber(g);
    auto weightLess = params[Keys::weightCompare | std::less<WeightType>() ];
    auto treeEdgeMap = params[Keys::treeEdgeMap | calculateVertexIndexComposeMap<EdgeDescriptor>(indexMap, n)];

    auto getTreeEdgeWeight = [&](VertexDescriptor u) {
             return weightMap[treeEdgeMap[u]];
    };

    auto treeEdgeWeightLess = [&](VertexDescriptor x, VertexDescriptor y) {
        return weightLess(getTreeEdgeWeight(y), getTreeEdgeWeight(x));
    };

    decltype(auto) heap = params[Keys::heap || [&]() {
        auto heapIndexMap = makeVertexIndexComposeMap<size_t>(indexMap, n, (std::size_t)-1);
        using DefaultHeap = IndexableHeap<VertexDescriptor, decltype(heapIndexMap),  std::size_t(-1), decltype(treeEdgeWeightLess)>;
        return DefaultHeap(heapIndexMap, treeEdgeWeightLess);
    }];

    decltype(auto) _marker = params[Keys::marker || [&]() {
        return IndexMarker<decltype(indexMap)>(indexMap, n);
    }];

    auto _vertices = vertices(g);
    auto rootVertex = params[ Keys::rootVertex | *_vertices.first];

    breadthFirstSearch(g, (

        BreadthFirstSearchKeywords::startVertex = rootVertex,
        BreadthFirstSearchKeywords::buffer = heap,
        BreadthFirstSearchKeywords::marker = _marker,
        BreadthFirstSearchKeywords::treeEdge =
            [&](EdgeDescriptor e, VertexDescriptor source, VertexDescriptor target) {
                treeEdgeMap[target] = e;
            },
        BreadthFirstSearchKeywords::notTreeEdge =
            [&](EdgeDescriptor e, VertexDescriptor source, VertexDescriptor target) {

                if(target == rootVertex) return ;
                if(weightLess(weightMap[e], weightMap[treeEdgeMap[target]]))
                {
                    treeEdgeMap[target] = e;
                    heap.decrease(target);
                }
            }
        ));










}








































} // namesapce lz

#endif /* LZ_PRIM_MINIMUM_SPANNING_TREE_H_ */
