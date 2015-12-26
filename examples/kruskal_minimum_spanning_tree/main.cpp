/*
 * main.cpp
 *
 *  Created on: 2015Äê12ÔÂ23ÈÕ
 *      Author: LZ
 */

#include <bits/stdc++.h>
#include "lz/kruskal_minimum_spanning_tree.h"
#include "lz/adjacency_list.h"
using namespace std;
using namespace lz;

//using G = AdjacencyList<UndirectedGraphTag>;
//G g;
//void display(int u)
//{
//	auto ei = g.outEdges(u);
//	for(auto i = ei.first; i != ei.second; ++ i)
//	{
//		cout << "edge: " << g.source(*i) << " " << g.target(*i) << "\n";
//	}
//}
//int main()
//{
//
//	for(int i = 0; i < 10; ++ i) g.addVertex();
//
//	g.addEdge(0, 1);
//
//	display(0);
//	cout << string(100, '|') << "\n";
//	display(1);
//
//	return 0;
//}




using EP = Property<EdgeWeightTag, int>;
using G = AdjacencyList<UndirectedGraphTag, NoProperty, EP>;

int main()
{

	G g;
	int n = 3;
	for(int i = 0; i < n; ++ i)
		g.addVertex();

	g.addEdge(0, 1, EP(10)  );
	g.addEdge(1, 2, EP(20)  );
	g.addEdge(2, 0, EP(320)  );
	kruskalMinimumSpanningTree(g,
		[&](auto e) ->void
        {
			cout <<  "| " << g.source(e) << " " <<  g.target(e) <<endl;
	    }

	);












	return 0;
}
