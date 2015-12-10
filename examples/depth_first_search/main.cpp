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




struct P:public DepthFirstSearchVisitor
{
	using V = GraphTraits<G>::VertexDescriptor;
	using E = typename GraphTraits<G>::EdgeDescriptor;

	G &g;
	P(G &g):g(g){}

	void treeEdge(E e, V u)
	{
		V other = opposite(g, e, u);
		cout << "Tree Edge: " << u << " " << other << endl;
	}
	void notTreeEdge(E e, V u)
	{
		V other = opposite(g, e, u);
		cout <<"not Tree Edge: " <<  u << " " << other << endl;
	}

};

int main()
{
	struct int_tag{};
	struct char_tag{};
	Property<int_tag, int,
	Property<char_tag, char>>  pp(std::make_tuple(3333, '%'));
//	pp[int_tag()] = 111;
//	pp[char_tag()] = '$';
	cout << pp[int_tag()] << endl;
	cout << pp[char_tag()] << endl;












	G g;
	int n = 5;
	for(int i = 0; i < n; ++ i)
		g.addVertex();
	g.addEdge(1, 2);
	g.addEdge(2, 3);
	g.addEdge(3, 4);
	g.addEdge(1, 0);
	g.addEdge(4, 1);


	P p(g);

	using namespace DepthFirstSearchKeyword;
	depthFirstSearch(g,


			(visitor = p)

	);
















	return 0;
}















