#ifndef DEPTH_FIRST_SEARCH_H
#define DEPTH_FIRST_SEARCH_H

// #include "graph_utility.h"
#include <tuple>
#include <algorithm>
#include <vector>
#include <stack>
#include "graph_utility.h"
namespace lz {

using std::tie;
using std::pair;
using std::stack;
using std::vector;












// DirectedDFS
// UndirectedDFS
	namespace DepthFirstSearchPrivate {



	} // namespace DepthFirstSearchPrivate 




class DFSVisitor
{
public:
	template<typename V, typename G> void initializeVertex(V u, const G &g) {}
	template<typename V, typename G> void startVertex(V u, const G &g) {}
	template<typename V, typename G> void discoverVertex(V u, const G &g) {}
	template<typename E, typename G> void examineEdge(E e, const G &g) {}
	template<typename E, typename G> void treeEdge(E e, const G &g) {}
	template<typename E, typename G> void treeEdgeReturn(E e, const G &g) {}
	template<typename E, typename G> void notTreeEdge(E e, const G &g) {}
	template<typename E, typename G> void finishEdge(E e, const G &g) {}
	template<typename V, typename G> void finishVertex(V e, const G &g) {}
	// void backEdge(const EdgeDescriptor &e, const Graph &g) {}
	// void forwardOrCrossEdge(const EdgeDescriptor &e, const Graph &g) {}
	
	
};



	namespace DepthFirstSearchPrivate {

		template<typename Graph, typename DFSVisitor, typename ColorIterator>
		void dfsImpl(const Graph &g, DFSVisitor &vis, ColorIterator color, int u)
		{
			typedef typename Graph::OutEdgeIterator OutEdgeIterator;
			typedef typename Graph::EdgeDescriptor EdgeDescriptor;

			color[u] = 1;
			vis.discoverVertex(u, g);
			OutEdgeIterator ei, ei_end;

			tie(ei, ei_end) = g.outEdges(u);

			for(ei; ei != ei_end; ++ ei)
			{
				EdgeDescriptor e = *ei;				
				int to = g.target(e);
				vis.examineEdge(e, g);
				if(color[to] == 0)
				{
					vis.treeEdge(e, g);
					dfsImpl(g, vis, color, to);
					vis.treeEdgeReturn(e, g);
				}
				else
				{
					vis.notTreeEdge(e, g);
				}
				vis.finishEdge(u, g);
			}
			vis.finishVertex(u, g);
		}

		template<typename Graph, typename DFSVisitor, typename VC, typename EC>
		void undfsImpl(const Graph &g, DFSVisitor &vis, VC vc, EC ec, int u)
		{
			typedef typename Graph::OutEdgeIterator OutEdgeIterator;
			typedef typename Graph::EdgeDescriptor EdgeDescriptor;

			vc[u] = 1;			
			vis.discoverVertex(u, g);
			OutEdgeIterator ei, ei_end;

			tie(ei, ei_end) = g.outEdges(u);

			for(; ei != ei_end; ++ ei)
			{
				EdgeDescriptor e = *ei;
				if(ec[e]) continue;
				ec[e] = ec[g.reverseEdge(e)] = 1;

				int to = g.target(e);
				vis.examineEdge(e, g);
				if(vc[to] == 0)
				{
					vis.treeEdge(e, g);
					undfsImpl(g, vis, vc, ec, to);
					vis.treeEdgeReturn(e, g);
				}
				else
				{
					vis.notTreeEdge(e, g);
				}
				vis.finishEdge(u, g);
			}
			vis.finishVertex(u, g);
		}



	} // namespace DepthFirstSearchPrivate 


// template<typename Graph, typename DFSVisitor>
// void depthFirstSearch(const Graph &g, DFSVisitor &vis, int s = -1)
// {
// 	int n = g.vertexNumber();
// 	vector<int> color(n);
// 	for(int i = 0; i < n; ++ i) 
// 	{
// 		color[i] = 0;
// 		vis.initializeVertex(i, g);
// 	}
// 	if(s >= 0)
// 	{
// 		vis.startVertex(s, g);
// 		DepthFirstSearchPrivate::dfsImpl(g, vis, color.begin(), s);
// 		return ;
// 	}
// 	for(int i = 0; i < n; ++ i)
// 	{
// 		if(color[i] == 0)
// 		{
// 			vis.startVertex(i, g);
// 			DepthFirstSearchPrivate::dfsImpl(g, vis, color.begin(), i);
// 		}
// 	}
// }



template<typename Graph, typename DFSVisitor>
void undirectedDFS(const Graph &g, DFSVisitor &vis, int s = -1)
{
	int n = g.vertexNumber();
	vector<bool> vc(n, 0);
	vector<bool> ec(g.edgeNumber(), 0);

	for(int i = 0; i < n; ++ i) vis.initializeVertex(i, g);
	if(s >= 0)
	{
		vis.startVertex(s, g);
		DepthFirstSearchPrivate::undfsImpl(g, vis, vc.begin(), ec.begin(), s);
		return ;
	}
	for(int i = 0; i < n; ++ i)
	{
		if(vc[i] == 0)
		{			
			vis.startVertex(i, g);
			DepthFirstSearchPrivate::undfsImpl(g, vis, vc.begin(), ec.begin(), i);
		}
	}
}







} // namesace lz
#endif // DEPTH_FIRST_SEARCH_H