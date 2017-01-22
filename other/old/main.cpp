#include <iostream>
#include <cstdio>
#include <tuple>
#include "adjacency_list.h"
// #include "biconnected_components_of_point.h"
// #include "heavy_light_path.h"
// #include "segment_tree.h"
// #include "heap.h"

// #include "segment_tree.h"
// #include "dijkstra_shortest_paths.h"

// #include "adjacency_list.h"
// #include "multi_array.h"

// #include "multi_array.h"
// #include "adjacency_matrix.h"
// #include "kmp.h"

// #include "boost/multi_array.hpp"
// #include <boost/graph/adjacency_list.hpp>
// using namespace boost;
using namespace std;
using namespace lz;

// using namespace ns_AdjacencyMatrix;

int main()
{    
    // int n = 11, m = 11;

    // multi_array<bool, 3> d(extents[33][100][100]);
    // d.resize(extents[3][1][1]);
    // cout << d.size() << endl;
    // cout << d.num_elements() << endl;
    // cout << d.num_dimensions() << endl;

    // MultiArray<bool, 2> a(extents[11][11]);
    // a[1][1] = 1;

    // const MultiArray<bool, 2> b(a);
    // b[4][4] = 1;
    // int c = b[1][1];

    typedef AdjacencyMatrix<> G;
    G g(5);
    g.add(0, 1);
    g.add(0, 2);
    g.add(0, 3);
    g.add(0, 0);
    g.add(0, 4);
    g.add(0, 5);
    G::Iterator it ;
    
    it = g.begin(0);
    for(;it != g.end(0); ++ it)
    {
        cout << (*it).end << endl;
    }


    // EVG<NoProperty, NoProperty, NoProperty> t;
    // cout << sizeof(t) << endl;
    // cout << sizeof(MultiArray<int, 2>) << endl;


    // cout << "MY " << sizeof(AdjacencyList<>) << endl;
    // cout << "FF " << endl;
    // typedef adjacency_list<listS, vecS, directedS, 
    // no_property, property<edge_weight_t, int>, 
    // property<edge_weight_t, int> > Graph;
    // cout <<"Graph" << sizeof(Graph) << endl;

    // adjacency_list<> al;
    // cout << sizeof(al) << endl;

    // multi_array<int, 2> a;
    // a.resize(extents[44][44]);
    // cout << "FF " << endl;








	return 0;
}