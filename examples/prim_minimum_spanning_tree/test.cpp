
#include <bits/stdc++.h>
#include "lz/prim_minimum_spanning_tree.h"
#include "lz/adjacency_list.h"
using namespace lz;
using namespace std;



using EP = Property<EdgeWeightTag, int>;
using G = AdjacencyList<UndirectedGraphTag, NoProperty, EP>;

int main()
{

    namespace Keys = PrimMininumSpanningTreeKeywords;
    G g;
    int n = 3;
    for(int i = 0; i < n; ++ i)
        g.addVertex();

    g.addEdge(0, 1, EP(10)  );
    g.addEdge(1, 2, EP(20)  );
    g.addEdge(2, 0, EP(320)  );

    vector<int> treeEdgeMap(n, -2);


    cout << "ENTER" << endl;
    primMininumSpanningTree(g, (
            Keys::treeEdgeMap = makeIteratorMap(treeEdgeMap.begin())

    ));

    cout << string(100, '-') << endl;

    for(int i = 0; i < n; ++ i)
    {
        cout << treeEdgeMap[i] << endl;
        if(treeEdgeMap[i] != -1)
        cout << "treeEdge" << g.source(treeEdgeMap[i]) << " " <<g.target(treeEdgeMap[i]) << endl;
    }











    return 0;
}