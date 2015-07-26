#include <iostream>
#include <cstdio>
#include <cstring>
#include <tuple>


#include "adjacency_list.h"
#include "tarjan_lca.h"

using namespace std;
using namespace lz;





int main()
{
	typedef AdjacencyList<Undirected> Graph;
    Graph g;

    int n = 4;
    for(int i = 0; i < n; ++ i) g.addVertex();

    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(0, 3);

    int q = 2;

    int qa[] = {0, 2};
    int qb[] = {1, 3};
    int ans[q];

    cout << "SFSDF" << endl;
    tarjanLCA(g, qa, qb, q, ans);
    for(int i = 0; i < q; ++ i)
    {
    	cout << qa[i] << " " << qb[i] << " lca:" <<  ans[i] << endl;
    }








    return 0;
}
