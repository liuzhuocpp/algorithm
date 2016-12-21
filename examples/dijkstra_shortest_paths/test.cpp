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

    vector<int> p(n, -1);

    addEdge(g, 0, 1, EP(2));
    addEdge(g, 1, 2, EP(2));
    addEdge(g, 0, 2, EP(1));
    addEdge(g, 2, 4, EP(98));

    addEdge(g, 0, 3, EP(10));
    addEdge(g, 3, 5, EP(33));
    addEdge(g, 5, 6, EP(440));
    addEdge(g, 6, 7, EP(110));
    addEdge(g, 7, 8, EP(1340));
    addEdge(g, 4, 8, EP(1055));
    addEdge(g, 0, 8, EP(33));

    const auto inf = std::numeric_limits<int>::max();
    vector<int> distanceVec(n, inf);

    struct Marker
    {
        vector<int>::iterator mp;
        bool isMark(int i) const
        {
            return mp[i] != inf;
        }
        void mark(int i) {}
    }marker{distanceVec.begin()};
    auto distanceMap = makeIteratorMap(distanceVec.begin());
    dijkstraShortestPaths(g, 0, (

            DijkstraShortestPathsKeywords::marker = marker,
            DijkstraShortestPathsKeywords::distanceMap = distanceMap,
            DijkstraShortestPathsKeywords::edgeRelaxed = [&](Edge e, Vertex u, Vertex to)
            {
                cout << "ENTER" << endl;
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
