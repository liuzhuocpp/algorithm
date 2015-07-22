#ifndef BICONNECTED_COMPONENTS_OF_EDGE_H
#define BICONNECTED_COMPONENTS_OF_EDGE_H


#include "depth_first_search.h"


namespace lz {


	namespace BiconnectedComponentsOfEdgePrivate {

		template<typename G, typename DfnIterator, typename LowIterator, typename ComponentIterator>
		struct Vis: public DFSVisitor<G>
		{
			DfnIterator dfn;
			LowIterator low;
			ComponentIterator c;
			vector<int> stack;
			int time_stmap, comp_num;
			void init()
			{
				time_stmap = comp_num = 0;
				stack.clear();
			}
			void discoverVertex(int u, const Graph &g) 
			{
				dfn[u] = low[u] = ++ time_stmap;
				stack.push_back(u);
			}
			void treeEdgeReturn(const EdgeDescriptor &e, const Graph &g)
			{
				int u = g.source(e), to = g.target(e);
				low[u] = min(low[u], low[to]);
			}
			void notTreeEdge(const EdgeDescriptor &e, const Graph &g) 
			{
				int u = g.source(e), to = g.target(e);
				low[u] = min(low[u], dfn[to]);
			}
			void finishVertex(int u, const Graph &g) 
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

}
















} // namespace lz









#endif // BICONNECTED_COMPONENTS_OF_EDGE_H