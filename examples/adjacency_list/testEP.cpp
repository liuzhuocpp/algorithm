/*
 * testEP.cpp
 *
 *  Created on: 2015��11��10��
 *      Author: LZ
 */


#include <iostream>
#include <cstdio>
#include <cstring>
#include <tuple>
#include <vector>


#include "lz/adjacency_list.h"
#include "lz/map.h"
#include "lz/property.h"
#include "lz/std_utility.h"

// property graph test
using namespace std;
using namespace lz;

struct int_tag{};
struct char_tag{};
int main()
{


	using EP = Property<int_tag, int,
			   Property<char_tag, char> >;

	using G = AdjacencyList<UndirectedGraphTag, NoProperty, EP>;
	G g;

	int n = 5;
	for(int i = 0; i < n; ++ i)
	{
		g.addVertex();
	}



	int m = 4;
	g.addEdge(0, 1, EP(111, 'z'));
	g.addEdge(0, 3, EP(33333, 'a'));
	g.addEdge(2, 4, EP(1234, 'A'));
	g.addEdge(3, 5, EP(5412, '$'));


//	G::EdgeIterator ei, end_ei;
	for(auto e: edges(g))
	{
		cout << g.source(e) << " " << g.target(e) << endl;
	}
	cout << string(100, '-') << "\n";


	const G ng = g;
	auto mp = ng.edgePropertyMap(int_tag());

	for(auto e: outEdges(g, 0))
	{
		cout << ng.source(e) << " " << ng.target(e) << " " << mp[e] << endl;
	}















    return 0;
}
















