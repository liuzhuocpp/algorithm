

#include <bits/stdc++.h>
#include "lz/graph_utility.h"
#include "lz/adjacency_list.h"
#include "lz/parameter.h"
#include "lz/biconnected_components_of_edge.h"

using namespace std;
using namespace lz;

struct int_tag{};
using VP = Property<int_tag, int>;
using EP = Property<int_tag, int>;

using G = AdjacencyList<UndirectedGraphTag,  VP, EP>;





int main()
{


	G g;

	int n = 10;
	int m = 9;
	for(int i = 0; i < n; ++ i)
		addVertex(g);
	addEdge(g, 0, 1);
	addEdge(g, 1, 2);
	addEdge(g, 2, 3);
	addEdge(g, 3, 4);
	addEdge(g, 4, 1);

//	addEdge(g, 5, 6);
//	addEdge(g, 6, 7);
//	addEdge(g, 7, 8);
//	addEdge(g, 7, 8);


	vector<int> a(n, -1);


	auto comp = biconnectedComponentsOfEdge(g, makeIteratorMap(a.begin()), (
	        BiconnectedComponentsOfEdgeKeywords::discoverBridge = [](auto e) {
	            cout << "discoverBridge: " << e << endl;

	    }));

	for(int i = 0; i < n; ++ i)
    {
	    cout << "vertex: " <<  i << " biconnectedComponent: " << a[i] << endl;
    }

	cout << "biconnectedComponent number: " << comp << endl;


//	assert(comp == 5);
//	assert(a[0] == 1);
//	assert(a[1] == 0);
//	assert(a[2] == 0);
//	assert(a[3] == 0);
//	assert(a[4] == 0);
//	assert(a[5] == 4);
//	assert(a[6] == 3);
//	assert(a[7] == 2);
//	assert(a[8] == 2);










	return 0;
}

















