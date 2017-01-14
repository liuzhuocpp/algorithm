/*
 * bellman_ford_shortest_paths.h
 *
 *  Created on: 2017年1月13日
 *      Author: LZ
 */

#ifndef LZ_BELLMAN_FORD_SHORTEST_PATHS_H_
#define LZ_BELLMAN_FORD_SHORTEST_PATHS_H_

#include <lz/parameter.h>
#include <lz/std_utility.h>
#include <lz/depth_first_search.h>
#include <lz/iterator_range.h>
#include <lz/graph_utility.h>



namespace lz {


    namespace BellmanFordShortestPathsKeywords {

    LZ_PARAMETER_KEYWORD(tag, vertexIndexMap)
    LZ_PARAMETER_KEYWORD(tag, weightMap)
    LZ_PARAMETER_KEYWORD(tag, distanceMap)
    LZ_PARAMETER_KEYWORD(tag, distanceCompare)
    LZ_PARAMETER_KEYWORD(tag, distanceCombine)
    LZ_PARAMETER_KEYWORD(tag, distanceInf)
    LZ_PARAMETER_KEYWORD(tag, distanceZero)

    LZ_PARAMETER_KEYWORD(tag, startVertex)
    LZ_PARAMETER_KEYWORD(tag, edgeRelaxed)





    } // namespace BellmanFordShortestPathsKeywords



template<typename G, typename Params = EmptyParamPack>
bool bellmanFordShortestPaths(const G &g, const Params& params = Params())
{
    namespace Keys = BellmanFordShortestPathsKeywords;

    auto weightMap = params[Keys::weightMap | edgePropertyMap(g, edgeWeightTag)];
    auto indexMap = params[Keys::vertexIndexMap | vertexPropertyMap(g, vertexIndexTag)];

    auto distanceMap = params[Keys::distanceMap || calculateVertexIndexComposeMap<
        typename MapTraits<decltype(weightMap)>::KeyType>(indexMap, verticesNumber(g))];
    using DistanceType = typename MapTraits<decltype(distanceMap)>::ValueType;
    auto distanceLess = params[Keys::distanceCompare | std::less<DistanceType>()];
    auto distanceCombine = params[Keys::distanceCombine | std::plus<DistanceType>()];

    auto distanceInf = params[Keys::distanceInf | std::numeric_limits<DistanceType>::max()];
    auto distanceZero = params[Keys::distanceZero | 0];

    auto _vertices = vertices(g);
    auto startVertex = params[Keys::startVertex | *_vertices.first];
    auto edgeRelaxed = params[Keys::edgeRelaxed | emptyFunction];

    using Vertex = typename GraphTraits<G>::VertexDescriptor;
    using Edge = typename GraphTraits<G>::EdgeDescriptor;

    auto _edges = edges(g);

    for(auto u: _vertices)
    {
        distanceMap[u] = distanceInf;
    }
    distanceMap[startVertex] = distanceZero;

    auto n = verticesNumber(g);
    for(auto i: irange(n - 1))
    {
        for(Edge e: _edges)
        {
            Vertex u = source(g, e);
            Vertex to = target(g, e);
            if(relaxEdge(weightMap, distanceMap, distanceLess, distanceCombine, distanceInf, distanceZero, e, u, to))
            {
                edgeRelaxed(e, u, to);
            }
        }
    }

    for(Edge e: _edges)
    {
        Vertex u = source(g, e);
        Vertex to = target(g, e);
        if(relaxEdge(weightMap, distanceMap, distanceLess, distanceCombine, distanceInf, distanceZero, e, u, to))
        {
            return false;
        }
    }
    return true;









}



















} // namespace lz





#endif /* LZ_BELLMAN_FORD_SHORTEST_PATHS_H_ */
