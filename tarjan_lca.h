/*
 * tarjan_lca.h
 *
 *  Created on: 2015Äê7ÔÂ26ÈÕ
 *      Author: LZ
 */

#ifndef TARJAN_LCA_H_
#define TARJAN_LCA_H_

#include <vector>
namespace lz {

using std::vector;

	namespace TarjanLCAPrivate {

	template<typename G, typename QueryList>
	struct Vis:public DFSVisitor
	{
		typedef typename GraphTraits<G>::VertexDescriptor V;
		typedef typename GraphTraits<G>::EdgeDescriptor E;

		QueryList q;
		vector<bool> color;
		vector<V> parent;
		void initializeVertex(const G &g, V u)
		{
			color[u] = 0;
			parent[u] = u;
		}
		void treeEdgeReturn(const G &g, E e, V u)
		{
			V to = opposite(g, e, u);
			parent[to] = u;
		}
		void finishVertex(const G &g, V u)
		{
			color[u] = 1;




		}

	};



	} // namespace TarjanLCAPrivate


template<typename Graph, typename InputIterator, typename OutIterator>
void tarjanLCA(const Graph &g, InputIterator q1, InputIterator q2, OutIterator ans)
{


}


} // namespace lz



#endif /* TARJAN_LCA_H_ */
