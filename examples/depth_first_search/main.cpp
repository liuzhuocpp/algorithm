#include <iostream>
#include <cstdio>
#include <cstring>
#include <tuple>
#include "lz/adjacency_list.h"
#include "lz/depth_first_search.h"
#include "lz/property.h"

using namespace std;
using namespace lz;


using G = AdjacencyList<>;


struct P:public DFSParams<G>
{
	using V = GraphTraits<G>::VertexDescriptor;
	using E = typename GraphTraits<G>::EdgeDescriptor;

	G &g;
	P(G &g):g(g){}
	void treeEdge(E e, V u)
	{
		V other = opposite(g, e, u);
		cout << u << " " << other << endl;
	}
};

int main()
{


	G g;
	int n = 5;
	for(int i = 0; i < n; ++ i)
		g.addVertex();
	g.addEdge(1, 2);
	g.addEdge(2, 3);
	g.addEdge(3, 4);
	g.addEdge(1, 0);



	P p(g);

	depthFirstSearch(g, p);









	return 0;
}















