/*
 * biconnected_components_of_vertex.h
 *
 *  Created on: 2017年1月11日
 *      Author: LZ
 */

#ifndef LZ_BICONNECTED_COMPONENTS_OF_VERTEX_H_
#define LZ_BICONNECTED_COMPONENTS_OF_VERTEX_H_


#include <lz/parameter.h>
#include <lz/depth_first_search.h>
#include <stack>
#include <vector>

namespace lz {


    namespace BiconnectedComponentsOfVertexKeywords {

    LZ_PARAMETER_KEYWORD(tag, vertexIndexMap)
    LZ_PARAMETER_KEYWORD(tag, discoverTimeMap)
    LZ_PARAMETER_KEYWORD(tag, lowTimeMap)
    LZ_PARAMETER_KEYWORD(tag, discoverArticulationPoint)
    LZ_PARAMETER_KEYWORD(tag, stack)

    }



template<typename Graph, typename ComponentMap, typename Params = EmptyParamPack>
auto biconnectedComponentsOfVertex(const Graph& g, ComponentMap componentMap, const Params& params = Params())
{
    using Vertex = typename GraphTraits<Graph>::VertexDescriptor;
    using Edge = typename GraphTraits<Graph>::EdgeDescriptor;
    namespace Keys = BiconnectedComponentsOfVertexKeywords;

    auto n = verticesNumber(g);

    auto indexMap = params[Keys::vertexIndexMap | vertexPropertyMap(g, vertexIndexTag)];
    auto discoverTimeMap = params[Keys::discoverTimeMap || calculateVertexIndexComposeMap<size_t>(indexMap, n)];
    auto lowTimeMap = params[Keys::lowTimeMap || calculateVertexIndexComposeMap<size_t>(indexMap, n)];
    auto discoverArticulationPoint = params[Keys::discoverArticulationPoint | emptyFunction];
    decltype(auto) stack = params[Keys::stack || [&]() { return std::stack<Edge, std::vector<Edge>>(); } ];

    using DiscoverTimeMap = decltype(discoverTimeMap);
    using TimeType = typename MapTraits<DiscoverTimeMap>::ValueType;

    typename MapTraits<ComponentMap>::ValueType componentNumber = 0;
    TimeType dfsTime = 0;

    struct Marker
    {
        DiscoverTimeMap& discoverTimeMap;
        TimeType& dfsTime;
        bool isMark(Vertex u) const
        {
            return discoverTimeMap[u] != 0;
        }

        void mark(Vertex u)
        {
            discoverTimeMap[u] = ++ dfsTime;
        }

    }marker{discoverTimeMap, dfsTime};

    Vertex cntRootVertex;
    std::size_t cntRootSons;
    depthFirstSearch(g, (

            DepthFirstSearchKeywords::marker = marker,

            DepthFirstSearchKeywords::startVertex = [&](Vertex u) {
                cntRootVertex = u;
                cntRootSons = 0;
            },

            DepthFirstSearchKeywords::discoverVertex = [&](Vertex u) {
                lowTimeMap[u] = discoverTimeMap[u];
            },

            DepthFirstSearchKeywords::treeEdge = [&](Edge e, Vertex u, Vertex to) {
                stack.push(e);
                if(u == cntRootVertex)
                {
                    cntRootSons ++;
                }

            },

            DepthFirstSearchKeywords::treeEdgeReturn = [&](Edge e, Vertex u, Vertex to) {

                lowTimeMap[u] = std::min(lowTimeMap[u], lowTimeMap[to]);
                if(lowTimeMap[to] >= discoverTimeMap[u])
                {
                    Edge tmp;
                    do
                    {
                        tmp = stack.top();
                        stack.pop();
                        componentMap[tmp] = componentNumber;
                    }
                    while(tmp != e);
                    componentNumber ++;
                }
            },
            DepthFirstSearchKeywords::notTreeEdge = [&](Edge e, Vertex u, Vertex to) {
                if(discoverTimeMap[u] >= discoverTimeMap[to])
                {
                    stack.push(e);
                    lowTimeMap[u] = std::min(lowTimeMap[u], lowTimeMap[to]);
                }
            },
            DepthFirstSearchKeywords::finishVertex = [&](Vertex u) {
                if(u == cntRootVertex)
                {
                    if(cntRootSons > 1)
                        discoverArticulationPoint(u);
                }
                else
                {
                    for(Edge e: outEdges(g, u))
                    {
                        Vertex to = opposite(g, e, u);
                        if(lowTimeMap[to] >= discoverTimeMap[u])
                        {
                            discoverArticulationPoint(u);
                            break;
                        }
                    }

                }
            }));

    return componentNumber;

}















} // namespace lz





#endif /* LZ_BICONNECTED_COMPONENTS_OF_VERTEX_H_ */
