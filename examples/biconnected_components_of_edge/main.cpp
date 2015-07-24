#include <iostream>
#include <cstdio>
#include <cstring>
#include <tuple>
#include "adjacency_list.h"
// #include "depth_first_search.h"
#include "biconnected_components_of_edge.h"
using namespace std;
using namespace lz;

typedef AdjacencyList<> Graph;



int main()
{
    
    Graph g;

    int n = 6;
    g.assignVertex(n);


    g.addEdge(0, 5);
    g.addEdge(5, 0);

    g.addEdge(0, 5);
    g.addEdge(5, 0);


    

    vector<int> v(n);
    int num = BiconnectedComponentsOfEdge(g, v.begin());
    cout << "biconnected_components_of_edge: " << num  << endl;

    for(int i = 0; i < n; ++ i)
    {
        cout << "i=" << i << ": " << v[i] << endl;
    }







    // depthFirstSearch(g, vis);

    // undirectedDFS(g, vis);





    return 0;
}