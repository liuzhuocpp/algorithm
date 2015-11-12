#include <iostream>
#include <cstdio>
#include <cstring>
#include <tuple>
#include "lz/adjacency_list.h"
#include "lz/depth_first_search.h"

using namespace std;
using namespace lz;


using G = AdjacencyList<>;

struct Vis:public DFSVisitor<G>
{
	using V = GraphTraits<G>::VertexDescriptor;
	using E = typename GraphTraits<G>::EdgeDescriptor;
	void treeEdge(const G &g, E e, V u)
	{
		V other = opposite(g, e, u);
		auto idMap = g.vertexPropertyMap(VertexIndexTag());
		cout << idMap[u] << " " << idMap[other] << endl;
	}
};

int main()
{
	G g;
	int n = 5;
	for(int i = 0; i < n; ++ i) g.addVertex();
	g.addEdge(0, 1);
	g.addEdge(1, 2);
	g.addEdge(2, 3);
	g.addEdge(3, 4);
//	g.addEdge(2, 0);

	Vis vis;
	depthFirstSearch(g, vis, 0);











	return 0;
}















