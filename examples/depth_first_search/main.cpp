#include <iostream>
#include <cstdio>
#include <cstring>
#include <tuple>
#include <list>
#include <algorithm>
#include <vector>
#include "lz/depth_first_search.h"


#include "lz/adjacency_list.h"



using namespace std;


using namespace lz;
using G = AdjacencyList<DirectedGraphTag>;

int main()
{

	G g;
	int n = 5;
	for(int i = 0; i < n; ++ i)
		g.addVertex();
	g.addEdge(0, 2);

//	auto e1 = *g.outEdges(0).first;
//	auto e2 = *g.outEdges(2).first;
//
//	cout << e1 << "++ " << e2 << endl;

	g.addEdge(2, 3);
	g.addEdge(0, 1);
	g.addEdge(1, 4);





	using namespace DepthFirstSearchKeywords;


	using V = GraphTraits<G>::VertexDescriptor;
	using E = typename GraphTraits<G>::EdgeDescriptor;

	auto _treeEdge = [&](E e, V u)
	{
		V other = opposite(g, e, u);
		cout << "Tree Edge!!: " //<< e << " "
				<< u << " " << other << endl;
	};
	auto _notTreeEdge = [&](E e, V u)
	{
		V other = opposite(g, e, u);
		cout <<"not Tree Edge: "  //<< e << " "
				<< u << " " << other << endl;
	};



//	cout << "OOO" << endl;
	depthFirstSearch(g,
			(
			isInit = std::true_type(),
			treeEdge = _treeEdge,
			notTreeEdge = _notTreeEdge,

			treeEdgeReturn = [&](E e, V u)
			{
				V other = opposite(g, e, u);
				cout <<"Tree Ruturn Edge: "  //<< e << " "
						<< u << " " << other << endl;
			},
			enterVertex = 0


//			,
//			isTree = std::true_type()

			)   );





























	return 0;
}















