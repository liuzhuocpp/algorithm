
#include <bits/stdc++.h>
#include "lz/lowest_common_ancestor.h"
#include "lz/adjacency_list.h"

using namespace lz;
using namespace std;

struct EdgeVertexDescriptorTag {};

int main()
{
    int n = 10;
    AdjacencyList<UndirectedGraphTag> tree(n);
    addEdge(tree, 0, 1);
    addEdge(tree, 0, 5);
    addEdge(tree, 1, 2);
    addEdge(tree, 1, 3);
    addEdge(tree, 1, 4);

    addEdge(tree, 2, 6);
    addEdge(tree, 2, 7);
    addEdge(tree, 7, 8);
    addEdge(tree, 8, 9);

    using EP =Property<EdgeVertexDescriptorTag, int>;

    AdjacencyList<UndirectedGraphTag, NoProperty, EP> query(n);
    auto epm = edgePropertyMap(query, EdgeVertexDescriptorTag());
    addEdge(query, 0, 1);
    addEdge(query, 5, 0);
    addEdge(query, 5, 4);
    addEdge(query, 2, 4);
    addEdge(query, 6, 7);
    addEdge(query, 9, 3);
    addEdge(query, 9, 5);




    lowestCommonAncestor(tree, query, epm);

    for(auto e : edges(query))
    {
        cout << source(query, e) << " " << target(query, e) << " " << epm[e] << std::endl;
    }

    assert(epm[0] == 0);
    assert(epm[1] == 0);
    assert(epm[2] == 0);
    assert(epm[3] == 1);
    assert(epm[4] == 2);
    assert(epm[5] == 1);
    assert(epm[6] == 0);








	return 0;
}
