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
//    LZ_PARAMETER_KEYWORD(tag, weightInf)
//    LZ_PARAMETER_KEYWORD(tag, weightZero)

    LZ_PARAMETER_KEYWORD(tag, heap)
    LZ_PARAMETER_KEYWORD(tag, rootVertex)


} // namesapce PrimMininumSpanningTreeKeywords


template<typename G, typename Params>
void primMininumSpanningTree(const G &g, const Params &params)
{
    namespace Keys = PrimMininumSpanningTreeKeywords;

    using VertexDescriptor = typename GraphTraits<G>::VertexDescriptor;
    using EdgeDescriptor = typename GraphTraits<G>::EdgeDescriptor;

    auto weightMap = params[Keys::weightMap | g.edgePropertyMap(lz::edgeWeightTag)];
    auto indexMap = params[Keys::vertexIndexMap | g.vertexPropertyMap(lz::vertexIndexTag)];

    using WeightType = typename MapTraits<decltype(weightMap)>::ValueType;

    auto n = g.verticesNumber();
    auto weightLess = params[Keys::weightCompare | std::less<WeightType>() ];
    auto treeEdgeMap = params[Keys::treeEdgeMap | calculateVertexIndexComposeMap<EdgeDescriptor>(indexMap, n)];

    auto getTreeEdgeWeight = [&](VertexDescriptor u) {
        if(treeEdgeMap[u] == G::nullEdge())
             return WeightType();
         else
             return weightMap[treeEdgeMap[u]];
     };
    auto treeEdgeWeightLess = [&](VertexDescriptor x, VertexDescriptor y) {
        return weightLess(getTreeEdgeWeight(y), getTreeEdgeWeight(x));
    };


    auto heap = params[Keys::heap || [&]() {
        auto heapIndexMap = makeVertexIndexComposeMap<size_t>(indexMap, n, (std::size_t)-1);
        using DefaultHeap = lz::IndexableHeap<VertexDescriptor, decltype(heapIndexMap),  std::size_t(-1), decltype(treeEdgeWeightLess)>;
        return DefaultHeap(heapIndexMap, treeEdgeWeightLess);
    }];


    auto vertices = g.vertices();
    auto rootVertex = params[ Keys::rootVertex | *vertices.first];

    for(auto u : vertices)
    {
        treeEdgeMap[u] = G::nullEdge();
    }

    struct Marker
    {
        decltype(treeEdgeMap) treeEdgeMap;
        VertexDescriptor rootVertex;
        bool isMark(VertexDescriptor u) const
        {
            return (u == rootVertex) || (treeEdgeMap[u] != G::nullEdge());
        }
        void mark(VertexDescriptor u)
        {
            // do nothing
        }
    }marker{treeEdgeMap, rootVertex};


    breadthFirstSearch(g, rootVertex, (
            BreadthFirstSearchKeywords::buffer = heap,
            BreadthFirstSearchKeywords::marker = marker,
            BreadthFirstSearchKeywords::treeEdge =
                [&](EdgeDescriptor e, VertexDescriptor source, VertexDescriptor target) {
                    treeEdgeMap[target] = e;
                },
            BreadthFirstSearchKeywords::notTreeEdge =
                [&](EdgeDescriptor e, VertexDescriptor source, VertexDescriptor target) {
                    if(target != rootVertex && weightLess(weightMap[e], weightMap[treeEdgeMap[target]]))
                    {
                        treeEdgeMap[target] = e;
                        heap.decrease(target);
                    }
                }
            ));










}








































} // namesapce lz

#endif /* LZ_PRIM_MINIMUM_SPANNING_TREE_H_ */
