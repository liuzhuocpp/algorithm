#include <iostream>
#include <cstdio>
#include "adjacency_list.h"
#include "biconnected_components_of_point.h"
#include "heavy_light_path.h"
#include "segment_tree.h"
// #include "heap.h"

#include "dijkstra_shortest_paths.h"

#include "adjacency_list.h"



using namespace std;
using namespace lz;

#define LL long long
const LL inf = 0x7fffffffffffffffLL;
int main()
{






	AdjacencyList<NoProperty, LL> g;
	DijkstraShortestPaths<LL> d;
	int n, m;
	scanf("%d%d", &n, &m);
	g.clear(n);
	while(m --)
	{
		int a, b, w;
		scanf("%d%d%d", &a, &b, &w);
		a --; 
		b --;
		g.addBidirectionalEdge(a, b, w);
	}
	d.calculate(g, 0, 0, inf);




	if(d.distance(n - 1) == inf)
	{
		printf("-1\n");
	}
	else
	{
		vector<int> o = d.getPath(n - 1);	
		for(int i = 0; i < o.size(); ++ i)
		{
			printf("%d ", o[i] + 1);
		}
		puts("");
	}
	
	










	return 0;
}