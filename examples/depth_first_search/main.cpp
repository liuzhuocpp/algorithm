#include <iostream>
#include <cstdio>
#include <cstring>
#include <tuple>
#include "lz/depth_first_search.h"


#include "lz/adjacency_list.h"


#include "lz/property.h"

using namespace std;
using namespace lz;


using G = AdjacencyList<>;






struct AA
{
	void operator()()
	{
		cout << "FFF000000000" << endl;
	}
};

LZ_PARAMETER_KEYWORD(tag, aa)
LZ_PARAMETER_KEYWORD(tag, bb)
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
	g.addEdge(4, 1);

	using namespace DepthFirstSearchKeywords;


	using V = GraphTraits<G>::VertexDescriptor;
	using E = typename GraphTraits<G>::EdgeDescriptor;

//	G &g;
//	P(G &g):g(g){}

	auto _treeEdge = [&](E e, V u)
	{
		V other = opposite(g, e, u);
		cout << "Tree Edge: " << u << " " << other << endl;
	};
	auto _notTreeEdge = [&](E e, V u)
	{
		V other = opposite(g, e, u);
		cout <<"not Tree Edge: " <<  u << " " << other << endl;
	};


	depthFirstSearch(g,
			(
			isInit = std::true_type(),
			treeEdge = _treeEdge,
			notTreeEdge = _notTreeEdge
			)   );


//	P p(g);

	using namespace DepthFirstSearchKeywords;
//	depthFirstSearch(g,
//
//			(isInit = std::true_type())
////			(visitor = p)
//
//	);
















	return 0;
}















