
#include <bits/stdc++.h>
#include <lz/jump_pointers_lowest_common_ancestor.h>
#include <lz/adjacency_list.h>

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
    using G = AdjacencyList<UndirectedGraphTag>;
    using V = typename GraphTraits<G>::VertexDescriptor;
    G g(n);
    addEdge(g, 0, 1);
    addEdge(g, 1, 2);
    addEdge(g, 1, 3);
    addEdge(g, 1, 4);
    addEdge(g, 0, 5);

    outputOutEdges(g, 0);
    outputOutEdges(g, 1);



    auto getAns = jumpPointersLowestCommonAncestor(g);
    cout << getAns(0, 1) << endl;
    cout << getAns(5, 2) << endl;
    cout << getAns(4, 3) << endl;

    assert(getAns(0, 1) == 0);
    assert(getAns(5, 2) == 0);
    assert(getAns(4, 3) == 1);





	return 0;
}
