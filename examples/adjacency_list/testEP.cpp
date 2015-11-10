/*
 * testEP.cpp
 *
 *  Created on: 2015Äê11ÔÂ10ÈÕ
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

// property graph test
using namespace std;
using namespace lz;

struct int_tag{};
struct char_tag{};
int main()
{
	using EP = Property<int_tag, int,
			   Property<char_tag, char> >;

	using G = AdjacencyList<DirectedGraphTag, NoProperty, EP>;
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


	G::EdgeIterator ei, end_ei;
	for(tie(ei, end_ei) = g.edges(); ei != end_ei; ++ ei)
	{
		cout << g.source(*ei) << " " << g.target(*ei) << endl;
	}
	cout << string(100, '-') << "\n";



	G::OutEdgeIterator oi, end_oi;
	tie(oi, end_oi) = g.outEdges(0);

	const G ng = g;
	auto mp = ng.edgePropertyMap(int_tag());
	for(; oi != end_oi; ++ oi)
	{
//		mp[*oi] = 66666;
		cout << ng.source(*oi) << " " << ng.target(*oi) << " " << mp[*oi] << endl;
	}















    return 0;
}
















