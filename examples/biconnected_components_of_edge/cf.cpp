#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include "adjacency_list.h"
#include "depth_first_search.h"
#include "biconnected_components_of_edge.h"

using namespace std;
using namespace lz;
typedef AdjacencyList<Undirected> G;
typedef GraphTraits<G>::VertexDescriptor V;
typedef GraphTraits<G>::EdgeDescriptor E;

G g, ng;
struct Vis:public DFSVisitor
{
	void treeEdge(const G &g, E e, V u)
	{
		ng.addEdge(u, opposite(g, e, u));
	}
	void backEdge(const G &g, E e, V u)
	{
		ng.addEdge(u, opposite(g, e, u));
	}

};

int main()
{
	freopen("in.txt", "r", stdin);

	int n, m;
	scanf("%d%d", &n, &m);
	for(int i = 0; i < n; ++ i) g.addVertex(), ng.addVertex();
	while(m --)
	{
		int a, b;
		scanf("%d%d", &a, &b);
		a --;
		b --;

		g.addEdge(a, b);
	}
	vector<int> nid(n);
	BiconnectedComponentsOfEdge(g, nid.begin());
	bool same = 1;
	for(int i = 1; i < n; ++ i)
	{
		if(nid[i] != nid[0])
		{
			same = 0; break;
		}
	}



	if(same)
	{
		Vis vis;
		undirectedDFS(g, vis, 0);
		for(int i = 0; i < ng.edgeNumber(); ++ i)
		{
			printf("%d %d\n", ng.source(i) + 1, ng.target(i) + 1);
		}

	}
	else puts("0");



















	return 0;
}






