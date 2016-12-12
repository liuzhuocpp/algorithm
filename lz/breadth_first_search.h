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

    LZ_PARAMETER_KEYWORD(tag, marker)
    LZ_PARAMETER_KEYWORD(tag, vertexIndexMap)
    LZ_PARAMETER_KEYWORD(tag, buffer)


} // namespace BreadthFirstSearchKeywords

/*

G must model a IncidenceGraph, VertexListGraph

 */
template<typename G, typename Params>
void breadthFirstSearch(const G &g, typename G::VertexDescriptor s,  const Params& params)
{
    using VertexDescriptor = typename G::VertexDescriptor;
    using EdgeDescriptor = typename G::EdgeDescriptor;

    namespace Keys = BreadthFirstSearchKeywords;

    auto n = g.verticesNumber();

    auto indexMap = params[Keys::vertexIndexMap | g.vertexPropertyMap(vertexIndexTag)];
    auto marker = params[Keys::marker || [&](){
        return std::move(lz::IndexMarker<decltype(indexMap) >(indexMap, n) );
    }];
    auto buffer = params[Keys::buffer || [](){
        return Queue<VertexDescriptor>();
    }];

    auto discoverVertex = params[Keys::discoverVertex | emptyFunction];
    auto examineVertex = params[Keys::examineVertex | emptyFunction];
    auto examineEdge = params[Keys::examineEdge | emptyFunction];
    auto treeEdge = params[Keys::treeEdge | emptyFunction];
    auto notTreeEdge = params[Keys::notTreeEdge | emptyFunction];
    auto finishEdge = params[Keys::finishEdge | emptyFunction];
    auto finishVertex = params[Keys::finishVertex | emptyFunction];

    marker.mark(s);
    buffer.push(s);
    discoverVertex(s);

    while(!buffer.empty())
    {
        VertexDescriptor u = buffer.top();
        buffer.pop();
        examineVertex(u);
        std::cout << "U  " << u << std::endl;
        for(auto e: g.outEdges(u))
        {

//            std::cout << "E  " << e << std::endl;
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
//                std::cout << "E@@  " << e << std::endl;
                notTreeEdge(e, u, target);
//                std::cout << "Eee  " << e << std::endl;
            }

//            std::cout << "E end:  " << e << std::endl;
            finishEdge(e, u, target);
        }
        finishVertex(u);
    }
}













} // namesapce lz





#endif /* LZ_BREADTH_FIRST_SEARCH_H_ */
