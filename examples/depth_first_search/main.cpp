#include <iostream>
#include <cstdio>
#include <cstring>
#include <tuple>
#include "adjacency_list.h"
#include "depth_first_search.h"

using namespace std;
using namespace lz;

typedef AdjacencyList<> G;


struct Vis:DFSVisitor<G>
{
    
};

int main()
{
    
    G g;

    int n = 6;
    g.assignVertex(n);
    g.addEdge(0, 1);
    g.addEdge(0, 3);
    g.addEdge(0, 5);
    g.addEdge(0, 5);
    g.addEdge(0, 5);
    g.addEdge(0, 5);

    g.addEdge(1, 2);



    vector<int> a(n);
    Vis vis;
    depthFirstSearch(g, vis, a.begin());




    return 0;
}