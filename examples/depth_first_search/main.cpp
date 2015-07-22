#include <iostream>
#include <cstdio>
#include <cstring>
#include <tuple>
#include "adjacency_list.h"
#include "depth_first_search.h"

using namespace std;
using namespace lz;

typedef AdjacencyList<> Graph;


struct Vis: DFSVisitor
{
    template<typename E, typename G>
    void treeEdge(E e, const G &g) const 
    {
        cout << "dfs: treeEdge: " <<  g.source(e) << " " << g.target(e) << endl;
    }
    template<typename E, typename G>
    void notTreeEdge(E e, const G &g) const 
    {
        cout << "dfs: notTreeEdge: " <<  g.source(e) << " " << g.target(e) << endl;
    }

};

int main()
{
    
    Graph g;

    int n = 6;
    g.assignVertex(n);


    g.addEdge(0, 5);
    g.addEdge(5, 0);

    g.addEdge(0, 5);
    g.addEdge(5, 0);









    Vis vis;
    // depthFirstSearch(g, vis);

    undirectedDFS(g, vis);




    return 0;
}