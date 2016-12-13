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

    using G = AdjacencyList<DirectedGraphTag, NoProperty, EP>;
    using Edge = G::EdgeDescriptor;
    using Vertex = G::VertexDescriptor;
    G g(n);
    using namespace DijkstraShortestPathsKeywords;

    vector<int> p(10, -1);

    g.addEdge(0, 1, EP(2));
    g.addEdge(1, 2, EP(2));
    g.addEdge(0, 2, EP(1));
    g.addEdge(2, 4, EP(98));

    g.addEdge(0, 3, EP(10));
    g.addEdge(3, 5, EP(33));
    g.addEdge(5, 6, EP(440));
    g.addEdge(6, 7, EP(110));
    g.addEdge(7, 8, EP(1340));
    g.addEdge(4, 8, EP(1055));
    g.addEdge(0, 8, EP(33));

    dijkstraShortestPaths(g, 0, (

            DijkstraShortestPathsKeywords::edgeRelaxed = [&](Edge e, Vertex u, Vertex to)
            {
                p[to] = u;
            }
          ));

    for(int i = 0; i < n; ++ i)
    {
        cout << "tree edge: " << i << " " << p[i] << endl;
    }


    assert(p[0] == -1);
    assert(p[1] ==  0);
    assert(p[2] ==  0);
    assert(p[3] ==  0);
    assert(p[4] ==  2);
    assert(p[5] ==  3);
    assert(p[6] ==  5);
    assert(p[7] ==  6);
    assert(p[8] ==  0);
    assert(p[9] ==  -1);










	return 0;
}
