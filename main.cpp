#include <iostream>
#include <cstdio>
// #include "adjacency_list.h"
// #include "biconnected_components_of_point.h"
// #include "heavy_light_path.h"
// #include "segment_tree.h"
// #include "heap.h"

// #include "dijkstra_shortest_paths.h"

#include "adjacency_list.h"



using namespace std;
using namespace lz;

#define LL long long
const LL inf = 0x7fffffffffffffffLL;
int main()
{
	cout << "FUCK " << endl;
	AdjacencyList<> g(5);
	cout << "FF " << endl;
	g.add(0, 4);
	g.add(0, 3);
	g.add(0, 1);
	g.add(0, 0);
	AdjacencyList<>::Iterator it;
	
	for(it = g.begin(0); it != g.end(0); ++ it)
	{
		cout << it->end << endl;
	}








	










	return 0;
}