#include <bits/stdc++.h>

#include <lz/bellman_ford_shortest_paths.h>
#include <lz/adjacency_list.h>

using namespace lz;
using namespace std;

void testNoNegativeCycle()
{
    cout << __FUNCTION__ << " " << string(100, '-') << endl;
    using EP = Property<EdgeWeightTag, int>;

    using G = AdjacencyList<DirectedGraphTag, NoProperty, EP>;
    G g;
    int n = 10;
    for(auto i: irange(n))
    {
        addVertex(g);
    }

    addEdge(g, 0, 1, 10);
    addEdge(g, 0, 2, 110);
    addEdge(g, 0, 3, 20);
    addEdge(g, 1, 2, 30);
    vector<int> dis(n, -1);

    bool noNegCycle = bellmanFordShortestPaths(g, (
            BellmanFordShortestPathsKeywords::distanceMap = lz::makeIteratorMap(dis.begin())
            ));

    cout << "No negative cycle: " << noNegCycle << endl;

    for(int i = 0; i < n; ++ i)
    {
        cout <<"i="<< i << ": " <<  dis[i] << endl;
    }

    assert(noNegCycle == 1);
    assert(dis[0] == 0);
    assert(dis[1] == 10);
    assert(dis[2] == 40);
    assert(dis[3] == 20);
    assert(dis[4] == std::numeric_limits<int>::max());
}

void testHasNegativeCycle()
{
    cout << __FUNCTION__ << " " << string(100, '-') << endl;
    using EP = Property<EdgeWeightTag, int>;

    using G = AdjacencyList<DirectedGraphTag, NoProperty, EP>;
    G g;
    int n = 10;
    for(auto i: irange(n))
    {
        addVertex(g);
    }

    addEdge(g, 0, 1, 2);
    addEdge(g, 1, 2, 9);
    addEdge(g, 2, 3, 20);
    addEdge(g, 3, 1, -30);

    addEdge(g, 0, 4, 10);
    addEdge(g, 0, 5, 78);
    addEdge(g, 5, 6, 32);
    addEdge(g, 6, 7, 25);
    addEdge(g, 0, 7, 300);
    vector<int> dis(n, -1);

    bool noNegCycle = bellmanFordShortestPaths(g, (
            BellmanFordShortestPathsKeywords::distanceMap = lz::makeIteratorMap(dis.begin())
            ));

    cout << "No negative cycle: " << noNegCycle << endl;

    for(int i = 0; i < n; ++ i)
    {
        cout <<"i="<< i << ": " <<  dis[i] << endl;
    }


    assert(noNegCycle == 0);
    assert(dis[0] == 0);
    assert(dis[1] < 0);

    assert(dis[4] == 10);
    assert(dis[5] == 78);
    assert(dis[6] == 110);
    assert(dis[7] == 135);

    assert(dis[8] == std::numeric_limits<int>::max());
    assert(dis[9] == std::numeric_limits<int>::max());
}



int main()
{

    testNoNegativeCycle();
    testHasNegativeCycle();












	return 0;
}
