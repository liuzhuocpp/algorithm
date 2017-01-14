#include <bits/stdc++.h>

#include <lz/dag_shortest_paths.h>
#include <lz/adjacency_list.h>
#include <lz/iterator_range.h>
using namespace lz;
using namespace std;


int main()
{
//    struct int_tag{};
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

    dagShortestPaths(g, (
            DagShortestPathsKeywords::distanceMap = lz::makeIteratorMap(dis.begin())
            ));

    for(int i = 0; i < n; ++ i)
    {
        cout <<"i="<< i << ": " <<  dis[i] << endl;
    }

    assert(dis[0] == 0);
    assert(dis[1] == 10);
    assert(dis[2] == 40);
    assert(dis[3] == 20);
    assert(dis[4] == std::numeric_limits<int>::max());





	return 0;
}

