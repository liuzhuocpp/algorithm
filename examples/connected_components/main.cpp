/*
 * main.cpp
 *
 *  Created on: 2015Äê12ÔÂ16ÈÕ
 *      Author: LZ
 */





#include <iostream>
#include <cstdio>
#include <cstring>
#include <tuple>
#include <ctime>

#include "lz/adjacency_list.h"

#include "lz/connected_components.h"
#include "lz/parameter.h"


using namespace std;
using namespace lz;

struct int_tag{};
using VP = Property<int_tag, int>;
using EP = Property<int_tag, int>;

using G = AdjacencyList<UndirectedGraphTag,  VP, EP>;






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
	g.addEdge(3, 4);
	g.addEdge(3, 4);
	g.addEdge(3, 4);

	vector<int> a(n);

	auto num = connectedComponents(g, makeIteratorMap(a.begin()));

	cout << "NUM " << num << endl;


















	return 0;
}

















