#ifndef DEPTH_FIRST_SEARCH_H
#define DEPTH_FIRST_SEARCH_H

// #include "graph_utility.h"
#include <tuple>
#include <algorithm>
#include <vector>
#include <stack>
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
	typedef typename Graph::EdgeDescriptor EdgeDescriptor;
	typedef typename Graph::VertexDescriptor VertexDescriptor;
public:
	void initializeVertex(int u, const Graph &g) {}
	void startVertex(int u, const Graph &g){}
	void discoverVertex(int u, const Graph &g) {}
	void examineEdge(const EdgeDescriptor &e, const Graph &g) {}
	void treeEdge(const EdgeDescriptor &e, const Graph &g) {}
	void treeEdgeReturn(const EdgeDescriptor &e, const Graph &g) {}
	void backEdge(const EdgeDescriptor &e, const Graph &g) {}
	void forwardOrCrossEdge(const EdgeDescriptor &e, const Graph &g) {}
	void finishEdge(const EdgeDescriptor &e, const Graph &g) {}
	void finishVertex(int u, const Graph &g) {}
};

	namespace DepthFirstSearchPrivate {



	} // namespace DepthFirstSearchPrivate 


template<typename Graph, typename DFSVisitor, typename ColorIterator>
void directedDFS(const Graph &graph, DFSVisitor &vis, ColorMap color)
{
	
}

template<typename Graph>
class DirectedDFS
{
	typedef typename Graph::OutEdgeIterator OutEdgeIterator;
	typedef typename Graph::EdgeDescriptor EdgeDescriptor;
	typedef typename Graph::VertexDescriptor VertexDescriptor;
	vector<bool> vis;
	const Graph* m_graph;
public:
	// DirectedDFS(): m_graph(0){}
	// DirectedDFS(const Graph &g): m_graph(g){}

	bool isVisited(int u)	{ return vis[u]; }

	const Graph& graph() const { return *m_graph; }
	void setGraph(const Graph &g) { m_graph = &g; }

	void start(int s)
	{
		int n = m_graph->vertexNumber();
		vis.assign(n, 0);
		for(int i = 0; i < n; ++ i) initializeVertex(i);

		startVertex(s);
		dfs(s);
	}
	void start()
	{
		int n = m_graph->vertexNumber();
		vis.assign(n, 0);
		for(int i = 0; i < n; ++ i) initializeVertex(i);

		for(int i = 0; i < n; ++ i)
		{
			if(vis[i] == 0)
			{
				startVertex(i);
				dfs(i);
			}
		}
	}
protected:
	virtual void initializeVertex(int u) {}
	virtual void startVertex(int u){}
	virtual void discoverVertex(int u) {}
	virtual void examineEdge(const EdgeDescriptor &eid) {}
	virtual void treeEdge(const EdgeDescriptor &eid) {}
	virtual void treeEdgeReturn(const EdgeDescriptor &eid) {}
	virtual void notTreeEdge(const EdgeDescriptor &eid) {}	
	virtual void finishEdge(const EdgeDescriptor &eid) {}
	virtual void finishVertex(int u) {}

	
private:
	void dfs(int u)
	{		
		discoverVertex(u);
		vis[u] = 1;
		OutEdgeIterator oe, oe_end;
	    tie(oe, oe_end) = m_graph->outEdges(u);
	    
		for(;oe != oe_end; ++ oe)
		{
			EdgeDescriptor ei = *oe;
			int to = m_graph->target(ei);
			examineEdge(ei);

			if(vis[to] == 0) 
			{
				treeEdge(ei);
				dfs(to);
				treeEdgeReturn(ei);
			}
			else
			{
				notTreeEdge(ei);
			}			
			finishEdge(ei);
		}
		finishVertex(u);
	}



};

} // namesace lz
#endif // DEPTH_FIRST_SEARCH_H