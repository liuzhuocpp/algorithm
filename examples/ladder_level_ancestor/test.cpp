#include <bits/stdc++.h>
#include <lz/ladder_level_ancestor.h>
#include <lz/adjacency_list.h>


using namespace lz;
using namespace std;


int n = 10;
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
    using G = AdjacencyList<UndirectedGraphTag>;
    using V = typename GraphTraits<G>::VertexDescriptor;
    G g(n);
    addEdge(g, 0, 1);
    addEdge(g, 1, 2);
    addEdge(g, 1, 3);
    addEdge(g, 1, 4);
    addEdge(g, 0, 5);

    addEdge(g, 5, 6);
    addEdge(g, 6, 7);
    addEdge(g, 7, 8);
    addEdge(g, 8, 9);



    auto getAns = ladderLevelAncestor(g);

    cout << getAns(3, 0) << endl;
    cout << getAns(2, 1) << endl;
    cout << getAns(9, 2) << endl;
    cout << getAns(9, 3) << endl;
    cout << getAns(9, 4) << endl;
    cout << getAns(9, 5) << endl;
    cout << getAns(9, 6) << endl;

    assert(getAns(3, 0) == 0);
    assert(getAns(2, 1) == 1);
    assert(getAns(9, 2) == 6);
    assert(getAns(9, 3) == 7);
    assert(getAns(9, 4) == 8);
    assert(getAns(9, 5) == 9);
    assert(getAns(9, 6) == -1);







    return 0;
}
