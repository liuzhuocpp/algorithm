#ifndef DEPTH_FIRST_SEARCH_H
#define DEPTH_FIRST_SEARCH_H

// #include "graph_utility.h"
#include <tuple>
#include <algorithm>

namespace lz {

using std::tie;
using std::pair;

template<typename Graph>
class DepthFirstSearch
{
	Graph *g;
	typedef typename Graph::OutEdgeIterator OutEdgeIterator;
	typedef typename Graph::EdgeIndex EdgeIndex;
	vector<bool> color;
public:
	explicit DepthFirstSearch():g(null_ptr){}
	explicit DepthFirstSearch(const Graph &_g):g(&_g){}

	const Graph& graph() const { return *g; }
	Graph& graph() { return *g; }
	

	virtual void initializeVertex(int u) {}
	virtual void startVertex(int u){}
	virtual void discoverVertex(int u) {}
	virtual void examineEdge(const EdgeIndex &eid) {}
	virtual void treeEdge(const EdgeIndex &eid) {}
	virtual void notTreeEdge(const EdgeIndex &eid) {}	
	virtual void finishEdge(const EdgeIndex &eid) {}
	virtual void finishVertex(int u) {}

	void start(int s = -1)
	{
		int n = g->vertexNumber();
		for(int i = 0; i < n; ++ i)
		{			
			initializeVertex(i);
			color[i] = 0;
		}

		if(s != -1) 
		{
			startVertex(s);
			dfs(s);
		}
		for(int i = 0; i < n; ++ i)
		{
			if(color[i] == 0)
			{
				startVertex(s);
				dfs(s);
			}
		}

	}
private:
	void dfs(int u)
	{		
		discoverVertex(u);
		color[u] = 1;
		OutEdgeIterator oe, oe_end;
	    tie(oe, oe_end) = g->outEdges(u);
		for(;oe != oe_end; ++ oe)
		{
			EdgeIndex ei = *oe;
			if(color[to] == 0) 
			{
				treeEdge(ei);				
				dfs(to);
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