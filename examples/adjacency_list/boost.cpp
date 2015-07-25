
#include <iostream>



#include <boost/graph/adjacency_list.hpp>

using namespace std;

using namespace boost;


int main()
{
    typedef adjacency_list<vecS, vecS, undirectedS>  G;

    G g;


    add_vertex(g);
    add_vertex(g);

    add_edge(0, 1, g);

    graph_traits<G>::out_edge_iterator  oi, oi_end;
    graph_traits<G>::edge_descriptor  a, b;
    tie(oi, oi_end) = out_edges(0, g);
    a = *oi;

    for(;oi != oi_end; ++ oi)
    {
        cout << source(*oi, g) << " " << target(*oi, g) << endl;
    }

    cout << string(100, '-') << endl;
    tie(oi, oi_end) = out_edges(1, g);
    b = *oi;
    for(;oi != oi_end; ++ oi)
    {
        cout << source(*oi, g) << " " << target(*oi, g) << endl;
    }


    cout << "KK: " << (a == b) << endl;



    return 0;
}