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

	template<typename G, typename Params>
	struct Impl
	{
		using V = typename GraphTraits<G>::VertexDescriptor;
		using VertexIndexMap = ChooseVertexIndexMap<typename std::add_const<G>::type, decltype(&Params::vertexIndexMap)>;
		using DefaultColor = ColorTraits<>::Type;
		using ColorMap = ChooseVertexIndexComposeMap<decltype(&Params::colorMap), VertexIndexMap, DefaultColor>;

		VertexIndexMap indexMap;
		ColorMap colorMap;
		const G &g;
		Params &p;
		Impl(const G &g, Params &p):g(g), p(p)
		{
			indexMap = chooseParamReturnValue(p.vertexIndexMap(),
											  g.vertexPropertyMap(VertexIndexTag()));
			colorMap = chooseVertexIndexComposeMap<ColorTraits<>::Type>(p.colorMap(), indexMap, g.vertexNumber());
		}
		auto outEdges(V u)
		->typename std::remove_reference<
			decltype(chooseParamReturnValue(p.outEdges(u), g.outEdges(u) )  )>::type
		{
			return chooseParamReturnValue(p.outEdges(u), g.outEdges(u) );
		};
		void init(std::true_type)
		{
			auto&& vi = g.vertices();
			for(;vi.first != vi.second; ++vi.first)
			{
				auto&& u = *vi.first;
				colorMap[u] = p.white();
				p.initializeVertex(u);
			}
		}
		void init(std::false_type) {}

		template<typename EnterVertex>
		void chooseEnterVertex(EnterVertex u)
		{
			p.startVertex(u);
			dfs(u);
		}
		void chooseEnterVertex(ParamNotFound )
		{
			auto&& vi = g.vertices();
			for(;vi.first != vi.second; ++vi.first)
			{
				auto&& u = *vi.first;
				if(colorMap[u] == p.white())
				{
					p.startVertex(u);
					dfs(u);
				}
			}
		}


		void dfs(typename GraphTraits<G>::VertexDescriptor u)
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
					dfs(to);
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

		void run()
		{
			init(typename Params::IsInit());
			chooseEnterVertex(p.enterVertex());
			deleteVertexIndexComposeMap(colorMap, p.colorMap());
		}
	};
	} // namespace DepthFirstSearchPrivate

class DepthFirstSearchParams
{
	using DefaultColor = ColorTraits<>::Type;
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

	DefaultColor white() { return ColorTraits<>::white(); }
	DefaultColor black() { return ColorTraits<>::black(); }

	using IsInit = std::true_type;
	ParamNotFound vertexIndexMap() {}
	template<typename V>
	ParamNotFound outEdges(V u) {}
	ParamNotFound colorMap() {}
	ParamNotFound enterVertex(){}
};

template<typename G, typename Params>
void depthFirstSearch(const G &g, Params &&p)
{
	using RealParams = typename std::remove_reference<Params>::type;

	DepthFirstSearchPrivate::Impl<G, RealParams> impl(g, p);
	impl.run();
}


} // namesace lz
#endif // DEPTH_FIRST_SEARCH_H
