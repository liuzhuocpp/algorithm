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




template<typename Graph>
class DFSVisitor
{
public:
	typedef typename Graph::EdgeDescriptor EdgeDescriptor;

	typedef typename Graph::VertexDescriptor VertexDescriptor;
public:
	void initializeVertex(int u, const Graph &g) {}
	void startVertex(int u, const Graph &g){}
	void discoverVertex(int u, const Graph &g) {}
	void examineEdge(const EdgeDescriptor &e, const Graph &g) {}
	void treeEdge(const EdgeDescriptor &e, const Graph &g) {}
	void treeEdgeReturn(const EdgeDescriptor &e, const Graph &g) {}
	void notTreeEdge(const EdgeDescriptor &e, const Graph &g) {}
	// void backEdge(const EdgeDescriptor &e, const Graph &g) {}
	// void forwardOrCrossEdge(const EdgeDescriptor &e, const Graph &g) {}
	void finishEdge(const EdgeDescriptor &e, const Graph &g) {}
	void finishVertex(int u, const Graph &g) {}
};



	namespace DepthFirstSearchPrivate {

		template<typename Graph, typename DFSVisitor, typename ColorMap>
		void dfsImpl(const Graph &g, DFSVisitor &vis, ColorMap color, int u)
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

	} // namespace DepthFirstSearchPrivate 


template<typename Graph, typename DFSVisitor, typename ColorIterator>
void depthFirstSearch(const Graph &g, DFSVisitor &vis, ColorIterator color, int s = -1)
{

	int n = g.vertexNumber();
	for(int i = 0; i < n; ++ i) 
	{
		color[i] = 0;
		vis.initializeVertex(i, g);
	}
	if(s >= 0)
	{
		vis.startVertex(s, g);
		DepthFirstSearchPrivate::dfsImpl(g, vis, color, s);
		return ;
	}

	for(int i = 0; i < n; ++ i)
	{
		if(color[i] == 0)
		{
			vis.startVertex(i, g);
			DepthFirstSearchPrivate::dfsImpl(g, vis, color, i);
		}
	}
}









} // namesace lz
#endif // DEPTH_FIRST_SEARCH_H