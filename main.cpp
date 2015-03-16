#include <iostream>
#include <cstdio>
#include <tuple>
// #include "adjacency_list.h"
// #include "biconnected_components_of_point.h"
// #include "heavy_light_path.h"
// #include "segment_tree.h"
// #include "heap.h"

#include "segment_tree.h"
#include "dijkstra_shortest_paths.h"

#include "adjacency_list.h"



using namespace std;
using namespace lz;

#define LL long long
const LL inf = 0x7fffffffffffffffLL;

struct Plus
{
	LL operator()(LL a, int b)
	{
		return a + b;
	}

};

int main()
{
	typedef AdjacencyList<NoProperty, tuple<LL> > Graph;
    Graph g;
    DijkstraShortestPaths<Graph, LL, 0, Plus> d;
    int n, m;
    scanf("%d%d", &n, &m);
    g.clear(n, 2 * m);
    while(m --)
    {
        int a, b, w;
        scanf("%d%d%d", &a, &b, &w);
        a --;
        b --;
        g.addBidirection(a, b, make_tuple(w));
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


	// cout << "FUCK " << endl;
	// AdjacencyList<> g(5);
	// cout << "FF " << endl;
	// g.add(0, 4);
	// g.add(0, 3);
	// g.add(0, 1);
	// g.add(0, 0);
	// AdjacencyList<>::Iterator it;

	// cout << sizeof(AdjacencyList<>) << endl;
	
	// for(it = g.begin(0); it != g.end(0); ++ it)
	// {
	// 	cout << it->end << endl;
	// }








	










	return 0;
}