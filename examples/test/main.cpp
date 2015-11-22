/*
 * main.cpp
 *
 *  Created on: 2015Äê11ÔÂ20ÈÕ
 *      Author: LZ
 */


#include <iostream>
#include <cstdio>
#include <cstring>
#include <tuple>
#include "lz/adjacency_list.h"
//#include "lz/depth_first_search.h"
//#include "lz/property.h"





#ifndef STRONG_COMPONENTS_H
#define STRONG_COMPONENTS_H

#include <utility>
#include <functional>
#ifndef DEPTH_FIRST_SEARCH_H
#define DEPTH_FIRST_SEARCH_H

#include <tuple>
#include <algorithm>
#include <vector>
#include <stack>
#include <type_traits>
#include <iostream>

#include "lz/graph_utility.h"
#include "lz/utility.h"
#include "lz/property.h"
#include "lz/map.h"

namespace lz {

using std::tie;
using std::pair;
using std::stack;
using std::vector;
using std::cout;
using std::endl;


	namespace DepthFirstSearchPrivate {






	template<typename G, typename Params, typename IndexMap, typename OutEdges, typename ColorMap> // colorMap found!
	static void dispatch1(const G &g, Params &&p, IndexMap i_map, OutEdges outEdges, ColorMap colorMap)
	{

	}
	template<typename G, typename Params, typename IndexMap, typename OutEdges>
	static void dispatch1(const G &g, Params &&p, IndexMap i_map, OutEdges outEdges, ParamNotFound)
	{
		std::vector<ColorTraits<>::Type> colors(g.vertexNumber(), p.white());
		auto colorMap = makeComposeMap(i_map, makeIteratorMap(colors.begin()));
		DepthFirstSearchPrivate::
		dispatch1(g, p, i_map, outEdges, colorMap);
	}


	} // namespace DepthFirstSearchPrivate









class DepthFirstSearchParams
{
public:
	template<typename V> void initializeVertex(V u) {}
	template<typename V> void startVertex(V u) {}
	template<typename V> void discoverVertex(V u) {}
	template<typename E, typename V> void examineEdge(E e, V u) {}
	template<typename E, typename V> void treeEdge(E e, V u) {}
	template<typename E, typename V> void treeEdgeReturn(E e, V u) {}
	template<typename E, typename V> void notTreeEdge(E e, V u) {}
	template<typename E, typename V> void finishEdge(E e, V u) {}
	template<typename V> void finishVertex(V u) {}

	auto white() ->decltype(ColorTraits<>::white()) { return ColorTraits<>::white(); }
	auto black() ->decltype(ColorTraits<>::black()) { return ColorTraits<>::black(); }



	ParamNotFound vertexIndexMap() {}
	template<typename V> ParamNotFound outEdges(V u) {}
	ParamNotFound colorMap() {}
	constexpr bool isInit() { return true; }
	ParamNotFound enterVertex(){}


};

template<typename G, typename DFSParams = DepthFirstSearchParams>
void depthFirstSearch(const G &g, DFSParams &&p = DepthFirstSearchParams())
{

	using V = typename GraphTraits<G>::VertexDescriptor;

	auto&& i_map = chooseParam(g.vertexPropertyMap(VertexIndexTag()),
							  p.vertexIndexMap() );

	auto outEdgesWrapper =[&](V u) {
		return chooseParam(g.outEdges(u), p.outEdges(u));
	};


	DepthFirstSearchPrivate::dispatch1(g, p, i_map, outEdgesWrapper, p.colorMap());

}








} // namesace lz
#endif // DEPTH_FIRST_SEARCH_H


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
	};



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























using namespace std;
using namespace lz;


using G = AdjacencyList<>;




int main()
{

	G g;
	int n = 5;
	for(int i = 0; i < n; ++ i)
		g.addVertex();
	g.addEdge(1, 2);
	g.addEdge(2, 3);
	g.addEdge(3, 4);
	g.addEdge(4, 1);

	vector<int> a(n);
	int comp = stronglyConnectedComponents(g, makeIteratorMap(a.begin()));

	cout << comp << endl;


//	P p(g);

//	depthFirstSearch(g, p);









	return 0;
}

















