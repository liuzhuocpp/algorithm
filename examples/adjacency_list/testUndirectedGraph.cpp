/*
 * testUndirectedGraph.cpp
 *
 *  Created on: 2015Äê11ÔÂ10ÈÕ
 *      Author: LZ
 */




/*
 * AdjacencyList test
 */
#include <iostream>
#include <cstdio>
#include <cstring>
#include <tuple>
#include <vector>


#include "lz/adjacency_list.h"
#include "lz/map.h"
#include "lz/property.h"
#include "lz/iterator.h"

using namespace std;
using namespace lz;


int main()
{
	using G = AdjacencyList<UndirectedGraphTag>;

	G g;

	int n = 5;
	for(int i = 0; i < n; ++ i)
		g.addVertex();

	cout << string(100, '-') << "\n";
	int m = 4;
	g.addEdge(1, 2);
	g.addEdge(4, 0);
	g.addEdge(3, 2);
	g.addEdge(4, 2);
	cout << "edgeNumber " <<  g.edgeNumber() << endl;
//
	G::EdgeIterator ei, end_ei;
	for(tie(ei, end_ei) = g.edges(); ei != end_ei; ++ ei)
	{
		cout << g.source(*ei) << " " << g.target(*ei) << endl;
	}
//
//
//
	cout << string(100, '-') << "\n";
	G::OutEdgeIterator oi, end_oi;
	tie(oi, end_oi) = g.outEdges(4);
	for(; oi != end_oi; ++ oi)
	{
		cout << g.source(*oi) << " " << g.target(*oi) << endl;
	}







    return 0;
}











