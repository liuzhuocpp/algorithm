#include <bits/stdc++.h>

#include <boost/graph/adjacency_list.hpp>


using namespace std;
using namespace boost;

int n = 10;
int main()
{

    adjacency_list<vecS, vecS, undirectedS> g;
    for(int i = 0; i < n; ++ i)
        add_vertex(g);

    add_edge(0, 1, g);
//    add_edge(0, 1, g);
//    add_edge(0, 1, g);

    auto out0 = out_edges(0, g);
    cout << *out0.first << endl;

    auto out1 = out_edges(1, g);
    cout << *out1.first << endl;

    cout << (*out0.first == *out1.first) << endl;

    auto allEdges = edges(g);
    cout << *allEdges.first << endl;

    cout << num_edges(g) << endl;




	return 0;
}
