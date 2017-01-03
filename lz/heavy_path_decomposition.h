#ifndef HEAVY_PATH_DECOMPOSITION
#define HEAVY_PATH_DECOMPOSITION

#include <vector>
#include <functional>
#include <algorithm>
#include <iostream>
#include <utility>
#include "lz/parameter.h"
#include "lz/utility.h"
#include "lz/std_utility.h"
#include "lz/graph_utility.h"
#include "lz/depth_first_search.h"
#include "lz/iterator_facade.h"

namespace lz {

namespace HeavyPathDecompositionKeywords {

    LZ_PARAMETER_KEYWORD(tag, parentMap)
    LZ_PARAMETER_KEYWORD(tag, heavySonMap)
    LZ_PARAMETER_KEYWORD(tag, sizeMap)
    LZ_PARAMETER_KEYWORD(tag, depthMap)

    LZ_PARAMETER_KEYWORD(tag, topmostMap)
    LZ_PARAMETER_KEYWORD(tag, rootVertex)
    LZ_PARAMETER_KEYWORD(tag, vertexIndexMap)
    LZ_PARAMETER_KEYWORD(tag, newIndexMap)

}

namespace HeavyPathDecompositionPrivate {

//    template<typename G, typename ParentMap, typename HeavySonMap, typename SizeMap, typename DepthMap>
//    void dfs1(const G &g, ParentMap parentMap, HeavySonMap heavySonMap, SizeMap sizeMap, DepthMap depthMap,
//               typename GraphTraits<G>::VertexDescriptor u,
//               typename GraphTraits<G>::VertexDescriptor fa)
//    {
//        using VertexDescriptor = typename GraphTraits<G>::VertexDescriptor;
//        for(auto e: outEdges(g, u))
//        {
//            VertexDescriptor to = opposite(g, e, u);
//            if(to == fa) continue;
//            depthMap[to] = depthMap[u] + 1;
//            parentMap[to] = u;
//            dfs1(g, parentMap, heavySonMap, sizeMap, depthMap, to, u);
//            sizeMap[u] += sizeMap[to];
//            if(heavySonMap[u] == GraphTraits<G>::nullVertex() || sizeMap[heavySonMap[u]] < sizeMap[to])
//            {
//                heavySonMap[u] = to;
//            }
//        }
//    }
//
//    template<typename G, typename HeavySonMap, typename TopmostMap, typename NewIndexMap, typename SizeType>
//    void dfs2(const G &g, HeavySonMap heavySonMap, TopmostMap topmostMap, NewIndexMap newIndexMap,
//               SizeType &timestamp,
//               typename GraphTraits<G>::VertexDescriptor u,
//               typename GraphTraits<G>::VertexDescriptor fa)
//    {
//        newIndexMap[u] = timestamp ++;
//        using VertexDescriptor = typename GraphTraits<G>::VertexDescriptor;
//        if(heavySonMap[u] != GraphTraits<G>::nullVertex())
//        {
//            VertexDescriptor to = heavySonMap[u];
//            topmostMap[to] = topmostMap[u];
//            dfs2(g, heavySonMap, topmostMap, newIndexMap, timestamp, to, u);
//        }
//
//        for(auto e: outEdges(g, u))
//        {
//            VertexDescriptor to = opposite(g, e, u);
//            if(to == fa || to == heavySonMap[u]) continue;
//            dfs2(g, heavySonMap, topmostMap, newIndexMap,timestamp, to, u);
//        }
//    }



// I want to reuse the depthFirstSearch instead of dfs2, but failed!!!
// Now, it is successful !



    template<typename G, typename HeavySonMap>
    struct NewGraph
    {
        using V = typename GraphTraits<G>::VertexDescriptor;
        struct AdjacencyVertexIterator:
                IteratorAdapter<AdjacencyVertexIterator,
                                typename GraphTraits<G>::AdjacencyVertexIterator>
        {
            using Base = IteratorAdapter<AdjacencyVertexIterator,
                    typename GraphTraits<G>::AdjacencyVertexIterator>;
            V firstSon;
            V heavySon;

            AdjacencyVertexIterator() = default;
            AdjacencyVertexIterator(const Base& base,
                    V _firstSon = GraphTraits<G>::nullVertex(),
                    V _heavySon = GraphTraits<G>::nullVertex()):
                Base(base), firstSon(_firstSon), heavySon(_heavySon) {}
            V operator*() const
            {
                V cnt = this->base().operator*();
                if(heavySon == GraphTraits<G>::nullVertex()) return cnt;
                if(cnt == firstSon) return heavySon;
                else if(cnt == heavySon) return firstSon;
                else return cnt;
            }
        };

        const G &g;
        HeavySonMap &heavySonMap;
    };







} // namespace HeavyPathDecompositionPrivate


template<typename G, typename HeavySonMap>
struct GraphTraits<HeavyPathDecompositionPrivate::NewGraph<G, HeavySonMap>>: public GraphTraits<G>
{
    using AdjacencyVertexIterator = typename GraphTraits<G>::AdjacencyVertexIterator;
};

template<typename G, typename HeavySonMap>
std::pair<
typename HeavyPathDecompositionPrivate::NewGraph<G, HeavySonMap>::AdjacencyVertexIterator,
typename HeavyPathDecompositionPrivate::NewGraph<G, HeavySonMap>::AdjacencyVertexIterator>

adjacencyVertices(const HeavyPathDecompositionPrivate::NewGraph<G, HeavySonMap> &g,
                  typename GraphTraits<G>::VertexDescriptor u)
{
    using Self = HeavyPathDecompositionPrivate::NewGraph<G, HeavySonMap>;
    using AdjacencyVertexIterator = typename Self::AdjacencyVertexIterator;
    auto cnt = adjacencyVertices(g.g, u);
    AdjacencyVertexIterator beg(cnt.first, *cnt.first, g.heavySonMap[u]), end(cnt.second);
    return std::make_pair(beg, end);
}


/*

G must modal AdjacencyGraph, and must a Tree

 */
template<typename G, typename ParamPack = EmptyParamPack>
auto heavyPathDecomposition(const G &g, const ParamPack &params = EmptyParamPack())
{
    namespace Keys = HeavyPathDecompositionKeywords;
    auto n = verticesNumber(g);

    using VertexDescriptor = typename GraphTraits<G>::VertexDescriptor;
    using EdgeDescriptor = typename GraphTraits<G>::EdgeDescriptor;
    auto _vertices = vertices(g);

    auto vertexIndexMap = params[Keys::vertexIndexMap | vertexPropertyMap(g, vertexIndexTag)];
    auto parentMap = params[Keys::parentMap || calculateVertexIndexComposeMap<VertexDescriptor>(vertexIndexMap, n)];
    auto heavySonMap = params[Keys::heavySonMap|| calculateVertexIndexComposeMap<VertexDescriptor>(vertexIndexMap, n, GraphTraits<G>::nullVertex())];
    auto sizeMap = params[Keys::sizeMap || calculateVertexIndexComposeMap<std::size_t>(vertexIndexMap, n)];
    auto depthMap = params[Keys::depthMap || calculateVertexIndexComposeMap<std::size_t>(vertexIndexMap, n)];
    auto topmostMap = params[Keys::topmostMap || calculateVertexIndexComposeMap<VertexDescriptor>(vertexIndexMap, n)];
    auto newIndexMap = params[Keys::newIndexMap || calculateVertexIndexComposeMap<std::size_t>(vertexIndexMap, n)];

    auto rootVertex = params[Keys::rootVertex | *_vertices.first];
    for(auto u: _vertices)
    {
        topmostMap[u] = u;
        heavySonMap[u] = GraphTraits<G>::nullVertex();
        sizeMap[u] = 1;
    }
    parentMap[rootVertex] = rootVertex;

    depthMap[rootVertex] = 0;

    typename MapTraits<decltype(newIndexMap)>::ValueType timestamp = 0;


//    HeavyPathDecompositionPrivate::dfs1(g, parentMap, heavySonMap, sizeMap,depthMap, rootVertex, rootVertex);

    adjacencyTreeDFS(g, (
            DepthFirstSearchKeywords::enterVertex = rootVertex
           ,DepthFirstSearchKeywords::treeEdge =
               [&](VertexDescriptor u, VertexDescriptor to) {
                   depthMap[to] = depthMap[u] + 1;
                   parentMap[to] = u;
               }
            ,DepthFirstSearchKeywords::treeEdgeReturn =
                [&](VertexDescriptor u, VertexDescriptor to) {
                    sizeMap[u] += sizeMap[to];
                    if(heavySonMap[u] == GraphTraits<G>::nullVertex() || sizeMap[heavySonMap[u]] < sizeMap[to])
                    {
                        heavySonMap[u] = to;
                    }
                }

            ));



//    HeavyPathDecompositionPrivate::dfs2(g, heavySonMap, topmostMap, newIndexMap,timestamp, rootVertex, rootVertex);

    HeavyPathDecompositionPrivate::NewGraph<G, decltype(heavySonMap)> ng{g, heavySonMap};
    adjacencyTreeDFS(ng, (
            DepthFirstSearchKeywords::enterVertex = rootVertex
           ,DepthFirstSearchKeywords::treeEdge =
               [&](VertexDescriptor u, VertexDescriptor to) {
                   if(to == heavySonMap[u])
                   {
                       topmostMap[to] = topmostMap[u];
                   }
               }
            ,DepthFirstSearchKeywords::discoverVertex =
                [&](VertexDescriptor u) {
                    newIndexMap[u] = timestamp ++;
                }
            ));




    // 按照点进行返回的，例如：[(3, 3), (6, 10)]，每个区间内的节点新编号是连续的
    auto getPath = [=, &g](auto a, auto b, auto outputIterator)
    {
        while(topmostMap[a] != topmostMap[b])
        {
            if(depthMap[topmostMap[a]] < depthMap[topmostMap[b]]) std::swap(a, b);
            *outputIterator++ = std::make_pair(topmostMap[a], a);
            a = parentMap[topmostMap[a]];
        }

        if(depthMap[a] < depthMap[b]) std::swap(a, b);
        if(depthMap[a] != depthMap[b])
        {
            *outputIterator++ = std::make_pair(b, a);
        }

        return std::make_pair(outputIterator, b);
    };

    return getPath;
}






//using std::swap;
//using std::pair;
//using std::make_pair;
//
//class HeavyLightPath
//{
//
//public:
//    HeavyLightPath(){}
//    HeavyLightPath(const AdjacencyList<> &_g, int rt = 0){ decomposition(_g, rt); }
//    void decomposition(const AdjacencyList<> &_g, int rt = 0)
//    {
//        root = rt;
//        g = &_g;
//        int n = g->vertexNumber();
//        siz.assign(n, 1);
//        dep.assign(n, 1);
//        son.assign(n, -1);
//        p.assign(n, -1);
//        top.clear();
//        for(int i = 0; i < n; ++ i) top.push_back(i);
//        nid.assign(n, -1);
//        inc = 0;
//        dfs1(root, root);
//        dfs2(root, root);
//
//        vector<int> tp(n, -1), ttop(n, -1), tdep(n, 1);
//
//        // np.assign(n, -1);
//        // ntop.assign(n, -1);
//
//        for(int i = 0; i < n; ++ i)
//        {
//            ttop[nid[i]] = nid[top[i]];
//            tdep[nid[i]] = dep[i];
//            if(p[i] == -1) continue;
//            tp[nid[i]] = nid[p[i]];
//        }
//        p = std::move(tp);
//        top = std::move(ttop);
//        dep = std::move(tdep);
//    }
//    inline vector<int>& newId()  { return nid; }
//    inline vector<int>& parent()  { return p; }
//    inline vector<int>& topVertex()  { return top; }
//    inline vector<int>& depth() { return dep; }
//    int getPath(int a, int b, vector<pair<int, int> > &st) /// a->b
//    {
//        st.clear();
//        while(top[a] != top[b])
//        {
//            if(dep[top[a]] < dep[top[b]]) swap(a, b);
//            st.push_back(make_pair(top[a], a));
//            a = p[top[a]];
//        }
//
//        if(dep[a] > dep[b]) swap(a, b);
//        st.push_back(make_pair(a, b));
//        return a;
//    }
//
//private:
//    void dfs1(int u, int fa)
//    {
//        for(int i = g->begin(u); ~i; i = g->next(i))
//        {
//            int to = g->to(i);
//            if(to == fa) continue;
//            dep[to] = dep[u] + 1;
//            p[to] = u;
//            dfs1(to, u);
//            siz[u] += siz[to];
//            if(son[u] == -1 || siz[son[u]] < siz[to])
//            {
//                son[u] = to;
//            }
//        }
//    }
//    void dfs2(int u, int fa)
//    {
//        nid[u] = inc ++;
//        if(son[u] != -1)
//        {
//            top[son[u]] = top[u];
//            dfs2(son[u], u);
//        }
//        for(int i = g->begin(u); ~i; i = g->next(i))
//        {
//            int to = g->to(i);
//            if(to == fa || to == son[u]) continue;
//            dfs2(to, u);
//        }
//    }
//    const AdjacencyList<> *g;
//    vector<int> siz, son, dep, p, nid, top;
//    // vector<int> np, ntop, ndep;
//
//    int inc, root;
//
//};


} // namespace lz
#endif //HEAVY_PATH_DECOMPOSITION
