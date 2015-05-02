#include <iostream>
#include <cstdio>
#include <cstring>
#include <tuple>
#include "adjacency_list.h"
#include "depth_first_search.h"

using namespace std;
using namespace lz;

template<typename G>
class DFS:public DepthFirstSearch<G>
{

protected:
    void initializeVertex(int u)
    {
        cout << "FS " << endl;
    }
    void 

};
int main()
{
    typedef AdjacencyList<> G;
    G g;

    g.assign(6);
    g.addEdge(0, 1);
    g.addEdge(0, 3);
    g.addEdge(0, 5);
    g.addEdge(0, 5);
    g.addEdge(0, 5);
    g.addEdge(0, 5);

    DFS<G> dfs;
    dfs.start(g);



    cout << "FF " << endl;

    G::OutEdgeIterator oe, oe_end;
    tie(oe, oe_end) = g.outEdges(0);
    for(;oe != oe_end; oe ++)
    {
        G::EdgeIndex ei = *oe;
        cout << g.source(ei) << " " << g.target(ei) << endl;
    }

    return 0;
}