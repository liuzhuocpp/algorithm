
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
		addVertex(g);

	cout << string(100, '-') << "\n";
	int m = 4;
	addEdge(g, 1, 2);
	addEdge(g, 4, 0);
	addEdge(g, 3, 2);
	addEdge(g, 4, 2);
	cout << "edgeNumber " <<  edgesNumber(g) << endl;
//
	G::EdgeIterator ei, end_ei;
	for(tie(ei, end_ei) = edges(g); ei != end_ei; ++ ei)
	{
		cout << source(g, *ei) << " " << target(g, *ei) << endl;
	}
//
//
//
	cout << string(100, '-') << "\n";
	G::OutEdgeIterator oi4, end_oi4, oi0, end_oi0;
	tie(oi4, end_oi4) = outEdges(g, 4);
	tie(oi0, end_oi0) = outEdges(g, 0);
	cout << "source: 4\n";
	for(G::OutEdgeIterator it = oi4; it != end_oi4; it ++)
	{
		cout << source(g, *it) << " " << target(g, *it) << endl;
	}
	cout << "source: 0:\n";
	for(G::OutEdgeIterator it = oi0; it != end_oi0; it++)
	{
	    cout << source(g, *it) << " " << target(g, *it) << endl;
	}

	assert(((*(++oi4)) == *oi0));

//	cout << ((*(++oi4)) == *oi0) << endl;



//	cout << (*oi == *(oi +1)) << endl;









    return 0;
}











