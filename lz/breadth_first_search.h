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

    LZ_PARAMETER_KEYWORD(tag, initializeVertex)
    LZ_PARAMETER_KEYWORD(tag, startVertex)
    LZ_PARAMETER_KEYWORD(tag, discoverVertex)
    LZ_PARAMETER_KEYWORD(tag, examineEdge)
    LZ_PARAMETER_KEYWORD(tag, treeEdge)
    LZ_PARAMETER_KEYWORD(tag, treeEdgeReturn)
    LZ_PARAMETER_KEYWORD(tag, notTreeEdge)
    LZ_PARAMETER_KEYWORD(tag, finishEdge)
    LZ_PARAMETER_KEYWORD(tag, finishVertex)

    LZ_PARAMETER_KEYWORD(tag, colorMap)
    LZ_PARAMETER_KEYWORD(tag, white)
    LZ_PARAMETER_KEYWORD(tag, black)
    LZ_PARAMETER_KEYWORD(tag, vertexIndexMap)
    LZ_PARAMETER_KEYWORD(tag, buffer)


} // namespace BreadthFirstSearchKeywords

/*
G must model a IncidenceGraph, VertexListGraph

 */
template<typename G, typename Params>
void breadthFirstSearch(const G &g, typename G::VertexDescriptor startVertex,  const Params& params)
{
    using VertexDescriptor = typename G::VertexDescriptor;
    using EdgeDescriptor = typename G::EdgeDescriptor;
    namespace Keys = BreadthFirstSearchKeywords;
    auto black = params[Keys::black];
    auto white = params[Keys::black];
    auto colorMap = params[Keys::black];
    auto buffer = params[Keys::buffer];

    using ColorType = typename MapTraits<decltype(colorMap)>::ValueType;
    for(auto u: g.vertices())
    {
        colorMap[u] = white();
    }

    colorMap[startVertex] = black();
    buffer.push(startVertex);
    while(!buffer.empty())
    {
        VertexDescriptor u = buffer.top();
        buffer.pop();
        for(auto e: g.outEdges(u))
        {
            VertexDescriptor target = opposit(g, e, u);
            if(colorMap[target] == white())
            {
                colorMap[target] = black();
                buffer.push(target);
            }
        }
    }
}













} // namesapce lz





#endif /* LZ_BREADTH_FIRST_SEARCH_H_ */
