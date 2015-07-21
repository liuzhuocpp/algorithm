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
    for(int i = 0; i < 10; ++ i) g.addVertex();


         
    g.addEdge(0, 1);
    g.addEdge(0, 5);
    g.addEdge(0, 9);
    g.addEdge(0, 8);


    G::OutEdgeIterator oe, oe_end;
    tie(oe, oe_end) = g.outEdges(0);
    for(;oe != oe_end; oe ++)
    {
        G::EdgeDescriptor ei = *oe;
        cout << g.target(ei) << endl;
    }

    G::EdgeIterator ei, ei_end;
    tie(ei, ei_end) = g.edges();
    for(;ei != ei_end; ei ++)
    {
        G::EdgeDescriptor _ei = *ei;
        cout << g.source(_ei) << " " << g.target(_ei) << endl;
    }

    return 0;
}