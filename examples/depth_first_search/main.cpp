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
    template<typename G, typename E>
    void treeEdge(const G &g, E e) const 
    {
        cout << "dfs: treeEdge: " <<  g.source(e) << " " << g.target(e) << endl;
    }
    template<typename G, typename E>
    void backEdge(const G &g, E e) 
    {
        cout << "dfs: backEdge: " <<  g.source(e) << " " << g.target(e) << endl;
    }
    template<typename G, typename E> 
    void forwardOrCrossEdge(const G &g, E e) 
    {
        cout << "dfs: forwardOrCrossEdge: " <<  g.source(e) << " " << g.target(e) << endl;
    }


};

int main()
{
    
    Graph g;

    int n = 6;
    // g.assignVertex(n);
    for(int i = 0; i < n; ++ i) g.addVertex();


    g.addEdge(0, 5);
    g.addEdge(5, 0);

    g.addEdge(0, 5);
    g.addEdge(5, 0);









    Vis vis;
    // depthFirstSearch(g, vis);

    undirectedDFS(g, vis);




    return 0;
}