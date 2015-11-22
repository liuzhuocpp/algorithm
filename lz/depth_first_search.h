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

//	template<typename G, typename Params, typename ColorMap, typename OutEdges>
//	void dfsImpl(const G &g, Params &&p, ColorMap color, typename GraphTraits<G>::VertexDescriptor u, OutEdges outEdges)
//	{
//		color[u] = p.black();
//		p.discoverVertex(u);
//		auto&& ei = outEdges(u);
//		for(;ei.first != ei.second; ++ ei.first)
//		{
//			auto e = *ei.first;
//			auto to = g.target(e);
//			p.examineEdge(e, u);
//			if(color[to] == p.white())
//			{
//				p.treeEdge(e, u);
//				dfsImpl(g, p, color, to, outEdges);
//				p.treeEdgeReturn(e, u);
//			}
//			else
//			{
//				p.notTreeEdge(e, u);
//			}
//			p.finishEdge(e, u);
//		}
//		p.finishVertex(u);
//	}



//	template<typename G, typename Params, typename OutEdges, typename ColorMap, typename EnterVertex>
//	static void dispatch3(const G &g, Params &&p, OutEdges outEdges, ColorMap colorMap, EnterVertex u)
//	{
//
//		p.startVertex(u);
//		dfsImpl(g, p, colorMap, u, outEdges);
//	}
//	template<typename G, typename Params, typename OutEdges, typename ColorMap>
//	static void dispatch3(const G &g, Params &&p, OutEdges outEdges, ColorMap colorMap, ParamNotFound)
//	{
//		auto&& vi = g.vertices();
//		for(;vi.first != vi.second; ++vi.first)
//		{
//			auto&& u = *vi.first;
//			if(colorMap[u] == p.white())
//			{
//				p.startVertex(u);
//				dfsImpl(g, p, colorMap, u, outEdges);
//			}
//		}
//	}
//
//
//	template<typename G, typename Params, typename OutEdges, typename ColorMap, typename EnterVertex> // need init
//	static void dispatch2(const G &g, Params &&p, OutEdges outEdges, ColorMap colorMap, EnterVertex u, std::integral_constant<bool, true>)
//	{
//		auto&& vi = g.vertices();
//		for(;vi.first != vi.second; ++vi.first)
//		{
//			auto&& u = *vi.first;
//			colorMap[u] = p.white();
//			p.initializeVertex(u);
//
//		}
//		dispatch3(g, p, outEdges, colorMap, u);
//	}
//
//	template<typename G, typename Params, typename OutEdges, typename ColorMap, typename EnterVertex>
//	static void dispatch2(const G &g, Params &&p, OutEdges outEdges, ColorMap colorMap, EnterVertex u, std::integral_constant<bool, false>)
//	{
//		dispatch3(g, p, outEdges, colorMap, u);
//	}
//
//
//
//	template<typename G, typename Params, typename IndexMap, typename OutEdges, typename ColorMap> // colorMap found!
//	static void dispatch1(const G &g, Params &&p, IndexMap i_map, OutEdges outEdges, ColorMap colorMap)
//	{
//		DepthFirstSearchPrivate:: // i don`t know why must have this code
//		dispatch2(g, p, outEdges, colorMap, p.enterVertex(), std::integral_constant<bool, p.isInit()>());
//	}
//	template<typename G, typename Params, typename IndexMap, typename OutEdges>
//	static void dispatch1(const G &g, Params &&p, IndexMap i_map, OutEdges outEdges, ParamNotFound)
//	{
//		std::vector<ColorTraits<>::Type> colors(g.vertexNumber(), p.white());
//		auto colorMap = makeComposeMap(i_map, makeIteratorMap(colors.begin()));
//
//		DepthFirstSearchPrivate:: // i don`t know why must have this code
//		dispatch2(g, p, outEdges, colorMap, p.enterVertex(), std::integral_constant<bool, p.isInit()>());
//	}



//	template<typename Dispatch>



	template<typename G, typename Params>
	struct Dispatch
	{
		using V = typename GraphTraits<G>::VertexDescriptor;


		using ParamVertexIndexMap = typename std::result_of< decltype(&Params::vertexIndexMap)(Params)>::type;

		using DefaultVertexIndexMap = decltype( typename std::add_const<G>::type().vertexPropertyMap(VertexIndexTag()));

		using VertexIndexMap = typename std::conditional<std::is_same<ParamVertexIndexMap, ParamNotFound>::value,
														 DefaultVertexIndexMap,
														 ParamVertexIndexMap>::type;

		using ParamColorMap = typename std::result_of< decltype(&Params::colorMap)(Params)>::type;

		using DefaultColor = ColorTraits<>::Type;
		using DefaultColorMap = ComposeMap<VertexIndexMap, IteratorMap<DefaultColor*> >;

		using ColorMap = typename std::conditional<std::is_same<ParamColorMap, ParamNotFound>::value,
												   DefaultColorMap,
												   ParamColorMap>::type;





		VertexIndexMap indexMap;
		ColorMap colorMap;



		const G &g;
		Params &p;

		Dispatch(const G &g, Params &p):g(g), p(p)
		{
			indexMap = chooseParam(g.vertexPropertyMap(VertexIndexTag()),
			  	  	   p.vertexIndexMap() );

			colorMap = chooseColorMap(p.colorMap());
		}


		auto outEdges(V u)
		->decltype(chooseParam(g.outEdges(u), p.outEdges(u)))
		{
			return chooseParam(g.outEdges(u), p.outEdges(u));
		};

		template<typename ParamColorMap>
		ColorMap chooseColorMap(ParamColorMap colorMap)
		{
			return colorMap;
		}
		ColorMap chooseColorMap(ParamNotFound)
		{
			DefaultColor* colors = new DefaultColor[g.vertexNumber()];
			return makeComposeMap(indexMap, makeIteratorMap(colors) );
		}


		void init(std::integral_constant<bool, true>)
		{
			auto&& vi = g.vertices();
			for(;vi.first != vi.second; ++vi.first)
			{
				auto&& u = *vi.first;
				colorMap[u] = p.white();
				p.initializeVertex(u);
			}
		}
		void init(std::integral_constant<bool, false>) {}



		template<typename EnterVertex>
		void chooseEnterVertex(EnterVertex u)
		{
			p.startVertex(u);
			impl(u);
		}

		void chooseEnterVertex(ParamNotFound )
		{
			auto&& vi = g.vertices();
			int n = g.vertexNumber();
			for(;vi.first != vi.second; ++vi.first)
			{
				auto&& u = *vi.first;
				if(colorMap[u] == p.white())
				{
					p.startVertex(u);
					impl(u);
				}
			}
		}

		template<typename ParamColorMap>
		void deleteColorMap(ParamColorMap) {}

		void deleteColorMap(DefaultColorMap)
		{
			delete[] colorMap.secondMap().iterator();
		}

		void run()
		{
			init(p.isInit());
			chooseEnterVertex(p.enterVertex());
			deleteColorMap(p.colorMap());
		}




		void impl(typename GraphTraits<G>::VertexDescriptor u)
		{
			colorMap[u] = p.black();
			p.discoverVertex(u);
			auto ei = outEdges(u);
			for(;ei.first != ei.second; ++ ei.first)
			{
				auto e = *ei.first;
				auto to = g.target(e);
				p.examineEdge(e, u);
				if(colorMap[to] == p.white())
				{
					p.treeEdge(e, u);
					impl(to);
					p.treeEdgeReturn(e, u);
				}
				else
				{
					p.notTreeEdge(e, u);
				}
				p.finishEdge(e, u);
			}
			p.finishVertex(u);
		}







	};



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
	std::integral_constant<bool, true> isInit() const {}
	ParamNotFound enterVertex(){}


};

template<typename G, typename Params>
void depthFirstSearch(const G &g, Params &&p)
{
	using RealParams = typename std::remove_reference<Params>::type;


	DepthFirstSearchPrivate::Dispatch<G, RealParams> dispatch(g, p);
	dispatch.run();


//	using T = typename DepthFirstSearchPrivate::Dispatch<G, RealParams>::DefaultVertexIndexMap;
//	using Tar = typename GraphTraits<G>::template VertexPropertyMap<VertexIndexTag>::ConstType;
//
//	bool ans = std::is_same<T, Tar>::value;
//
//	cout << "ANS " << ans << endl;



//	using V = typename GraphTraits<G>::VertexDescriptor;
//
//	auto&& i_map = chooseParam(g.vertexPropertyMap(VertexIndexTag()),
//							  p.vertexIndexMap() );
//
//	auto outEdgesWrapper =[&](V u) {
//		return chooseParam(g.outEdges(u), p.outEdges(u));
//	};
//
//
//	DepthFirstSearchPrivate::dispatch1(g, p, i_map, outEdgesWrapper, p.colorMap());

}
	







} // namesace lz
#endif // DEPTH_FIRST_SEARCH_H
