/*
 * tarjan_lca.h
 *
 *  Created on: 2015Äê7ÔÂ26ÈÕ
 *      Author: LZ
 */

#ifndef TARJAN_LCA_H_
#define TARJAN_LCA_H_

#include <vector>
#include "merge_find_set.h"
#include "adjacency_list.h"
#include "depth_first_search.h"
#include "graph_traits.h"


namespace lz {

using std::vector;

	namespace TarjanLCAPrivate {

	template<typename G, typename QueryList, typename OutputIterator>
	struct Vis:public DFSVisitor
	{
		typedef typename GraphTraits<G>::VertexDescriptor V;
		typedef typename GraphTraits<G>::EdgeDescriptor E;

		QueryList ql;
		vector<bool> color;
		MergeFindSet<> mfs;
		OutputIterator ans;
		Vis(int n, OutputIterator ans): ql(n), color(n, 0), mfs(n), ans(ans){}
		void treeEdgeReturn(const G &g, E e, V u)
		{
			V to = opposite(g, e, u);
			mfs.setParent(to, u);
		}
		void finishVertex(const G &g, V u)
		{
			color[u] = 1;
			typename GraphTraits<G>::OutEdgeIterator oi, oi_end;
			tie(oi, oi_end) = ql.outEdges(u);
			for(;oi != oi_end; ++ oi)
			{
				E e = *oi;
				V to = opposite(ql, e, u);

				if(color[to])
				{
					ans[e] = mfs.find(to);
				}
			}
		}

	};



	} // namespace TarjanLCAPrivate


template<typename Graph, typename InputIterator, typename OutputIterator>
void tarjanLCA(const Graph &g,
			   typename GraphTraits<Graph>::VertexDescriptor root,
			   InputIterator qa,
			   InputIterator qb,
			   int q,
			   OutputIterator ans)
{
	int n = g.vertexNumber();

	TarjanLCAPrivate::Vis<Graph, AdjacencyList<Undirected>, OutputIterator> vis(n, ans);
//	cout << "UROU" << endl;
	for(int i = 0; i < q; ++ i)
	{
		vis.ql.addEdge(*qa, *qb);
		++qa;
		++qb;
	}
	undirectedDFS(g, vis, root);




}


} // namespace lz



#endif /* TARJAN_LCA_H_ */
