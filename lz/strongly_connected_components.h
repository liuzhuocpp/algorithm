#ifndef STRONG_COMPONENTS_H
#define STRONG_COMPONENTS_H

#include <utility>
#include <functional>
#include "lz/depth_first_search.h"
#include "lz/map.h"
#include "lz/utility.h"
#include "lz/graph_utility.h"

namespace lz {

using std::swap;
using std::less;


struct StronglyConnectedComponentsParams{

	ParamNotFound vertexIndexMap(){}
	ParamNotFound discoverTimeMap(){}
	ParamNotFound rootMap(){}
};



    namespace StronglyConnectedComponentsPrivate {



	template<typename G, typename ComponentMap, typename Params>
	struct Impl: public DepthFirstSearchParams
	{
		using V = typename GraphTraits<G>::VertexDescriptor;
		using E = typename GraphTraits<G>::EdgeDescriptor;

		using VertexIndexMap = ChooseVertexIndexMap<typename std::add_const<G>::type, decltype(&Params::vertexIndexMap)>;

		using DiscoverTimeMap = ChooseVertexIndexComposeMap<decltype(&Params::discoverTimeMap), VertexIndexMap, size_t>;

		using RootMap = ChooseVertexIndexComposeMap<decltype(&Params::rootMap), VertexIndexMap, V>;

		using ComponentType = typename std::decay<typename MapTraits<ComponentMap>::ValueType>::type;
		using TimeType = typename std::decay<typename MapTraits<DiscoverTimeMap>::ValueType>::type;

		const G &g;
		ComponentMap compMap;
		Params &p;
		DiscoverTimeMap discoverTimeMap;
		RootMap rootMap;
		vector<bool> inStack;
		vector<V> stack;
		ComponentType compNumber = 0;
		TimeType dfsTime = 0;

		Impl(const G &g,
			 ComponentMap compMap,
			 Params & p):g(g), compMap(compMap), p(p)
		{
			VertexIndexMap
			indexMap = chooseParamReturnValue(
						p.vertexIndexMap(),
						g.vertexPropertyMap(VertexIndexTag()));
			auto n = g.vertexNumber();

			discoverTimeMap = chooseVertexIndexComposeMap<size_t>(p.discoverTimeMap(), indexMap, n);

			rootMap = chooseVertexIndexComposeMap<V>(p.rootMap(), indexMap, n);

			inStack.assign(n, 0);
		}

		DiscoverTimeMap colorMap() { return discoverTimeMap; }
		TimeType white() { return 0; }
		TimeType black()
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
			V to = opposite(g, e, u);
			if(inStack[to])
			{

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

    } // namespace StronglyConnectedComponentsPrivate

    template<typename G, typename ComponentMap, typename Params = StronglyConnectedComponentsParams>

    typename std::decay<typename MapTraits<ComponentMap>::ValueType>::type
	stronglyConnectedComponents(const G &g, ComponentMap compMap, Params &&p = StronglyConnectedComponentsParams() )
    {
		static_assert(std::is_same<typename GraphTraits<G>::DirectedCategory, DirectedGraphTag>::value, "this graph is not directed");
		using RealParams = typename std::remove_reference<Params>::type;
		StronglyConnectedComponentsPrivate::Impl<G, ComponentMap, RealParams> impl(g, compMap, p);
		depthFirstSearch(g, impl);
		deleteVertexIndexComposeMap(impl.rootMap, p.rootMap());
		deleteVertexIndexComposeMap(impl.discoverTimeMap, p.discoverTimeMap());
		return impl.compNumber;

    }



}




#endif // STRONG_COMPONENTS_H





















