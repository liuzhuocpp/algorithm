#include <iostream>
#include <cstdio>
#include <cstring>
#include <tuple>

#include "depth_first_search.h"
#include "adjacency_list.h"
#include "tarjan_lca.h"

using namespace std;
using namespace lz;

typedef AdjacencyList<Undirected> G;
G g;
typedef typename GraphTraits<G>::VertexDescriptor V;
typedef typename GraphTraits<G>::EdgeDescriptor E;

struct Vis:public DFSVisitor
{
	void treeEdge(const G &g, E e, V u)
	{
		V to = opposite(g, e, u);
	}
	void backEdge(const G &g, E e, V u)
	{
		V to = opposite(g, e, u);
	}

};


int main()
{
	freopen("in.txt", "r", stdin);
	int n, m;
	scanf("%d%d", &n, &m);
	for(int i = 0; i < n; ++ i) g.addVertex();
	for(int i = 0; i < m; ++ i)
	{
		int a, b; scanf("%d%d", &a, &b);
		g.addEdge(a, b);
	}

	Vis vis;
	undirectedDFS(g, vis, 0);








	return 0;
}











