#include <iostream>
#include <cstdio>
#include <cstring>
#include <tuple>
#include "adjacency_list.h"
#include "depth_first_search.h"

using namespace std;
using namespace lz;

typedef AdjacencyList<> G;


class DFS: public DirectedDFS<G>
{

protected:
    void initializeVertex(int u)
    {
        cout << "FS " << endl;
    }


};
int main()
{
    
    G g;

    g.assignVertex(6);
    g.addEdge(0, 1);
    g.addEdge(0, 3);
    g.addEdge(0, 5);
    g.addEdge(0, 5);
    g.addEdge(0, 5);
    g.addEdge(0, 5);

    DFS dfs;

    cout << "FF " << endl;
    dfs.setGraph(g);

    cout << "FF " << endl;
    dfs.start();



    cout << "FF " << endl;

    G::OutEdgeIterator oe, oe_end;
    tie(oe, oe_end) = g.outEdges(0);
    for(;oe != oe_end; oe ++)
    {
        G::EdgeDescriptor ei = *oe;
        cout << g.source(ei) << " " << g.target(ei) << endl;
    }

    return 0;
}