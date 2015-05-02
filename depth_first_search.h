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
template<typename Graph>
class DepthFirstSearch
{
	typedef typename Graph::OutEdgeIterator OutEdgeIterator;
	typedef typename Graph::EdgeIndex EdgeIndex;
	vector<bool> color;
public:
	DepthFirstSearch() = default;
	void start(const Graph &g, int s)
	{
		int n = g.vertexNumber();
		color.assign(n, 0);
		for(int i = 0; i < n; ++ i)
		{			
			initializeVertex(i);
		}
		startVertex(s);
		dfs(g, s);
	}
	void start(const Graph &g)
	{
		int n = g.vertexNumber();
		color.assign(n, 0);
		for(int i = 0; i < n; ++ i)
		{
			initializeVertex(i);
		}
		for(int i = 0; i < n; ++ i)
		{
			if(color[i] == 0)
			{
				startVertex(i);
				dfs(g, i);
			}
		}
	}
protected:
	virtual void initializeVertex(int u) {}
	virtual void startVertex(int u){}
	virtual void discoverVertex(int u) {}
	virtual void examineEdge(const EdgeIndex &eid) {}
	virtual void treeEdge(const EdgeIndex &eid) {}
	virtual void treeEdgeReturn(const EdgeIndex &eid) {}
	virtual void notTreeEdge(const EdgeIndex &eid) {}	
	virtual void finishEdge(const EdgeIndex &eid) {}
	virtual void finishVertex(int u) {}

	
private:
	void dfs(const Graph &g, int u)
	{		
		discoverVertex(u);
		color[u] = 1;

		OutEdgeIterator oe, oe_end;
		
	    tie(oe, oe_end) = g.outEdges(u);


		for(;oe != oe_end; ++ oe)
		{
			EdgeIndex ei = *oe;
			int to = g.target(ei);
			examineEdge(ei);

			if(color[to] == 0) 
			{
				treeEdge(ei);
				dfs(g, to);
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
	void dfsNotRecursive(int u, const Graph &g)
	{

	}



};

} // namesace lz
#endif // DEPTH_FIRST_SEARCH_H