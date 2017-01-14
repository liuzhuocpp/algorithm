/*
 * dag_shortest_paths.h
 *
 *  Created on: 2017年1月13日
 *      Author: LZ
 */

#ifndef LZ_DAG_SHORTEST_PATHS_H_
#define LZ_DAG_SHORTEST_PATHS_H_

#include <lz/parameter.h>
#include <lz/std_utility.h>
#include <lz/depth_first_search.h>
#include <lz/iterator_range.h>

namespace lz {

    namespace DagShortestPathsKeywords {

    LZ_PARAMETER_KEYWORD(tag, vertexIndexMap)
    LZ_PARAMETER_KEYWORD(tag, weightMap)
    LZ_PARAMETER_KEYWORD(tag, distanceMap)
    LZ_PARAMETER_KEYWORD(tag, distanceCompare)
    LZ_PARAMETER_KEYWORD(tag, distanceCombine)
    LZ_PARAMETER_KEYWORD(tag, distanceInf)
    LZ_PARAMETER_KEYWORD(tag, distanceZero)

    LZ_PARAMETER_KEYWORD(tag, startVertex)
    LZ_PARAMETER_KEYWORD(tag, edgeRelaxed)





    } // namespace DagShortestPathsKeywords


    namespace DagShortestPathsPrivate {

        template<typename WeightMap, typename DistanceMap, typename DistanceLess, typename DistanceCombine,
            typename Edge, typename Vertex, typename EdgeRelaxed>
        void relaxEdge(
            WeightMap weightMap,
            DistanceMap distanceMap,
            DistanceLess distanceLess,
            DistanceCombine distanceCombine,
            typename MapTraits<DistanceMap>::ValueType distanceInf,
            typename MapTraits<DistanceMap>::ValueType distanceZero,
            Edge e,
            Vertex u,
            Vertex to,
            EdgeRelaxed edgeRelaxed
            )
        {
            if(distanceMap[u] != distanceInf)
            {
                auto distanceTmp = distanceCombine(distanceMap[u], weightMap[e]);
                if(distanceLess( distanceTmp, distanceMap[to]))
                {
                    distanceMap[to] = distanceTmp;
                    edgeRelaxed(e, u, to);
                }
            }

        }
    }

template<typename G, typename Params = EmptyParamPack>
void dagShortestPaths(const G &g, const Params& params = Params())
{
    namespace Keys = DagShortestPathsKeywords;

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


    std::vector<Vertex> topoOrder;


    depthFirstSearch(g, (
            DepthFirstSearchKeywords::enterVertex = startVertex,
            DepthFirstSearchKeywords::finishVertex = [&](Vertex u) {
                std::cout << "U " << u << "\n";
                topoOrder.push_back(u);
            }));


    for(Vertex u: makeIteratorRange(topoOrder.rbegin(), topoOrder.rend()))
    {
        std::cout << "UU " << u << "\n";
        for(Edge e: outEdges(g, u))
        {
            Vertex to = opposite(g, e, u);
            DagShortestPathsPrivate::relaxEdge(weightMap, distanceMap, distanceLess, distanceCombine,
                distanceInf, distanceZero, e, u, to, edgeRelaxed);
        }
    }


}




} // namespace lz





#endif /* LZ_DAG_SHORTEST_PATHS_H_ */
