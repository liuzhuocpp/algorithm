/*
 * test.cpp
 *
 *  Created on: 2016年12月8日
 *      Author: LZ
 */


#include <bits/stdc++.h>
#include "lz/dijkstra_shortest_paths.h"
#include "lz/adjacency_list.h"
using namespace lz;
using namespace std;



int n = 10;

int main()
{

    using EP = Property<EdgeWeightTag, int>;

    using G = AdjacencyList<UndirectedGraphTag, NoProperty, EP>;

    using E = typename GraphTraits<G>::EdgeDescriptor;
    using V = typename GraphTraits<G>::VertexDescriptor;
    G g(n);
    using namespace DijkstraShortestPathsKeywords;

    vector<int> p(n, -1);

    addEdge(g, 1, 0, EP(2));
    addEdge(g, 2, 1, EP(2));
    addEdge(g, 2, 0, EP(1));

    dijkstraShortestPaths(g, 0, (

            DijkstraShortestPathsKeywords::edgeRelaxed = [&](E e, V u, V to)
            {
                p[to] = u;
            }
          ));
    assert(p[0] == -1);
    assert(p[1] == 0);
    assert(p[2] == 0);
    assert(p[3] == -1);
    assert(p[4] == -1);
    assert(p[5] == -1);
    assert(p[6] == -1);
    assert(p[7] == -1);
    assert(p[8] == -1);
    assert(p[9] == -1);

    for(int i = 0; i < n; ++ i)
    {
        cout << "tree edge: " << i << " " << p[i] << endl;
    }







    return 0;
}
