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









struct DepthFirstSearchVisitor
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
};

namespace DepthFirstSearchKeyword {

	LZ_PARAMETER_KEYWORD(tag, visitor)
	LZ_PARAMETER_KEYWORD(tag, colorMap)
	LZ_PARAMETER_KEYWORD(tag, white)
	LZ_PARAMETER_KEYWORD(tag, black)

	LZ_PARAMETER_KEYWORD(tag, isInit)
	LZ_PARAMETER_KEYWORD(tag, vertexIndexMap)
	LZ_PARAMETER_KEYWORD(tag, outEdges)
	LZ_PARAMETER_KEYWORD(tag, enterVertex)
}



	namespace DepthFirstSearchPrivate {


	using namespace DepthFirstSearchKeyword;


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
			p[visitor].discoverVertex(u);
			auto ei = p[outEdges](g, u);
			for(;ei.first != ei.second; ++ ei.first)
			{
				auto e = *ei.first;
				auto to = g.target(e);
				p[visitor].examineEdge(e, u);
				if(p[colorMap][to] == p[white]())
				{
					p[visitor].treeEdge(e, u);
					dfs(to);
					p[visitor].treeEdgeReturn(e, u);
				}
				else
				{
					p[visitor].notTreeEdge(e, u);
				}
				p[visitor].finishEdge(e, u);
			}
			p[visitor].finishVertex(u);
		}

		void init(std::true_type)
		{
			auto&& vi = g.vertices();
			for(;vi.first != vi.second; ++vi.first)
			{
				auto&& u = *vi.first;
				p[colorMap][u] = p[white]();
				p[visitor].initializeVertex(u);
			}
		}
		void init(std::false_type) {}


		template<typename EnterVertex>
		void chooseEnterVertex(EnterVertex u)
		{
			p[visitor].startVertex(u);
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
					p[visitor].startVertex(u);
					dfs(u);
				}
			}
		}






	};

	}


template<typename G, typename ParamPack>
void depthFirstSearch(const G &g, ParamPack &&p)
{
	namespace K = DepthFirstSearchKeyword;

	auto white = p[K::white | ColorTraits<>::white];
	auto black = p[K::black | ColorTraits<>::black];
	auto isInit = p[K::isInit | std::true_type()];
	auto visitor = p[K::visitor | DepthFirstSearchVisitor()];

	auto outEdges = p[K::outEdges | std::mem_fn(&G::outEdges)];
	auto vertexIndexMap = p[K::vertexIndexMap | g.vertexPropertyMap(VertexIndexTag())];

	using ColorValue = decltype(white());

	auto lazyColorMap = [&]() {
			auto color_array = std::unique_ptr<ColorValue[]>(new ColorValue[g.vertexNumber()]);

			return makeComposeMap(vertexIndexMap,
								  std::move(makeUniqueArrayMap( color_array )    ) );
		};

	auto colorMap = p[K::colorMap || lazyColorMap];




	auto newParamPack = (
			K::white = white,
			K::black = black,
			K::isInit = isInit,
			K::visitor = visitor,
			K::outEdges = outEdges,
			K::vertexIndexMap = vertexIndexMap,
			K::colorMap = colorMap,
			K::enterVertex = p[K::enterVertex] );

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
