#ifndef DEPTH_FIRST_SEARCH_H
#define DEPTH_FIRST_SEARCH_H

#include <tuple>
#include <algorithm>
#include <vector>
#include <stack>

#include <iostream>
#include "graph_utility.h"
#include "graph_traits.h"
namespace lz {

using std::tie;
using std::pair;
using std::stack;
using std::vector;
using std::cout;
using std::endl;
	namespace DepthFirstSearchPrivate {

		   
} // namespace DepthFirstSearchPrivate 



	
class DFSVisitor
{
public:
	template<typename G, typename V> void initializeVertex(const G &g, V u) {}
	template<typename G, typename V> void startVertex(const G &g, V u) {}
	template<typename G, typename V> void discoverVertex(const G &g, V u) {}
	template<typename G, typename E, typename V> void examineEdge(const G &g, E e, V u) {}
	template<typename G, typename E, typename V> void treeEdge(const G &g, E e, V u) {}
	template<typename G, typename E, typename V> void treeEdgeReturn(const G &g, E e, V u) {}
	template<typename G, typename E, typename V> void backEdge(const G &g, E e, V u) {}
	template<typename G, typename E, typename V> void forwardOrCrossEdge(const G &g, E e, V u) {}
	template<typename G, typename E, typename V> void finishEdge(const G &g, E e, V u) {}
	template<typename G, typename V> void finishVertex(const G &g, V u) {}
	
};



	namespace DepthFirstSearchPrivate {

		template<typename Graph, typename DFSVisitor, typename ColorIterator>
		void dfsImpl(const Graph &g, DFSVisitor &vis, ColorIterator color, typename Graph::VertexDescriptor u)
		{
			typedef typename Graph::OutEdgeIterator OutEdgeIterator;
			typedef typename Graph::EdgeDescriptor EdgeDescriptor;

			color[u] = Color::Gray;
			vis.discoverVertex(g, u);
			OutEdgeIterator ei, ei_end;
			tie(ei, ei_end) = g.outEdges(u);

			for(; ei != ei_end; ++ ei)
			{
				EdgeDescriptor e = *ei;
				typename Graph::VertexDescriptor to = g.target(e);
				vis.examineEdge(g, e, u);
				if(color[to] == Color::White)
				{
					vis.treeEdge(g, e, u);
					dfsImpl(g, vis, color, to);
					vis.treeEdgeReturn(g, e, u);
				}
				else if(color[to] == Color::Gray)
				{
					vis.backEdge(g, e, u);
				}
				else //if(color[to] == Color::Black)
				{
					vis.forwardOrCrossEdge(g, e, u);
				}
				vis.finishEdge(g, e, u);
			}
			color[u] = Color::Black;
			vis.finishVertex(g, u);
		}




		template<typename Graph, typename DFSVisitor, typename ColorIterator>
		void undfsImpl(const Graph &g, 
					   DFSVisitor &vis, 
					   ColorIterator color, 
					   typename Graph::VertexDescriptor u, 
					   bool is_start, 
					   typename Graph::EdgeDescriptor pre_e )
		{
			typedef typename Graph::OutEdgeIterator OutEdgeIterator;
			typedef typename Graph::EdgeDescriptor EdgeDescriptor;

			color[u] = Color::Gray;
			vis.discoverVertex(g, u);
			OutEdgeIterator ei, ei_end;
			tie(ei, ei_end) = g.outEdges(u);

			for(; ei != ei_end; ++ ei)
			{
				EdgeDescriptor e = *ei;
				typename Graph::VertexDescriptor to = opposite(g, e, u);
				vis.examineEdge(g, e, u);
				if(color[to] == Color::White)
				{
					// cout << "IIIII" << endl;
					vis.treeEdge(g, e, u);
					undfsImpl(g, vis, color, to, 0, pre_e);
					vis.treeEdgeReturn(g, e, u);
				}
				else if(color[to] == Color::Gray && !is_start && e != pre_e)
				{
					vis.backEdge(g, e, u);
				}
				vis.finishEdge(g, e, u);
			}
			color[u] = Color::Black;
			vis.finishVertex(g, u);
		}





	} // namespace DepthFirstSearchPrivate 


template<typename Graph, typename DFSVisitor>
void depthFirstSearch(const Graph &g, DFSVisitor &vis, int s = -1)
{
	int n = g.vertexNumber();
	vector<Color> color(n, Color::White);
	for(int i = 0; i < n; ++ i) 
	{
		vis.initializeVertex(g, i);
	}
	if(s >= 0)
	{
		vis.startVertex(g, s);
		DepthFirstSearchPrivate::dfsImpl(g, vis, color.begin(), s);
		return ;
	}
	for(int i = 0; i < n; ++ i)
	{
		if(color[i] == Color::White)
		{
			vis.startVertex(g, i);
			DepthFirstSearchPrivate::dfsImpl(g, vis, color.begin(), i);
		}
	}
}


	
template<typename Graph, typename DFSVisitor>
void undirectedDFS(const Graph &g, DFSVisitor &vis, int s = -1)
{
	int n = g.vertexNumber();
	vector<Color> color(n, Color::White);	

	for(int i = 0; i < n; ++ i) vis.initializeVertex(g, i);
	if(s >= 0)
	{
		vis.startVertex(g, s);
		DepthFirstSearchPrivate::undfsImpl(g, vis, color.begin(), s, 1, typename GraphTraits<Graph>::EdgeDescriptor());
		return ;
	}
	for(int i = 0; i < n; ++ i)
	{
		if(color[i] == Color::White)
		{
			 cout << "FFF" << endl;
			vis.startVertex(g, i);
			DepthFirstSearchPrivate::undfsImpl(g, vis, color.begin(), i, 1, typename GraphTraits<Graph>::EdgeDescriptor());
		}
	}
}







} // namesace lz
#endif // DEPTH_FIRST_SEARCH_H
