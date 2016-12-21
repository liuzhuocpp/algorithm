/*
 * breadth_first_search.h
 *
 *  Created on: 2016年12月10日
 *      Author: LZ
 */

#ifndef LZ_BREADTH_FIRST_SEARCH_H_
#define LZ_BREADTH_FIRST_SEARCH_H_


#include "lz/utility.h"
#include "lz/graph_utility.h"
#include "lz/map.h"
#include "lz/parameter.h"
#include "lz/std_utility.h"

namespace lz {


namespace BreadthFirstSearchKeywords {


    LZ_PARAMETER_KEYWORD(tag, discoverVertex)
    LZ_PARAMETER_KEYWORD(tag, examineVertex)
    LZ_PARAMETER_KEYWORD(tag, examineEdge)
    LZ_PARAMETER_KEYWORD(tag, treeEdge)
    LZ_PARAMETER_KEYWORD(tag, notTreeEdge)
    LZ_PARAMETER_KEYWORD(tag, finishEdge)
    LZ_PARAMETER_KEYWORD(tag, finishVertex)


    LZ_PARAMETER_KEYWORD(tag, startVertex)
    LZ_PARAMETER_KEYWORD(tag, vertexIndexMap)
    LZ_PARAMETER_KEYWORD(tag, marker)
    LZ_PARAMETER_KEYWORD(tag, buffer)


} // namespace BreadthFirstSearchKeywords

/*

G must model a IncidenceGraph, VertexListGraph

 */
template<typename G, typename Params>
void breadthFirstSearch(const G &g, const Params& params)
{
    using VertexDescriptor = typename G::VertexDescriptor;
    using EdgeDescriptor = typename G::EdgeDescriptor;

    namespace Keys = BreadthFirstSearchKeywords;

    auto n = verticesNumber(g);

    auto indexMap = params[Keys::vertexIndexMap | vertexPropertyMap(g, vertexIndexTag)];
    decltype(auto) marker = params[Keys::marker || [&]() {
        return std::move(lz::IndexMarker<decltype(indexMap) >(indexMap, n) );
    }];

    decltype(auto) buffer = params[Keys::buffer || [](){
        return std::move(Queue<VertexDescriptor>());
    }];

    auto discoverVertex = params[Keys::discoverVertex | emptyFunction];
    auto examineVertex = params[Keys::examineVertex | emptyFunction];
    auto examineEdge = params[Keys::examineEdge | emptyFunction];
    auto treeEdge = params[Keys::treeEdge | emptyFunction];
    auto notTreeEdge = params[Keys::notTreeEdge | emptyFunction];
    auto finishEdge = params[Keys::finishEdge | emptyFunction];
    auto finishVertex = params[Keys::finishVertex | emptyFunction];

    // Can be more efficient
    auto startVertex = params[Keys::startVertex | GraphTraits<G>::nullVertex()];

    if(startVertex != GraphTraits<G>::nullVertex())
    {
        marker.mark(startVertex);
        buffer.push(startVertex);
        discoverVertex(startVertex);
    }

    while(!buffer.empty())
    {
        VertexDescriptor u = buffer.top();
        buffer.pop();
        examineVertex(u);
        for(auto e: outEdges(g, u))
        {
            examineEdge(e, u);
            VertexDescriptor target = opposite(g, e, u);
            if(!marker.isMark(target))
            {
                treeEdge(e, u, target);
                marker.mark(target);
                buffer.push(target);
                discoverVertex(target);
            }
            else
            {
                notTreeEdge(e, u, target);
            }
            finishEdge(e, u, target);
        }
        finishVertex(u);
    }
}













} // namesapce lz





#endif /* LZ_BREADTH_FIRST_SEARCH_H_ */
