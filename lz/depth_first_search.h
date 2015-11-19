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

		template<typename G, typename Params, typename ColorMap, typename OutEdges>
		void dfsImpl(const G &g, Params &p, ColorMap color, typename GraphTraits<G>::VertexDescriptor u, OutEdges outEdges)
		{
			p.discoverVertex(u);
			auto&& ei = outEdges(u);
			for(;ei.first != ei.second; ++ ei.first)
			{
				auto e = *ei.first;
				auto to = g.target(e);
				p.examineEdge(e, u);
				if(color[to] == p.white())
				{
					p.treeEdge(e, u);
					dfsImpl(g, p, color, to, outEdges);
					p.treeEdgeReturn(e, u);
				}
				else
				{
					p.notTreeEdge(e, u);
				}
				p.finishEdge(e, u);
			}
			color[u] = p.black();
			p.finishVertex(u);
		}

		struct DefaultParam{};

		template<bool con>  // default, no enter vertex
		struct ChooseEnterVertex
		{
			template<typename G, typename Params, typename ColorMap, typename OutEdges>
			static void run(const G &g, Params &p, ColorMap colorMap, OutEdges outEdges)
			{
				auto&& vi = g.vertices();
				for(;vi.first != vi.second; ++vi.first)
				{
					auto&& u = *vi.first;
					if(colorMap[u] == p.white())
					{
						p.startVertex(u);
						DepthFirstSearchPrivate::dfsImpl(g, p, colorMap, u, outEdges);
					}
				}
			}
		};
		template<> // has enter vertex
		struct ChooseEnterVertex<0>
		{
			template<typename G, typename Params, typename ColorMap, typename OutEdges>
			static void run(const G &g, Params &p, ColorMap colorMap, OutEdges outEdges)
			{
				auto u = p.enterVertex();
				p.startVertex(u);
				DepthFirstSearchPrivate::dfsImpl(g, p, colorMap, u, outEdges);
			}

		};



	} // namespace DepthFirstSearchPrivate








template<typename G>
class DFSParams
{
	using V = typename GraphTraits<G>::VertexDescriptor;
	using E = typename GraphTraits<G>::EdgeDescriptor;
	using DefaultParam = DepthFirstSearchPrivate::DefaultParam;
public:
	void initializeVertex(V u) {}
	void startVertex(V u) {}
	void discoverVertex(V u) {}
	void examineEdge(E e, V u) {}
	void treeEdge(E e, V u) {}
	void treeEdgeReturn(E e, V u) {}
	void notTreeEdge(E e, V u) {}
	void finishEdge(E e, V u) {}
	void finishVertex(V u) {}

	DefaultParam enterVertex(){}
	DefaultParam colorMap() {}

	auto white() ->decltype(ColorTraits<>::white()) { return ColorTraits<>::white(); }
	auto black() ->decltype(ColorTraits<>::black()) { return ColorTraits<>::black(); }
	bool isInit() { return true; }

	DefaultParam vertexIndexMap() {}
	DefaultParam outEdges(V u) {}
};

template<typename G, typename DFSParams>
void depthFirstSearch(const G &g, DFSParams &p)
{
	using DefaultParam = DepthFirstSearchPrivate::DefaultParam;
	using V = typename GraphTraits<G>::VertexDescriptor;

	auto&& i_map = ChooseValue<
				std::is_same<decltype(p.vertexIndexMap()), DefaultParam >::value,
				decltype(g.vertexPropertyMap(VertexIndexTag())),
				decltype(p.vertexIndexMap())>
				::get(g.vertexPropertyMap(VertexIndexTag()), p.vertexIndexMap()  );


	auto outEdgeWrapper =[&](V u) {
		return ChooseValue<
				std::is_same<decltype(p.outEdges(V())), DefaultParam >::value,
				decltype(g.outEdges(u)),
				decltype(p.outEdges(u))>
				::get(g.outEdges(u),
					  p.outEdges(u)  );

	};




	using VectorColor = std::vector<ColorTraits<>::Type>;

	constexpr bool isDefaultColorMap = std::is_same<decltype(p.colorMap()), DefaultParam>::value;

	VectorColor colors;

	if(isDefaultColorMap)
	{
		colors.assign(g.vertexNumber(), p.white());
	}

	using CI = VectorColor::iterator;

	auto default_color = makeComposeMap(i_map,
										IteratorMap<CI, typename CI::difference_type, typename CI::reference>(colors.begin())   );

	auto colorMap = ChooseValue<isDefaultColorMap, decltype(default_color), decltype(p.colorMap())>
						::get(default_color, p.colorMap());

	if(p.isInit())
	{
		auto&& vi = g.vertices();
		for(;vi.first != vi.second; ++vi.first)
		{
			auto&& u = *vi.first;
			colorMap[u] = p.white();
			p.initializeVertex(u);
		}
	}


	DepthFirstSearchPrivate::ChooseEnterVertex<std::is_same<decltype(p.enterVertex()), DefaultParam>::value>
	::run(g, p, colorMap, outEdgeWrapper);

}
	







} // namesace lz
#endif // DEPTH_FIRST_SEARCH_H
