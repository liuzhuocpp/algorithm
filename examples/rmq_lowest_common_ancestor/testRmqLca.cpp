#include <bits/stdc++.h>
#include <lz/rmq_lowest_common_ancestor.h>

#include "lz/adjacency_list.h"

using namespace lz;
using namespace std;


int main()
{
    int n = 10;
    AdjacencyList<UndirectedGraphTag> tree(n);
    addEdge(tree, 0, 1);
    addEdge(tree, 0, 5);
    addEdge(tree, 1, 2);
    addEdge(tree, 1, 3);
    addEdge(tree, 1, 4);

    addEdge(tree, 2, 6);
    addEdge(tree, 2, 7);

    addEdge(tree, 7, 8);
    addEdge(tree, 8, 9);


    auto lcaQuery = rmqLowestCommonAncestor(tree);

//    vector<int> lcaAns;
//    lcaAns.push_back(lcaQuery(0, 1));
//    lcaAns.push_back(lcaQuery(5, 0));
//    lcaAns.push_back(lcaQuery(5, 4));
//    lcaAns.push_back(lcaQuery(2, 4));
//    lcaAns.push_back(lcaQuery(6, 7));
//    lcaAns.push_back(lcaQuery(9, 3));
//    lcaAns.push_back(lcaQuery(9, 5));


    cout << lcaQuery(0, 1) << endl;
    cout << lcaQuery(5, 0) << endl;
    cout << lcaQuery(5, 4) << endl;
    cout << lcaQuery(2, 4) << endl;
    cout << lcaQuery(6, 7) << endl;
    cout << lcaQuery(9, 3) << endl;
    cout << lcaQuery(9, 5) << endl;


   assert(lcaQuery(0, 1) == 0);
   assert(lcaQuery(5, 0) == 0);
   assert(lcaQuery(5, 4) == 0);
   assert(lcaQuery(2, 4) == 1);
   assert(lcaQuery(6, 7) == 2);
   assert(lcaQuery(9, 3) == 1);
   assert(lcaQuery(9, 5) == 0);






    return 0;
}
