#ifndef STRONG_COMPONENTS_H
#define STRONG_COMPONENTS_H

#include <utility>
#include <functional>

#include "lz/adjacency_list.h"
#include "lz/parameter.h"
#include "lz/depth_first_search.h"

#include "lz/map.h"
#include "lz/utility.h"
#include "lz/graph_utility.h"

namespace lz {

using std::swap;
using std::less;

namespace StronglyConnectedComponentsKeywords {

	LZ_PARAMETER_KEYWORD(tag, vertexIndexMap)
	LZ_PARAMETER_KEYWORD(tag, discoverTimeMap)
	LZ_PARAMETER_KEYWORD(tag, rootMap)
}


    namespace StronglyConnectedComponentsPrivate {




    }// namespace StronglyConnectedComponentsPrivate







    template<typename G, typename ComponentMap, typename ParamPack = lz::EmptyParamPack>

	auto stronglyConnectedComponents(const G &g, ComponentMap compMap, const ParamPack &p = lz::EmptyParamPack() )
    {
		static_assert(std::is_same<typename GraphTraits<G>::DirectedCategory, DirectedGraphTag>::value, "this graph is not directed");

		namespace k = StronglyConnectedComponentsKeywords;

		auto indexMap = p[k::vertexIndexMap | g.vertexPropertyMap(VertexIndexTag())];

		using Vertex = typename GraphTraits<G>::VertexDescriptor;
		using Edge = typename GraphTraits<G>::EdgeDescriptor;


		auto n = g.vertexNumber();

		auto discoverTimeMap =
				p[k::discoverTimeMap ||
				 std::bind(makeVertexIndexComposeMap<size_t, decltype(indexMap), decltype(n) >,
						   indexMap, n) ];

		auto rootMap =  p[k::rootMap ||
						  std::bind(makeVertexIndexComposeMap<Vertex, decltype(indexMap), decltype(n) >,
						  		    indexMap, n)];


		using ComponentType = typename std::decay<typename MapTraits<ComponentMap>::ValueType>::type;
		using TimeType = typename std::decay<typename MapTraits<decltype(discoverTimeMap)>::ValueType>::type;

		using namespace DepthFirstSearchKeywords;

		ComponentType compNumber = 0;
		TimeType dfsTime = 0;

		std::vector<Vertex> stack;
		std::vector<bool>inStack(n, 0);


		auto minRoot = [&](Vertex u, Vertex v) ->Vertex
		{
			auto ans =  discoverTimeMap[u] < discoverTimeMap[v] ? u : v;
			return ans;
		};


		depthFirstSearch(g, (

				colorMap = discoverTimeMap,
				white =  []()->TimeType { return 0; },
				black = [&]()->TimeType { return ++dfsTime; }
				,

				discoverVertex = [&](Vertex u)
				{
					rootMap[u] = u;
					stack.push_back(u);
					inStack[u] = 1;
				},

				treeEdgeReturn = [&](Edge e, Vertex u)
				{
					Vertex to = opposite(g, e, u);
					rootMap[u] = minRoot(rootMap[u], rootMap[to]);
				},

				notTreeEdge = [&](Edge e, Vertex u)
				{
					Vertex to = opposite(g, e, u);
					if(inStack[to])
					{
						rootMap[u] = minRoot(rootMap[u], to);
					}
				},

				finishVertex = [&](Vertex u)
				{
					if(u == rootMap[u])
					{
						Vertex tmp;
						do
						{
							tmp = stack.back();
							stack.pop_back();
							compMap[tmp] = compNumber;
							inStack[tmp] = 0;
						}
						while(tmp != u);
						++compNumber;
					}
				}
		));





		return compNumber;


    }





















//struct StronglyConnectedComponentsParams{
//
//	ParamNotFound vertexIndexMap(){}
//	ParamNotFound discoverTimeMap(){}
//	ParamNotFound rootMap(){}
//};
//
//
//
//    namespace StronglyConnectedComponentsPrivate {
//
//
//
//	template<typename G, typename ComponentMap, typename Params>
//	struct Impl: public DepthFirstSearchParams
//	{
//		using V = typename GraphTraits<G>::VertexDescriptor;
//		using E = typename GraphTraits<G>::EdgeDescriptor;
//
//		using VertexIndexMap = ChooseVertexIndexMap<G, decltype(&Params::vertexIndexMap)>;
//
//		using DiscoverTimeMap = ChooseVertexIndexComposeMap<decltype(&Params::discoverTimeMap), VertexIndexMap, size_t>;
//
//		using RootMap = ChooseVertexIndexComposeMap<decltype(&Params::rootMap), VertexIndexMap, V>;
//
//		using ComponentType = typename std::decay<typename MapTraits<ComponentMap>::ValueType>::type;
//		using TimeType = typename std::decay<typename MapTraits<DiscoverTimeMap>::ValueType>::type;
//
//		const G &g;
//		ComponentMap compMap;
//		Params &p;
//		DiscoverTimeMap discoverTimeMap;
//		RootMap rootMap;
//		vector<bool> inStack;
//		vector<V> stack;
//		ComponentType compNumber = 0;
//		TimeType dfsTime = 0;
//
//		Impl(const G &g,
//			 ComponentMap compMap,
//			 Params & p):g(g), compMap(compMap), p(p)
//		{
//			VertexIndexMap
//			indexMap = chooseParamReturnValue(
//						p.vertexIndexMap(),
//						g.vertexPropertyMap(VertexIndexTag()));
//			auto n = g.vertexNumber();
//
//			discoverTimeMap = chooseVertexIndexComposeMap<size_t>(p.discoverTimeMap(), indexMap, n);
//
//			rootMap = chooseVertexIndexComposeMap<V>(p.rootMap(), indexMap, n);
//
//			inStack.assign(n, 0);
//		}
//
//		DiscoverTimeMap colorMap() { return discoverTimeMap; }
//		TimeType white() { return 0; }
//		TimeType black()
//		{
//			return ++ dfsTime;
//		}
//		void discoverVertex(V u)
//		{
//			rootMap[u] = u;
//			stack.push_back(u);
//			inStack[u] = 1;
//		}
//		void treeEdgeReturn(E e, V u)
//		{
//			V to = opposite(g, e, u);
//			rootMap[u] = minRoot(rootMap[u], rootMap[to]);
//		}
//		void notTreeEdge(E e, V u)
//		{
//			V to = opposite(g, e, u);
//			if(inStack[to])
//			{
//
//				rootMap[u] = minRoot(rootMap[u], to);
//			}
//		}
//		void finishVertex(V u)
//		{
//			if(u == rootMap[u])
//			{
//				V tmp;
//				do
//				{
//					tmp = stack.back();
//					stack.pop_back();
//					compMap[tmp] = compNumber;
//					inStack[tmp] = 0;
//				}
//				while(tmp != u);
//				++compNumber;
//			}
//		}
//	private:
//		V minRoot(V u, V v)
//		{
//			return discoverTimeMap[u] < discoverTimeMap[v] ? u : v;
//		}
//	};
//
//    } // namespace StronglyConnectedComponentsPrivate
//
//    template<typename G, typename ComponentMap, typename Params = StronglyConnectedComponentsParams>
//
////    typename std::decay<typename MapTraits<ComponentMap>::ValueType>::type
//	auto stronglyConnectedComponents(const G &g, ComponentMap compMap, Params &&p = StronglyConnectedComponentsParams() )
//    {
//		static_assert(std::is_same<typename GraphTraits<G>::DirectedCategory, DirectedGraphTag>::value, "this graph is not directed");
//		using RealParams = typename std::remove_reference<Params>::type;
//
//		StronglyConnectedComponentsPrivate::Impl<G, ComponentMap, RealParams> impl(g, compMap, p);
//
//
//		depthFirstSearch(g, impl);
//
//
//		deleteVertexIndexComposeMap(impl.rootMap, p.rootMap());
//		deleteVertexIndexComposeMap(impl.discoverTimeMap, p.discoverTimeMap());
//		return impl.compNumber;
//
//    }



} // namespace lz




#endif // STRONG_COMPONENTS_H





















