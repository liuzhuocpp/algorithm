/*
 * level_ancestor.h
 *
 *  Created on: 2017年1月4日
 *      Author: LZ
 */

#ifndef LZ_JUMP_POINTERS_LEVEL_ANCESTOR_H_
#define LZ_JUMP_POINTERS_LEVEL_ANCESTOR_H_

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

    namespace JumpPointersLevelAncestorKeywords {


    LZ_PARAMETER_KEYWORD(tag, rootVertex)
    LZ_PARAMETER_KEYWORD(tag, vertexIndexMap)
    LZ_PARAMETER_KEYWORD(tag, depthMap)

    } // namespace jumpPointersLowestCommonAncestorKeywords



template<typename Tree, typename Params = EmptyParamPack>
auto jumpPointersLevelAncestor(const Tree& tree, const Params& params = Params())
{
    namespace Keys = JumpPointersLevelAncestorKeywords;
    using Vertex = typename GraphTraits<Tree>::VertexDescriptor;

    auto n = verticesNumber(tree);
    auto _vertices = vertices(tree);
    auto rootVertex = params[Keys::rootVertex | *_vertices.first];
    auto indexMap = params[Keys::vertexIndexMap | vertexPropertyMap(tree, vertexIndexTag)];
    auto depthMap = params[Keys::depthMap || calculateVertexIndexComposeMap<size_t>(indexMap, n)];
    using DepthType = typename MapTraits<decltype(depthMap)>::ValueType;

    int maxDepth = std::log2(n);
    std::vector<std::vector<Vertex>> p(n, std::vector<Vertex>(maxDepth + 1));

    depthMap[rootVertex] = 0;
    for(auto j: irange(maxDepth + 1))
        p[rootVertex][j] = rootVertex;

    adjacencyTreeDFS(tree, (
            DepthFirstSearchKeywords::treeEdge = [&](Vertex u, Vertex to) {
                p[to][0] = u;
                for(auto i: irange(1, maxDepth + 1))
                {
                    p[to][i] = p[p[to][i - 1]][i - 1];
                }
                depthMap[to] = depthMap[u] + 1;
            }
            ));

    return [=, p = std::move(p)](Vertex u, DepthType d) {

        if(depthMap[u] < d) return GraphTraits<Tree>::nullVertex();
        if(depthMap[u] == d) return u;
        for(auto i: irange(maxDepth, -1, -1))
        {
            if(depthMap[p[u][i]] >= d)
            {
                u = p[u][i];
            }
        }

        return u;



    };




}






}// namesapce lz

#endif /* LZ_JUMP_POINTERS_LEVEL_ANCESTOR_H_ */
