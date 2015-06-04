#include <iostream>
#include <cstdio>
#include <cstring>
#include <tuple>
#include "adjacency_list.h"

using namespace std;
using namespace lz;

int main()
{

    cout << "KK " << endl;
    typedef AdjacencyList<> G;
    G g;
    g.assignVertices(10);
    g.addEdge(0, 1);
    g.addEdge(0, 3);
    g.addEdge(0, 5);
    g.addEdge(0, 5);
    g.addEdge(0, 5);
    g.addEdge(0, 5);
    g.addEdge(0, 9);
    g.addEdge(0, 8);


    G::OutEdgeIterator oe, oe_end;
    tie(oe, oe_end) = g.outEdges(0);
    for(;oe != oe_end; oe ++)
    {
        G::EdgeIndex ei = *oe;
        cout << g.target(ei) << endl;
    }

    return 0;
}