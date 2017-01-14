/*
 * ladder_level_ancestor.h
 *
 *  Created on: 2017年1月14日
 *      Author: LZ
 */

#ifndef LZ_LADDER_LEVEL_ANCESTOR_H_
#define LZ_LADDER_LEVEL_ANCESTOR_H_


/*

Learn:
    https://en.wikipedia.org/wiki/Level_ancestor_problem

 */

#include <lz/parameter.h>
#include <lz/depth_first_search.h>
#include <lz/iterator_range.h>
#include <lz/graph_utility.h>
#include <cmath>
#include <vector>



namespace lz {

    namespace LadderLevelAncestorKeywords {


    LZ_PARAMETER_KEYWORD(tag, rootVertex)
    LZ_PARAMETER_KEYWORD(tag, vertexIndexMap)
    LZ_PARAMETER_KEYWORD(tag, longSonMap)
    LZ_PARAMETER_KEYWORD(tag, depthMap)
    LZ_PARAMETER_KEYWORD(tag, heightMap)
    LZ_PARAMETER_KEYWORD(tag, parentMap)

    LZ_PARAMETER_KEYWORD(tag, topmostMap)
    LZ_PARAMETER_KEYWORD(tag, newIndexMap)




    } // namespace jumpPointersLowestCommonAncestorKeywords





template<typename Tree, typename Params = EmptyParamPack>
auto ladderLevelAncestor(const Tree& tree, const Params& params = Params())
{
    namespace Keys = LadderLevelAncestorKeywords;

    using Vertex = typename GraphTraits<Tree>::VertexDescriptor;

    auto n = verticesNumber(tree);
    auto _vertices = vertices(tree);
    auto indexMap = params[Keys::vertexIndexMap | vertexPropertyMap(tree, vertexIndexTag)];
    auto depthMap = params[Keys::depthMap || calculateVertexIndexComposeMap<size_t>(indexMap, n)];
    auto heightMap = params[Keys::heightMap || calculateVertexIndexComposeMap<size_t>(indexMap, n)];

    auto parentMap = params[Keys::parentMap || calculateVertexIndexComposeMap<Vertex>(indexMap, n)];
    auto longSonMap = params[Keys::longSonMap || calculateVertexIndexComposeMap<Vertex>(indexMap, n)];
    auto newIndexMap = params[Keys::newIndexMap || calculateVertexIndexComposeMap<size_t>(indexMap, n)];

    auto topmostMap = params[Keys::topmostMap || calculateVertexIndexComposeMap<Vertex>(indexMap, n)];
    auto rootVertex = params[Keys::rootVertex | *_vertices.first];
    using DepthType = typename MapTraits<decltype(depthMap)>::ValueType;

    parentMap[rootVertex] = rootVertex;
    depthMap[rootVertex] = 0;
    for(auto u: _vertices)
    {
        heightMap[u] = 0;
        longSonMap[u] = GraphTraits<Tree>::nullVertex();
    }
    topmostMap[rootVertex] = rootVertex;

    adjacencyTreeDFS(tree, (
        DepthFirstSearchKeywords::treeEdge = [&](Vertex u, Vertex to) {
            parentMap[to] = u;
            depthMap[to] = depthMap[u] + 1;
        },

        DepthFirstSearchKeywords::treeEdgeReturn = [&](Vertex u, Vertex to) {
            if(heightMap[u] < heightMap[to] + 1)
            {
                heightMap[u] = heightMap[to] + 1;
                longSonMap[u] = to;
            }

        }));


    vector<Vertex> stack;
    vector<Vertex> data;
    vector<std::ptrdiff_t> first;

    adjacencyTreeDFS(tree, (
        DepthFirstSearchKeywords::discoverVertex = [&](Vertex u) {
            stack.push_back(u);

        },
        DepthFirstSearchKeywords::treeEdge = [&](Vertex u, Vertex to) {
            if(longSonMap[u] == to)
            {
                topmostMap[to] = topmostMap[u];
            }
            else
            {
                topmostMap[to] = to;
            }
        },

        DepthFirstSearchKeywords::finishVertex = [&](Vertex u) {

            if(longSonMap[u] == GraphTraits<Tree>::nullVertex())
            {
                Vertex topmost = topmostMap[u];
                DepthType h = depthMap[u] - depthMap[topmost];

                auto begin = std::max(stack.end() - (2 * h + 1), stack.begin());
                std::ptrdiff_t firstPos = data.size();
                for(Vertex x: makeIteratorRange(begin, stack.end()))
                {
                    newIndexMap[x] = data.size();
                    data.push_back(x);
                    first.push_back(firstPos);
                }
            }
            stack.pop_back();

        }));



    return [=, data = std::move(data), first = std::move(first)](Vertex u, DepthType d) {
        if(depthMap[u] < d) return GraphTraits<Tree>::nullVertex();
        if(depthMap[u] == d) return u;
        if(d == 0) return rootVertex;
        while(1)
        {
            std::ptrdiff_t pos = newIndexMap[u];
            Vertex firstVertex = data[first[pos]];
            if(depthMap[firstVertex] <= d)
            {
                d -= depthMap[firstVertex];
                return data[first[pos] + d];
            }
            else
            {
                u = parentMap[firstVertex];
            }
        }
    };











}


} // namespace lz





#endif /* LZ_LADDER_LEVEL_ANCESTOR_H_ */
