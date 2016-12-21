
#include <iostream>
#include <cstdio>
#include <cstring>
#include <tuple>
#include <ctime>
#include <cassert>

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

	int n = 6;
	for(int i = 0; i < n; ++ i)
		addVertex(g);

	addEdge(g, 1, 2);
	addEdge(g, 2, 3);
	addEdge(g, 3, 4);



	vector<int> a(n);

	auto num = connectedComponents(g, makeIteratorMap(a.begin()));

	assert(num == 3);

	assert(a[0] == 0);
	assert(a[1] == 1);
	assert(a[2] == 1);
	assert(a[3] == 1);
	assert(a[4] == 1);
	assert(a[5] == 2);

	cout << "NUM " << num << endl;


















	return 0;
}

















