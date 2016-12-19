

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
		addVertex(g);
	}



	int m = 4;
	addEdge(g, 0, 1, EP(111, 'z'));
	addEdge(g, 0, 3, EP(33333, 'a'));
	addEdge(g, 2, 4, EP(1234, 'A'));
	addEdge(g, 3, 5, EP(5412, '$'));

	for(auto e: edges(g))
	{
		cout << source(g, e) << " " << target(g, e) << endl;
	}
	cout << string(100, '-') << "\n";


	const G ng = g;
	auto mp = edgePropertyMap(ng, int_tag());

	for(auto e: outEdges(g, 0))
	{
		cout << source(ng, e) << " " << target(ng, e) << " " << mp[e] << endl;
	}






    return 0;
}
















