#ifndef BICONNECTED_COMPONENTS_OF_EDGE_H
#define BICONNECTED_COMPONENTS_OF_EDGE_H


#include "depth_first_search.h"
#include <algorithm>

namespace lz {

using std::min;

	namespace BiconnectedComponentsOfEdgePrivate {

		template<typename ComponentIterator>
		struct Vis: public DFSVisitor
		{
			vector<int> dfn;
			vector<int> low;
			ComponentIterator c;
			vector<int> stack;
			int time_stmap, comp_num;

			template<typename V, typename G>
			void discoverVertex(V u, const G &g) 
			{
				dfn[u] = low[u] = ++ time_stmap;
				stack.push_back(u);
			}
			template<typename E, typename G>
			void treeEdgeReturn(E e, const G &g)
			{
				int u = g.source(e), to = g.target(e);
				low[u] = min(low[u], low[to]);
			}
			template<typename E, typename G>
			void notTreeEdge(E e, const G &g) 
			{
				int u = g.source(e), to = g.target(e);
				low[u] = min(low[u], dfn[to]);
			}
			template<typename V, typename G>
			void finishVertex(V u, const G &g)
			{
				if(dfn[u] == low[u])
				{
					int cnt;					
					do
					{
						cnt = stack.back();
						stack.pop_back();
						c[cnt] = comp_num;
					}
					while(cnt != u);
					comp_num ++;
				}
			}
		};

	}



template<typename Graph, typename ComponentIterator>
int BiconnectedComponentsOfEdge(const Graph &g, ComponentIterator c)
{

	BiconnectedComponentsOfEdgePrivate::Vis<ComponentIterator> vis;
	int n = g.vertexNumber();
	vis.dfn.resize(n);
	vis.low.resize(n);
	vis.c = c;
	vis.time_stmap = 0;
	vis.comp_num = 0;


	undirectedDFS(g, vis);
	return vis.comp_num;
}
















} // namespace lz









#endif // BICONNECTED_COMPONENTS_OF_EDGE_H