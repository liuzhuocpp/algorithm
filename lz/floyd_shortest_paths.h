/*
 * floyd_shortest_paths.h
 *
 *  Created on: 2017年1月13日
 *      Author: LZ
 */

#ifndef LZ_FLOYD_SHORTEST_PATHS_H_
#define LZ_FLOYD_SHORTEST_PATHS_H_

#include <lz/parameter.h>
#include <lz/utility.h>
#include <lz/graph_utility.h>
#include <lz/map.h>
#include <lz/std_utility.h>
#include <iterator>
namespace lz {


/*

weightMap: key type must be EdgeDescriptor, value type must be distance type
distanceMap: key type must be pair<VertexDescriptor, VertexDescriptor>, value type must be distance type
distanceCompare: binary function, both argument type are distance type, return value is boolean
    which indicates whether first argument is less than second argument

distanceCombine: binary function, both argument type are distance type, return value is distance type
    which indicates the combination of first argument and second argument

 */
    namespace FloydShortestPathKeywords {

    LZ_PARAMETER_KEYWORD(tag, weightMap)
    LZ_PARAMETER_KEYWORD(tag, distanceMap)
    LZ_PARAMETER_KEYWORD(tag, distanceCompare)
    LZ_PARAMETER_KEYWORD(tag, distanceCombine)
    LZ_PARAMETER_KEYWORD(tag, distanceInf)
    LZ_PARAMETER_KEYWORD(tag, distanceZero)
    LZ_PARAMETER_KEYWORD(tag, edgeRelaxed)


    } // namespace FloydShortestPathKeywords


    namespace FloydShortestPathPrivate {

    using std::make_pair;

    template<typename VertexIteratorPair, typename DistanceMap, typename DistanceLess,
        typename DistanceCombine, typename EdgeRelaxed>
    bool work(VertexIteratorPair _vertices, DistanceMap distanceMap, DistanceLess distanceLess,
        DistanceCombine distanceCombine, EdgeRelaxed edgeRelaxed,
        typename MapTraits<DistanceMap>::ValueType distanceInf,
        typename MapTraits<DistanceMap>::ValueType distanceZero)
    {
        using VertexDescriptor = typename std::iterator_traits< typename VertexIteratorPair::first_type>::value_type;
        for(VertexDescriptor k: _vertices)
        {
            for(VertexDescriptor i: _vertices)
            {
                for(VertexDescriptor j: _vertices)
                {
                    if(distanceMap[make_pair(i, k)] != distanceInf
                        && distanceMap[make_pair(k, j)] != distanceInf)
                    {
                        auto distanceTmp = distanceCombine(distanceMap[make_pair(i, k)], distanceMap[make_pair(k, j)]);
                        if(distanceLess(distanceTmp, distanceMap[make_pair(i, j)]))
                        {
                            distanceMap[make_pair(i, j)] = distanceTmp;
                            edgeRelaxed(i, j);
                        }
                    }
                }
            }
        }

        for(VertexDescriptor i: _vertices)
        {
            if(distanceLess(distanceMap[make_pair(i, i)], distanceZero))
                return false;
        }
        return true;
    }

//    namespace Keys = FloydShortestPathKeywords;
//    template<typename G, typename Params>
//    auto calculateParams(const G &, const Params& params)
//    {
//
//        auto distanceMap = params[Keys::distanceMap];
//        using DistanceType = typename MapTraits<decltype(distanceMap)>::ValueType;
//        auto distanceLess = params[Keys::distanceCompare | std::less<DistanceType>()];
//        auto distanceCombine = params[Keys::distanceCombine | std::plus<DistanceType>()];
//
//        auto distanceInf = params[Keys::distanceInf | std::numeric_limits<DistanceType>::max()];
//        auto distanceZero = params[Keys::distanceZero | 0];
//        auto edgeRelaxed = params[Keys::edgeRelaxed | emptyFunction];
//
//        using VertexDescriptor = typename GraphTraits<G>::VertexDescriptor;
//        using EdgeDescriptor = typename GraphTraits<G>::EdgeDescriptor;
//
//        return (Keys::distanceMap = distanceMap,
//                Keys::distanceCompare = distanceLess,
//                Keys::distanceCombine = distanceCombine,
//                Keys::distanceInf = distanceInf,
//                Keys::distanceZero = distanceZero,
//                )
////        auto _vertices = vertices(g);
//
//    }



    }



template<typename G, typename Params = EmptyParamPack>
bool floydInitializedShortestPaths(const G &g, const Params& params = Params())
{
    namespace Keys = FloydShortestPathKeywords;
    using std::make_pair;

    auto distanceMap = params[Keys::distanceMap];
    using DistanceType = typename MapTraits<decltype(distanceMap)>::ValueType;
    auto distanceLess = params[Keys::distanceCompare | std::less<DistanceType>()];
    auto distanceCombine = params[Keys::distanceCombine | std::plus<DistanceType>()];

    auto distanceInf = params[Keys::distanceInf | std::numeric_limits<DistanceType>::max()];
    auto distanceZero = params[Keys::distanceZero | 0];
    auto edgeRelaxed = params[Keys::edgeRelaxed | emptyFunction];

    using VertexDescriptor = typename GraphTraits<G>::VertexDescriptor;
    using EdgeDescriptor = typename GraphTraits<G>::EdgeDescriptor;
    auto _vertices = vertices(g);

    return FloydShortestPathPrivate::work(_vertices, distanceMap, distanceLess, distanceCombine,
        edgeRelaxed, distanceInf, distanceZero);


}




template<typename G, typename Params = EmptyParamPack>
bool floydShortestPaths(const G &g, const Params& params = Params())
{
    namespace Keys = FloydShortestPathKeywords;
    using std::make_pair;


    auto weightMap = params[Keys::weightMap | edgePropertyMap(g, edgeWeightTag)];
    auto distanceMap = params[Keys::distanceMap];
    using DistanceType = typename MapTraits<decltype(distanceMap)>::ValueType;
    auto distanceLess = params[Keys::distanceCompare | std::less<DistanceType>()];
    auto distanceCombine = params[Keys::distanceCombine | std::plus<DistanceType>()];

    auto distanceInf = params[Keys::distanceInf | std::numeric_limits<DistanceType>::max()];
    auto distanceZero = params[Keys::distanceZero | 0];
    auto edgeRelaxed = params[Keys::edgeRelaxed | emptyFunction];

    using VertexDescriptor = typename GraphTraits<G>::VertexDescriptor;
    using EdgeDescriptor = typename GraphTraits<G>::EdgeDescriptor;
    auto _vertices = vertices(g);
    auto _edges = edges(g);

    for(VertexDescriptor i: _vertices)
    {
        for(VertexDescriptor j: _vertices)
        {
            auto vertexPair = std::make_pair(i, j);
            if(i == j) distanceMap[vertexPair] = distanceZero;
            else distanceMap[vertexPair] = distanceInf;
        }
    }

    for(EdgeDescriptor e: _edges)
    {
        DistanceType& distanceRef = distanceMap[incident(g, e)];
        if(distanceLess(weightMap[e], distanceRef))
        {
            distanceRef = weightMap[e];
            edgeRelaxed(source(g, e), target(g, e));
        }
    }

    if(std::is_same<typename GraphTraits<G>::DirectedCategory, UndirectedGraphTag>::value)
    {
        for(EdgeDescriptor e: _edges)
        {
            DistanceType& distanceRef = distanceMap[make_pair(target(g, e), source(g, e))];
            if(distanceLess(weightMap[e], distanceRef))
            {
                distanceRef = weightMap[e];
                edgeRelaxed(source(g, e), target(g, e));
            }
        }

    }


    return FloydShortestPathPrivate::work(_vertices, distanceMap, distanceLess, distanceCombine,
        edgeRelaxed, distanceInf, distanceZero);



}









} // namespace lz





#endif /* LZ_FLOYD_SHORTEST_PATHS_H_ */
