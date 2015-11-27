/*
 * main.cpp
 *
 *  Created on: 2015Äê11ÔÂ20ÈÕ
 *      Author: LZ
 */


#include <iostream>
#include <cstdio>
#include <cstring>
#include <tuple>
#include "lz/adjacency_list.h"

#include "lz/strongly_connected_components.h"

using namespace std;
using namespace lz;


using G = AdjacencyList<>;


int main()
{

	G g;
	int n = 5;
	for(int i = 0; i < n; ++ i)
		g.addVertex();
	g.addEdge(1, 2);
	g.addEdge(2, 3);
	g.addEdge(3, 4);
	g.addEdge(4, 1);

	vector<int> a(n);
	cout << "FFF " << endl;
	int comp = stronglyConnectedComponents(g, makeIteratorMap(a.begin()));

	cout << comp << endl;


//	P p(g);

//	depthFirstSearch(g, p);









	return 0;
}

















