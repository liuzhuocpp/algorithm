
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
        addVertex(g);

    addEdge(g, 0, 1, EP(10)  );
    addEdge(g, 1, 2, EP(20)  );
    addEdge(g, 2, 0, EP(3)  );
    addEdge(g, 0, 2, EP(320)  );


    vector<int> treeEdgeMap(n, -1);
    int enterVertex = 0;

    struct Marker
    {
        vector<int>::iterator mp;
        bool isMark(int i) const
        {
            return mp[i] != -1;
        }
        void mark(int i)
        {
            if(i == 0) mp[i] = -2;
        }
    }marker{treeEdgeMap.begin()};



    primMininumSpanningTree(g, (
        Keys::marker = marker,
        Keys::treeEdgeMap = makeIteratorMap(treeEdgeMap.begin())

    ));

    for(int i = 0; i < n; ++ i)
    {
        if(treeEdgeMap[i] != -1 || treeEdgeMap[i] != -2)
        {
            cout << "treeEdge " << source(g, treeEdgeMap[i]) << " " <<target(g, treeEdgeMap[i]) << endl;
        }
    }

    assert( source(g, treeEdgeMap[1]) == 0);
    assert( target(g, treeEdgeMap[1]) == 1);

    assert( source(g, treeEdgeMap[2]) == 2);
    assert( target(g, treeEdgeMap[2]) == 0);











    return 0;
}
