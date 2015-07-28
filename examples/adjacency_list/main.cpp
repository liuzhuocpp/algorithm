#include <iostream>
#include <cstdio>
#include <cstring>
#include <tuple>
#include "adjacency_list.h"
#include <vector>
using namespace std;
using namespace lz;

int main()
{
	vector<int> b;
    cout << "KK  uisdfsdf" << endl;
    typedef AdjacencyList<Undirected> G;
    G g;
    for(int i = 0; i < 10; ++ i) g.addVertex();





             
    g.addEdge(0, 1);
    g.addEdge(0, 5);
    g.addEdge(0, 9);
    g.addEdge(0, 8);

    cout << g.vertexNumber() << endl;
    cout << g.edgeNumber() << endl;

    cout << string(100, '-') << endl;

    G::OutEdgeIterator oe, oe_end;
    tie(oe, oe_end) = g.outEdges(0);
    for(;oe != oe_end; oe ++)
    {
        G::EdgeDescriptor ei = *oe;
        cout << g.target(ei) << endl;
    }


    return 0;
}
