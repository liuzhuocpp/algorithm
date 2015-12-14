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
#include <ctime>
#include "lz/graph_utility.h"
#include "lz/adjacency_list.h"

#include "lz/parameter.h"
#include "lz/strongly_connected_components.h"

using namespace std;
using namespace lz;

struct int_tag{};
using VP = Property<int_tag, int>;
using EP = Property<int_tag, int>;

using G = AdjacencyList<DirectedGraphTag,  VP, EP>;






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
//	g.addEdge(4, 1);

	vector<int> a(n);
//	cout << "FFFuuuuuuus " << endl;



	auto comp = stronglyConnectedComponents(g, makeIteratorMap(a.begin()));

	cout << comp << endl;









	return 0;
}

















