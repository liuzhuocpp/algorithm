#ifndef DEPTH_FIRST_SEARCH_H
#define DEPTH_FIRST_SEARCH_H

#include "graph_utility.h"

namesace lz {

template<typename Graph>
class DepthFirstSearch
{
	Graph *g;
	typedef typename Graph::EdgeProperty EP;
	typedef typename Graph::Iterator EdgeIterator;
	int s;
	vector<int> color;
public:
	explicit DepthFirstSearch(const Graph &_g):g(&_g){}
	Graph& graph() { return *g; }
	const Graph& graph() const { return *g; }
	void setGraph(const Graph &_g) { g = &_g; }
	void start(int s)
	{

	}
	virtual void initializeVertex(){}
	virtual void startVertex(){}
	virtual void discoverVertex(){}
	virtual void examineVdge(){}
	virtual void treeEdge(){}
	virtual void backEdge(){}
	virtual void forwardOrCrossEdge(){}
	virtual void finishEdge(){}
	virtual void finishVertex(){}
private:
	void dfs(int u)
	{
		discoverVertex(u);
		color[u] = 1;
		EdgeIterator it = g.begin(u);
		for(;it != g.end(u); ++ it)
		{
			int to = it->end;
			if(color[to] == 0) treeEdge();
			else if(color[to] == 1) backEdge();
			else if(color[to] == 2) forwardOrCrossEdge();
			finishEdge();
		}
		finishVertex(u);
	}



};

} // namesace lz
#endif // DEPTH_FIRST_SEARCH_H