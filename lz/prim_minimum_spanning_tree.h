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
#include "lz/parameter.h"

namespace lz {

namespace PrimMininumSpanningTreePrivate {

} // namesapce PrimMininumSpanningTreePrivate


namespace PrimMininumSpanningTreeKeywords {

    LZ_PARAMETER_KEYWORD(tag, weightMap)
    LZ_PARAMETER_KEYWORD(tag, vertexIndexMap)
    LZ_PARAMETER_KEYWORD(tag, lightEdgeMap)


    LZ_PARAMETER_KEYWORD(tag, weightCompare)
    LZ_PARAMETER_KEYWORD(tag, weightInf)
    LZ_PARAMETER_KEYWORD(tag, weightZero)

    LZ_PARAMETER_KEYWORD(tag, heap)
    LZ_PARAMETER_KEYWORD(tag, discoverTreeEdge)


} // namesapce PrimMininumSpanningTreeKeywords


template<typename G, typename Params>
void primMininumSpanningTree(const G &g, const Params &params)
{
    namespace Keys = PrimMininumSpanningTreeKeywords;

    using VertexDescriptor = typename GraphTraits<G>::VertexDescriptor;
    using EdgeDescriptor = typename GraphTraits<G>::EdgeDescriptor;
    using VertexIterator = typename GraphTraits<G>::VertexIterator;
    using OutEdgeIterator = typename GraphTraits<G>::OutEdgeIterator;


    auto weightMap = Params[Keys::weightMap];
    auto vertexIndexMap = Params[Keys::vertexIndexMap];
    auto weightLess = Params[Keys::weightCompare];
    auto weightInf = Params[Keys::weightInf];
    auto weightZero = Params[Keys::weightZero];
    auto heap = Params[Keys::heap];
    auto discoverTreeEdge = Params[Keys::discoverTreeEdge];
    auto lightEdgeMap = Params[Keys::lightEdgeMap];



    std::pair<VertexIterator, VertexIterator> vertices = g.vertices();
    auto startVertex = *vertices.first;

    for(auto u : vertices)
    {
        lightEdgeMap[u] = nullEdge;
    }

    for(auto e: g.outEdges(startVertex))
    {
        VertexDescriptor source = startVertex, target = opposite(g, e, source);
        lightEdgeMap[target] = e;
        heap.push(target);
    }




    while(!heap.empty())
    {
        VertexDescriptor u = heap.top();
        heap.pop();
        for(auto e: g.outEdges(u))
        {
            VertexDescriptor source = startVertex, target = opposite(g, e, source);
            if(lightEdgeMap[target] == nullEdge)
            {
                lightEdgeMap[target] = e;
                heap.push(target);
            }
            else if(weightLess(weightMap[e], weightMap[lightEdgeMap[target]]) && heap.contains(target))
            {
                lightEdgeMap[target] = e;
                heap.decrease(target);
            }
        }
    }








}








































} // namesapce lz

#endif /* LZ_PRIM_MINIMUM_SPANNING_TREE_H_ */
