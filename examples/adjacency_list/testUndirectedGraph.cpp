/*
 * testUndirectedGraph.cpp
 *
 *  Created on: 2015��11��10��
 *      Author: LZ
 */




/*
 * AdjacencyList test
 */
#include <lz/iterator_facade.h>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <tuple>
#include <vector>
#include <cassert>


#include "lz/adjacency_list.h"
#include "lz/map.h"
#include "lz/property.h"

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
	cout << "edgeNumber " <<  g.edgesNumber() << endl;
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
	G::OutEdgeIterator oi4, end_oi4, oi0, end_oi0;
	tie(oi4, end_oi4) = g.outEdges(4);
	tie(oi0, end_oi0) = g.outEdges(0);
	cout << "source: 4\n";
	for(G::OutEdgeIterator it = oi4; it != end_oi4; it ++)
	{
		cout << g.source(*it) << " " << g.target(*it) << endl;
	}
	cout << "source: 0:\n";
	for(G::OutEdgeIterator it = oi0; it != end_oi0; it++)
	{
	    cout << g.source(*it) << " " << g.target(*it) << endl;
	}

	assert(((*(++oi4)) == *oi0));

//	cout << ((*(++oi4)) == *oi0) << endl;



//	cout << (*oi == *(oi +1)) << endl;









    return 0;
}











