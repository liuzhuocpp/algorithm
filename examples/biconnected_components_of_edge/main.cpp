#include <iostream>
#include <cstdio>
#include <cstring>
#include <tuple>

#include "biconnected_components_of_edge.h"
#include "adjacency_list.h"

using namespace std;
using namespace lz;

typedef AdjacencyList<> Graph;



int main()
{
    
    Graph g;

    int n = 6;
    for(int i = 0; i < n; ++ i) g.addVertex();


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
