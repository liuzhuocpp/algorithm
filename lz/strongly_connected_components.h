



#ifndef STRONG_COMPONENTS_H
#define STRONG_COMPONENTS_H

#include <utility>
#include <functional>
#include "lz/depth_first_search.h"

namespace lz {

using std::swap;
using std::less;


struct StronglyConnectedComponentsParams{

	ParamNotFound vertexIndexMap(){}
	ParamNotFound discoverTimeMap(){}
	ParamNotFound rootMap(){}
};



    namespace StronglyConnectedComponentsPrivate {

	template<typename ComponentMap>
	using ComponentType = typename std::decay<typename MapTraits<ComponentMap>::ValueType>::type;

	template<typename DiscoverTimeMap>
	using TimeType = typename std::decay<typename MapTraits<DiscoverTimeMap>::ValueType>::type;


	template<typename G, typename ComponentMap, typename DiscoverTimeMap, typename RootMap, typename Stack, typename InStack>
	struct Impl: public DepthFirstSearchParams
	{
		using V = typename GraphTraits<G>::VertexDescriptor;
		using E = typename GraphTraits<G>::EdgeDescriptor;
	public:
		const G &g;
		ComponentMap compMap;
		DiscoverTimeMap discoverTimeMap;
		RootMap rootMap;
		InStack& inStack;
		Stack& stack;
		ComponentType <ComponentMap> compNumber;
		TimeType<DiscoverTimeMap> dfsTime;

		Impl(const G &g,
			 ComponentMap compMap,
			 DiscoverTimeMap discoverTimeMap,
			 RootMap rootMap,
		 	 Stack& stack,
			 InStack& inStack,
		     ComponentType <ComponentMap> compNumber,
			 TimeType<DiscoverTimeMap> dfsTime):

				 g(g),
				 compMap(compMap),
				 discoverTimeMap(discoverTimeMap),
				 rootMap(rootMap),
				 stack(stack),
				 inStack(inStack),
				 compNumber(compNumber),
				 dfsTime(dfsTime){}



		DiscoverTimeMap colorMap() { return discoverTimeMap; }
		TimeType<DiscoverTimeMap> white() { return 0; }
		TimeType<DiscoverTimeMap> black()
		{
			return ++ dfsTime;
		}

		void discoverVertex(V u)
		{
			rootMap[u] = u;
			stack.push_back(u);
			inStack[u] = 1;
		}
		void treeEdgeReturn(E e, V u)
		{
			V to = opposite(g, e, u);
			rootMap[u] = minRoot(rootMap[u], rootMap[to]);
		}
		void notTreeEdge(E e, V u)
		{
			if(inStack[u])
			{
				V to = opposite(g, e, u);
				rootMap[u] = minRoot(rootMap[u], to);
			}

		}
		void finishVertex(V u)
		{
			if(u == rootMap[u])
			{
				V tmp;
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
	private:
		V minRoot(V u, V v)
		{
			return discoverTimeMap[u] < discoverTimeMap[v] ? u : v;
		}
	};

//	template<typename G, typename ComponentMap, typename DiscoverTimeMap, typename RootMap, typename Stack, typename InStack>
//	 g(g),
//	 compMap(compMap),
//	 discoverTimeMap(discoverTimeMap),
//	 rootMap(rootMap),
//	 stack(stack),
//	 inStack(inStack),
//	 compNumber(compNumber),
//	 dfsTime(dfsTime){}

	template<typename G, typename ComponentMap, typename Params, typename VertexIndexMap, typename DiscoverTimeMap, typename RootMap>

	ComponentType<ComponentMap>
	dispatch2(const G &g, ComponentMap compMap, Params && p,
			  VertexIndexMap i_map, DiscoverTimeMap discoverTimeMap, RootMap rootMap) // rootMap found
	{
		using Stack = std::vector<typename GraphTraits<G>::VertexDescriptor> ;
		using InStack = std::vector<bool> ;
		Stack stack;
		InStack inStack(g.vertexNumber(), 0);
		auto impl = Impl<G, ComponentMap, DiscoverTimeMap, RootMap, Stack, InStack>
					         (g, compMap, discoverTimeMap, rootMap, stack, inStack,  0, 0);
		depthFirstSearch(g, impl);
		return impl.compNumber;
	}

	template<typename G, typename ComponentMap, typename Params, typename VertexIndexMap, typename DiscoverTimeMap>
	ComponentType<ComponentMap>
	dispatch2(const G &g, ComponentMap compMap, Params && p,
			  VertexIndexMap i_map, DiscoverTimeMap discoverTimeMap, ParamNotFound)
	{
		std::vector<typename GraphTraits<G>::VertexDescriptor> vec(g.vertexNumber());
		auto rootMap = makeComposeMap(i_map, makeIteratorMap(vec.begin()));
		return dispatch2(g, compMap, p, i_map, discoverTimeMap, rootMap);
	}



	template<typename G, typename ComponentMap, typename Params, typename VertexIndexMap, typename DiscoverTimeMap>
	ComponentType<ComponentMap>
	dispatch1(const G &g, ComponentMap compMap, Params && p,
			  VertexIndexMap i_map, DiscoverTimeMap discoverTimeMap) // discoverTimeMap found
	{
		return dispatch2(g, compMap, p, i_map, discoverTimeMap, p.rootMap());
	}

	template<typename G, typename ComponentMap, typename Params, typename VertexIndexMap>
	ComponentType<ComponentMap>
	dispatch1(const G &g, ComponentMap compMap, Params && p,
			  VertexIndexMap i_map, ParamNotFound)
	{
		std::vector<size_t> vec(g.vertexNumber(), 0);
		auto discoverTimeMap = makeComposeMap(i_map, makeIteratorMap(vec.begin()));
		return dispatch1(g, compMap, p, i_map, discoverTimeMap);
	}



    } // namespace StronglyConnectedComponentsPrivate



    template<typename G, typename ComponentMap, typename Params = StronglyConnectedComponentsParams>

	StronglyConnectedComponentsPrivate::ComponentType<ComponentMap>
	stronglyConnectedComponents(const G &g, ComponentMap compMap, Params &&p = StronglyConnectedComponentsParams() )
    {
		static_assert(std::is_same<typename GraphTraits<G>::DirectedCategory, DirectedGraphTag>::value, "this graph is not directed");

		auto&& i_map = chooseParam(g.vertexPropertyMap(VertexIndexTag()),
								   p.vertexIndexMap() );


		return StronglyConnectedComponentsPrivate::dispatch1(g, compMap, p, i_map, p.discoverTimeMap());
    }



}




#endif // STRONG_COMPONENTS_H





















