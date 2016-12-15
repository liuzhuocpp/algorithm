
#include <bits/stdc++.h>
#include "lz/heavy_path_decomposition.h"
#include "lz/adjacency_list.h"

using namespace lz;
using namespace std;

int n = 6;
template<typename G, typename V>
void outputOutEdges(const G &g,  V u)
{
    cout << "outEdges: " << u << ": " ;
    for(auto e: outEdges(g, u))
    {
        cout << opposite(g, e, u) << " , " ;
    }
    cout << endl;
}
int main()
{
    AdjacencyList<UndirectedGraphTag> g(n);
    addEdge(g, 0, 1);
    addEdge(g, 1, 2);
    addEdge(g, 1, 3);
    addEdge(g, 1, 4);
    addEdge(g, 0, 5);

    outputOutEdges(g, 0);
    outputOutEdges(g, 1);


//    int rootVertex = 0;

    auto getAns = heavyPathDecomposition(g);

    vector<pair<int, int>> a(5, make_pair(-2, -2));
//    getAns(3, 5, a.begin());
    getAns(3, 5, a.begin());


    for(int i = 0; i < a.size(); ++ i)
    {
        cout << a[i].first << " " << a[i].second << endl;
    }




	return 0;
}
