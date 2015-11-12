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

namespace lz {

using std::tie;
using std::pair;
using std::stack;
using std::vector;
using std::cout;
using std::endl;
	namespace DepthFirstSearchPrivate {

		   
	} // namespace DepthFirstSearchPrivate



template<typename G>
class DFSVisitor
{
	using V = typename GraphTraits<G>::VertexDescriptor;
	using E = typename GraphTraits<G>::EdgeDescriptor;
public:
	void initializeVertex(const G &g, V u) {}
	void startVertex(const G &g, V u) {}
	void discoverVertex(const G &g, V u) {}
	void examineEdge(const G &g, E e, V u) {}
	void treeEdge(const G &g, E e, V u) {}
	void treeEdgeReturn(const G &g, E e, V u) {}
	void backEdge(const G &g, E e, V u) {}
	void forwardOrCrossEdge(const G &g, E e, V u) {}
	void finishEdge(const G &g, E e, V u) {}
	void finishVertex(const G &g, V u) {}
};



	namespace DepthFirstSearchPrivate {

		template<typename G, typename DFSVisitor, typename ColorMap>
		void dfsImpl(const G &g, DFSVisitor &vis, ColorMap color, typename G::VertexDescriptor u)
		{
			using OutEdgeIterator = typename GraphTraits<G>::OutEdgeIterator;
			using EdgeDescriptor = typename GraphTraits<G>::EdgeDescriptor;
			using ColorType = typename std::remove_reference<decltype(color[u])>::type;
			using Color = ColorTraits<ColorType>;

			color[u] = Color::gray();
			vis.discoverVertex(g, u);
			OutEdgeIterator ei, ei_end;
			tie(ei, ei_end) = g.outEdges(u);

			for(; ei != ei_end; ++ ei)
			{
				auto e = *ei;
				auto to = g.target(e);
				vis.examineEdge(g, e, u);
				if(color[to] == Color::white())
				{
					vis.treeEdge(g, e, u);
					dfsImpl(g, vis, color, to);
					vis.treeEdgeReturn(g, e, u);
				}
				else if(color[to] == Color::gray())
				{
					vis.backEdge(g, e, u);
				}
				else //if(color[to] == Color::black())
				{
					vis.forwardOrCrossEdge(g, e, u);
				}
				vis.finishEdge(g, e, u);
			}
			color[u] = Color::black();
			vis.finishVertex(g, u);
		}




//		template<typename Graph, typename DFSVisitor, typename ColorIterator>
//		void undfsImpl(const Graph &g,
//					   DFSVisitor &vis,
//					   ColorIterator color,
//					   typename Graph::VertexDescriptor u,
//					   bool is_start,
//					   typename Graph::EdgeDescriptor pre_e )
//		{
//			typedef typename Graph::OutEdgeIterator OutEdgeIterator;
//			typedef typename Graph::EdgeDescriptor EdgeDescriptor;
//			typedef typename Graph::VertexDescriptor VertexDescriptor;
//			color[u] = Color::Gray;
//			vis.discoverVertex(g, u);
//			OutEdgeIterator ei, ei_end;
//			tie(ei, ei_end) = g.outEdges(u);
//			for(; ei != ei_end; ++ ei)
//			{
//				EdgeDescriptor e = *ei;
//				VertexDescriptor to = opposite(g, e, u);
//
//				vis.examineEdge(g, e, u);
//				if(color[to] == Color::White)
//				{
//					vis.treeEdge(g, e, u);
//					undfsImpl(g, vis, color, to, 0, e);
//					vis.treeEdgeReturn(g, e, u);
//				}
//				else if(color[to] == Color::Gray && (is_start || e != pre_e))
//				{
//					vis.backEdge(g, e, u);
//				}
//				vis.finishEdge(g, e, u);
//			}
//			color[u] = Color::Black;
//			vis.finishVertex(g, u);
//		}





	} // namespace DepthFirstSearchPrivate 


template<typename G, typename DFSVisitor>
void depthFirstSearch(const G &g, DFSVisitor &vis, typename GraphTraits<G>::VertexDescriptor start)
{
	int n = g.vertexNumber();
	using Color = ColorTraits<>;
	using V = typename GraphTraits<G>::VertexDescriptor;

	vector<Color::Type> color(n, Color::white());
	for(int i = 0; i < n; ++ i) 
	{
		vis.initializeVertex(g, i);
	}

	auto vecToMap = [&](V u) ->Color::Type&  { return color[g.vertexPropertyMap(VertexIndexTag())[u]]; };
	auto color_map = makeFunctionMap(vecToMap);

	vis.startVertex(g, start);
	DepthFirstSearchPrivate::dfsImpl(g, vis, color_map, start);
	return ;

//	for(int i = 0; i < n; ++ i)
//	{
//		if(color[i] == Color::White)
//		{
//			vis.startVertex(g, i);
//			DepthFirstSearchPrivate::dfsImpl(g, vis, color.begin(), i);
//		}
//	}
}


	
//template<typename Graph, typename DFSVisitor>
//void undirectedDFS(const Graph &g, DFSVisitor &vis, int s = -1)
//{
//	int n = g.vertexNumber();
//	vector<Color> color(n, Color::White);
//
//	for(int i = 0; i < n; ++ i) vis.initializeVertex(g, i);
//	if(s >= 0)
//	{
//		vis.startVertex(g, s);
//		DepthFirstSearchPrivate::undfsImpl(g, vis, color.begin(), s, 1, typename Graph::EdgeDescriptor());
//		return ;
//	}
//	for(int i = 0; i < n; ++ i)
//	{
//		if(color[i] == Color::White)
//		{
////			cout << "F$$$FF" << endl;
//			vis.startVertex(g, i);
//			DepthFirstSearchPrivate::undfsImpl(g, vis, color.begin(), i, 1, typename Graph::EdgeDescriptor());
//		}
//	}
//}







} // namesace lz
#endif // DEPTH_FIRST_SEARCH_H
