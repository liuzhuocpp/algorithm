#ifndef DEPTH_FIRST_SEARCH_H
#define DEPTH_FIRST_SEARCH_H

#include <tuple>
#include <algorithm>
#include <vector>
#include <stack>
#include <type_traits>
#include <iostream>
#include <memory>


#include "lz/utility.h"

#include "lz/graph_utility.h"
//#include "lz/utility.h"
//#include "lz/property.h"
#include "lz/map.h"
#include "lz/parameter.h"

namespace lz {

using std::tie;
using std::pair;
using std::stack;
using std::vector;
using std::cout;
using std::endl;



namespace DepthFirstSearchKeywords {

	LZ_PARAMETER_KEYWORD(tag, initializeVertex)
	LZ_PARAMETER_KEYWORD(tag, startVertex)
	LZ_PARAMETER_KEYWORD(tag, discoverVertex)
	LZ_PARAMETER_KEYWORD(tag, examineEdge)
	LZ_PARAMETER_KEYWORD(tag, treeEdge)
	LZ_PARAMETER_KEYWORD(tag, treeEdgeReturn)
	LZ_PARAMETER_KEYWORD(tag, notTreeEdge)
	LZ_PARAMETER_KEYWORD(tag, finishEdge)
	LZ_PARAMETER_KEYWORD(tag, finishVertex)


	LZ_PARAMETER_KEYWORD(tag, colorMap)
	LZ_PARAMETER_KEYWORD(tag, white)
	LZ_PARAMETER_KEYWORD(tag, black)

	LZ_PARAMETER_KEYWORD(tag, isInit)
	LZ_PARAMETER_KEYWORD(tag, vertexIndexMap)
	LZ_PARAMETER_KEYWORD(tag, outEdges)
	LZ_PARAMETER_KEYWORD(tag, enterVertex)
}



	namespace DepthFirstSearchPrivate {


	using namespace DepthFirstSearchKeywords;


	template<typename G, typename ParamPack>
	struct Impl
	{
		const G &g;
		const ParamPack &p;

		Impl(const G &g, const ParamPack &p):g(g), p(p)
		{
			init(p[isInit]);
			chooseEnterVertex(p[enterVertex]);
		}

		void dfs(typename GraphTraits<G>::VertexDescriptor u)
		{
			p[colorMap][u] = p[black]();
			p[discoverVertex](u);
			auto ei = p[outEdges](g, u);
			for(;ei.first != ei.second; ++ ei.first)
			{
				auto e = *ei.first;
				auto to = g.target(e);
				p[examineEdge](e, u);
				if(p[colorMap][to] == p[white]())
				{
					p[treeEdge](e, u);
					dfs(to);
					p[treeEdgeReturn](e, u);
				}
				else
				{
					p[notTreeEdge](e, u);
				}
				p[finishEdge](e, u);
			}
			p[finishVertex](u);
		}

		void init(std::true_type)
		{
			auto&& vi = g.vertices();
			for(;vi.first != vi.second; ++vi.first)
			{
				auto&& u = *vi.first;
				p[colorMap][u] = p[white]();
				p[initializeVertex](u);
			}
		}
		void init(std::false_type) {}


		template<typename EnterVertex>
		void chooseEnterVertex(EnterVertex u)
		{
			p[startVertex](u);
			dfs(u);
		}
		void chooseEnterVertex(ParamNotFound )
		{
			auto&& vi = g.vertices();
			for(;vi.first != vi.second; ++vi.first)
			{
				auto&& u = *vi.first;
				if(p[colorMap][u] == p[white]())
				{
					p[startVertex](u);
					dfs(u);
				}
			}
		}


	};

	}


template<typename G, typename ParamPack>
void depthFirstSearch(const G &g, const ParamPack &p)
{
	using namespace DepthFirstSearchKeywords;

	auto _white = p[white | ColorTraits<>::white];
	auto _black = p[black | ColorTraits<>::black];
	auto _isInit = p[isInit | std::true_type()];

	auto _outEdges = p[outEdges | std::mem_fn(&G::outEdges)];
	auto _vertexIndexMap = p[vertexIndexMap | g.vertexPropertyMap(VertexIndexTag())];

	using ColorValue = decltype(_white());

	auto n = g.vertexNumber();

	auto iMap = p[vertexIndexMap | g.vertexPropertyMap(VertexIndexTag())];

//	auto gg = std::bind(
//			makeVertexIndexComposeMap<ColorValue, decltype(iMap), decltype(n) >,
//			iMap, n);

	auto _colorMap = p[colorMap ||
					   std::bind(makeVertexIndexComposeMap<ColorValue, decltype(iMap), decltype(n) >,
					   			iMap, n)];
	auto _enterVertex = p[enterVertex];

	using V = typename GraphTraits<G>::VertexDescriptor;
	using E = typename GraphTraits<G>::EdgeDescriptor;

	auto actionVertex = [](V u){};
	auto actionEdge = [](E e, V u){};

	auto newParamPack = (
			initializeVertex = p[initializeVertex | actionVertex],
			startVertex = p[startVertex | actionVertex],
			discoverVertex = p[discoverVertex | actionVertex],
			examineEdge = p[examineEdge | actionEdge],
			treeEdge = p[treeEdge | actionEdge],
			treeEdgeReturn = p[treeEdgeReturn | actionEdge],
			notTreeEdge = p[notTreeEdge | actionEdge],
			finishEdge = p[finishEdge | actionEdge],
			finishVertex = p[finishVertex | actionVertex],

			white = _white,
			black = _black,
			isInit = _isInit,
			outEdges = _outEdges,
			vertexIndexMap = _vertexIndexMap,
			colorMap = _colorMap,
			enterVertex =  _enterVertex);


	DepthFirstSearchPrivate::Impl
	<G, decltype(newParamPack)>(g, newParamPack);






}





//	namespace DepthFirstSearchPrivate {
//
//	template<typename G, typename Params>
//	struct Impl
//	{
//		using V = typename GraphTraits<G>::VertexDescriptor;
//		using VertexIndexMap = ChooseVertexIndexMap<G, decltype(&Params::vertexIndexMap)>;
//		using DefaultColor = ColorTraits<>::Type;
//		using ColorMap = ChooseVertexIndexComposeMap<decltype(&Params::colorMap), VertexIndexMap, DefaultColor>;
//
//		VertexIndexMap indexMap;
//		ColorMap colorMap;
//		const G &g;
//		Params &p;
//		Impl(const G &g, Params &p):g(g), p(p)
//		{
//			indexMap = chooseParamReturnValue(p.vertexIndexMap(),
//											  g.vertexPropertyMap(VertexIndexTag()));
//			colorMap = chooseVertexIndexComposeMap<ColorTraits<>::Type>(p.colorMap(), indexMap, g.vertexNumber());
//		}
//		auto outEdges(V u)
//		{
//			return chooseParamReturnValue(p.outEdges(u), g.outEdges(u) );
//		};
//		void init(std::true_type)
//		{
//			auto&& vi = g.vertices();
//			for(;vi.first != vi.second; ++vi.first)
//			{
//				auto&& u = *vi.first;
//				colorMap[u] = p.white();
//				p.initializeVertex(u);
//			}
//		}
//		void init(std::false_type) {}
//
//		template<typename EnterVertex>
//		void chooseEnterVertex(EnterVertex u)
//		{
//			p.startVertex(u);
//			dfs(u);
//		}
//		void chooseEnterVertex(ParamNotFound )
//		{
//			auto&& vi = g.vertices();
//			for(;vi.first != vi.second; ++vi.first)
//			{
//				auto&& u = *vi.first;
//				if(colorMap[u] == p.white())
//				{
//					p.startVertex(u);
//					dfs(u);
//				}
//			}
//		}
//
//
//		void dfs(typename GraphTraits<G>::VertexDescriptor u)
//		{
//			colorMap[u] = p.black();
//			p.discoverVertex(u);
//			auto ei = outEdges(u);
//			for(;ei.first != ei.second; ++ ei.first)
//			{
//				auto e = *ei.first;
//				auto to = g.target(e);
//				p.examineEdge(e, u);
//				if(colorMap[to] == p.white())
//				{
//					p.treeEdge(e, u);
//					dfs(to);
//					p.treeEdgeReturn(e, u);
//				}
//				else
//				{
//					p.notTreeEdge(e, u);
//				}
//				p.finishEdge(e, u);
//			}
//			p.finishVertex(u);
//		}
//
//		void run()
//		{
//			init(typename Params::IsInit());
//			chooseEnterVertex(p.enterVertex());
//			deleteVertexIndexComposeMap(colorMap, p.colorMap());
//		}
//	};
//	} // namespace DepthFirstSearchPrivate
//
//class DepthFirstSearchParams
//{
//	using DefaultColor = ColorTraits<>::Type;
//public:
//	template<typename V> void initializeVertex(V u) {}
//	template<typename V> void startVertex(V u) {}
//	template<typename V> void discoverVertex(V u) {}
//	template<typename E, typename V> void examineEdge(E e, V u) {}
//	template<typename E, typename V> void treeEdge(E e, V u) {}
//	template<typename E, typename V> void treeEdgeReturn(E e, V u) {}
//	template<typename E, typename V> void notTreeEdge(E e, V u) {}
//	template<typename E, typename V> void finishEdge(E e, V u) {}
//	template<typename V> void finishVertex(V u) {}
//
//	DefaultColor white() { return ColorTraits<>::white(); }
//	DefaultColor black() { return ColorTraits<>::black(); }
//
//	using IsInit = std::true_type;
//	ParamNotFound vertexIndexMap() {}
//	template<typename V>
//	ParamNotFound outEdges(V u) {}
//	ParamNotFound colorMap() {}
//	ParamNotFound enterVertex(){}
//};
//
//template<typename G, typename Params>
//void depthFirstSearch(const G &g, Params &&p)
//{
//	using RealParams = typename std::remove_reference<Params>::type;
//
//	DepthFirstSearchPrivate::Impl<G, RealParams> impl(g, p);
//	impl.run();
//}



























} // namesace lz
#endif // DEPTH_FIRST_SEARCH_H
