#include <iostream>
#include <cstdio>
#include <tuple>
// #include "adjacency_list.h"
// #include "biconnected_components_of_point.h"
// #include "heavy_light_path.h"
// #include "segment_tree.h"
// #include "heap.h"

// #include "segment_tree.h"
// #include "dijkstra_shortest_paths.h"

// #include "adjacency_list.h"
// #include "multi_array.h"
// #include "boost/multi_array.hpp"
#include "multi_array.h"

// using namespace boost;
using namespace std;
using namespace lz;



int main()
{
    int n = 6, m = 4;

    MultiArray<int, 2> ma(extents[n][m]);

    cout << "FFFFFFFFFFFFFFFFFFFFFFFF" << endl;

    for(int i = 0; i < n; ++ i)
    {
        for(int j = 0; j < m; ++ j)
        {
            ma[i][j] = -1;
            cout << ma[i][j] << endl;
        }
    }
    





	










	return 0;
}