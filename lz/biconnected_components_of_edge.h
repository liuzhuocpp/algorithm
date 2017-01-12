#ifndef BICONNECTED_COMPONENTS_OF_EDGE_H
#define BICONNECTED_COMPONENTS_OF_EDGE_H


#include <lz/parameter.h>
#include <lz/depth_first_search.h>
#include <stack>
#include <vector>

namespace lz {


    namespace BiconnectedComponentsOfEdgeKeywords {

    LZ_PARAMETER_KEYWORD(tag, vertexIndexMap)
    LZ_PARAMETER_KEYWORD(tag, discoverTimeMap)
    LZ_PARAMETER_KEYWORD(tag, rootMap)
    LZ_PARAMETER_KEYWORD(tag, discoverBridge)
    LZ_PARAMETER_KEYWORD(tag, stack)

    }



template<typename Graph, typename ComponentMap, typename Params = EmptyParamPack>
auto biconnectedComponentsOfEdge(const Graph& g, ComponentMap componentMap, const Params& params = Params())
{
    using Vertex = typename GraphTraits<Graph>::VertexDescriptor;
    using Edge = typename GraphTraits<Graph>::EdgeDescriptor;
    namespace Keys = BiconnectedComponentsOfEdgeKeywords;

    auto n = verticesNumber(g);
    auto indexMap = params[Keys::vertexIndexMap | vertexPropertyMap(g, vertexIndexTag)];

    auto discoverTimeMap = params[Keys::discoverTimeMap || calculateVertexIndexComposeMap<size_t>(indexMap, n, 0)];
    auto rootMap = params[Keys::rootMap || calculateVertexIndexComposeMap<size_t>(indexMap, n)];
    auto discoverBridge = params[Keys::discoverBridge | emptyFunction];
    decltype(auto) stack = params[Keys::stack || [&]() { return std::stack<Edge, std::vector<Vertex>>(); } ];

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

    auto minRoot = [&](Vertex u, Vertex v) ->Vertex
    {
        auto ans =  discoverTimeMap[u] < discoverTimeMap[v] ? u : v;
        return ans;
    };

    depthFirstSearch(g, (

            DepthFirstSearchKeywords::marker = marker,

            DepthFirstSearchKeywords::discoverVertex = [&](Vertex u) {
                rootMap[u] = u;
                stack.push(u);
            },

            DepthFirstSearchKeywords::treeEdge = [&](Edge e, Vertex u, Vertex to) {

            },

            DepthFirstSearchKeywords::treeEdgeReturn = [&](Edge e, Vertex u, Vertex to) {
                rootMap[u] = minRoot(rootMap[u], rootMap[to]);
                std::cout << "FF " << u << " " << to << " " << rootMap[u] << " " << rootMap[to] << "\n";
                std::cout << discoverTimeMap[rootMap[to]] << " " <<  discoverTimeMap[u] << "!!\n";
                if(discoverTimeMap[rootMap[to]] > discoverTimeMap[u])
                {
                    discoverBridge(e);
                }
            },

            DepthFirstSearchKeywords::notTreeEdge = [&](Edge e, Vertex u, Vertex to) {
                if(discoverTimeMap[u] >= discoverTimeMap[to])
                {
                    rootMap[u] = minRoot(rootMap[u], to);
                }
            },

            DepthFirstSearchKeywords::finishVertex = [&](Vertex u) {
                if(rootMap[u] == u)
                {
                    Vertex tmp;
                    do
                    {
                        tmp = stack.top();
                        stack.pop();
                        componentMap[tmp] = componentNumber;
                    }
                    while(tmp != u);
                    componentNumber ++;
                }
            }));
    return componentNumber;
}















} // namespace lz













//#include "depth_first_search.h"
//#include <algorithm>
//#include "graph_traits.h"
//#include "graph_utility.h"
//namespace lz {
//
//
//using std::min;
//
//	namespace BiconnectedComponentsOfEdgePrivate {
//
//		template<typename ComponentIterator>
//		struct Vis: public DFSVisitor
//		{
//			vector<int> dfn;
//			vector<int> low;
//			ComponentIterator c;
//			vector<int> stack;
//			int time_stmap, comp_num;
//
//			template<typename G, typename V>
//			void discoverVertex(const G &g, V u)
//			{
//				dfn[u] = low[u] = ++ time_stmap;
//				stack.push_back(u);
//
//			}
//			template<typename G, typename E, typename V>
//			void treeEdgeReturn(const G &g, E e, V u)
//			{
//				typename GraphTraits<G>::VertexDescriptor to = opposite(g, e, u);
//				low[u] = min(low[u], low[to]);
//			}
//			template<typename G, typename E, typename V>
//			void backEdge(const G &g, E e, V u)
//			{
//				typename GraphTraits<G>::VertexDescriptor to = opposite(g, e, u);
//				low[u] = min(low[u], dfn[to]);
//			}
//			template<typename G, typename V>
//			void finishVertex(const G &g, V u)
//			{
//				if(dfn[u] == low[u])
//				{
//					int cnt;
//					do
//					{
////						cout << " bad:: " << endl;
//						cnt = stack.back();
//						stack.pop_back();
//						c[cnt] = comp_num;
//					}
//					while(cnt != u);
//					comp_num ++;
//				}
//			}
//		};
//
//	}
//
//
//
//template<typename Graph, typename ComponentIterator>
//int BiconnectedComponentsOfEdge(const Graph &g, ComponentIterator c)
//{
//	BiconnectedComponentsOfEdgePrivate::Vis<ComponentIterator> vis;
////	cout << "UU" << endl;
//	int n = g.vertexNumber();
//	vis.dfn.resize(n);
//	vis.low.resize(n);
//	vis.c = c;
//	vis.time_stmap = 0;
//	vis.comp_num = 0;
//
////	cout << "UIOUO" << endl;
//	undirectedDFS(g, vis);
////	cout << "*************" << endl;
//	return vis.comp_num;
//}
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//} // namespace lz
//








#endif // BICONNECTED_COMPONENTS_OF_EDGE_H
