#include <iostream>
#include <cstdio>
#include <cstring>
#include <tuple>
#include <vector>


#include "lz/adjacency_list.h"
#include "lz/std_utility.h"


using namespace std;
using namespace lz;


int n = 10;
int main()
{
    AdjacencyList<> g(n);
    addEdge(g, 0, 1);
    addEdge(g, 0, 2);
    addEdge(g, 0, 3);
    addEdge(g, 0, 4);

    auto u = 0;
    cout << "U " << u << endl;
    for(auto to: adjacencyVertices(g, u) )
    {
        cout << to << " ";
    }

    cout << endl;






















    return 0;
}
















