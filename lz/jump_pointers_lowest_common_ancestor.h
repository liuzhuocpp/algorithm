/*
 * jump_pointers_lowest_common_ancestor.h
 *
 *  Created on: 2017年1月13日
 *      Author: LZ
 */

#ifndef LZ_JUMP_POINTERS_LOWEST_COMMON_ANCESTOR_H_
#define LZ_JUMP_POINTERS_LOWEST_COMMON_ANCESTOR_H_

#include <lz/parameter.h>
#include <lz/depth_first_search.h>
#include <cmath>
#include <vector>

namespace lz {



    namespace JumpPointersLowestCommonAncestorKeywords {


    LZ_PARAMETER_KEYWORD(tag, rootVertex)
    LZ_PARAMETER_KEYWORD(tag, vertexIndexMap)
    LZ_PARAMETER_KEYWORD(tag, depthMap)

    } // namespace jumpPointersLowestCommonAncestorKeywords

template<typename Tree, typename Params = EmptyParamPack>
auto jumpPointersLowestCommonAncestor(const Tree& tree, const Params& params = Params())
{
    namespace Keys = JumpPointersLowestCommonAncestorKeywords;
    using Vertex = typename GraphTraits<Tree>::VertexDescriptor;

    auto n = verticesNumber(tree);
    auto _vertices = vertices(tree);
    auto rootVertex = params[Keys::rootVertex | *_vertices.first];
    auto indexMap = params[Keys::vertexIndexMap | vertexPropertyMap(tree, vertexIndexTag)];
    auto depthMap = params[Keys::depthMap || calculateVertexIndexComposeMap<size_t>(indexMap, n)];

    size_t maxDepth = std::log2(n);
    std::vector<std::vector<Vertex>> p(n, std::vector<Vertex>(maxDepth + 1));

    depthMap[rootVertex] = 0;
    for(int j = 0; j <= maxDepth; ++ j) p[rootVertex][j] = rootVertex;

    adjacencyTreeDFS(tree, (
            DepthFirstSearchKeywords::treeEdge = [&](Vertex u, Vertex to) {
                p[to][0] = u;
                for(size_t i = 1; i <= maxDepth; ++ i)
                {
                    p[to][i] = p[p[to][i - 1]][i - 1];
                }
                depthMap[to] = depthMap[u] + 1;
            }
            ));

    return [=, p = std::move(p)](Vertex a, Vertex b) {
        if(depthMap[a] < depthMap[b]) std::swap(a, b);

        for(std::ptrdiff_t i = maxDepth; i >= 0; -- i)
        {
            if(depthMap[p[a][i]] >= depthMap[b]) a = p[a][i];
        }
        if(a == b) return a;

        for(std::ptrdiff_t i = maxDepth; i >= 0; -- i)
        {
            if(p[a][i] != p[b][i]) a = p[a][i], b = p[b][i];
        }
        return p[a][0];
    };




}







} // namespace lz





#endif /* LZ_JUMP_POINTERS_LOWEST_COMMON_ANCESTOR_H_ */
