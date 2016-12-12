
#include <bits/stdc++.h>
#include "lz/breadth_first_search.h"
#include "lz/adjacency_list.h"


using namespace lz;
using namespace std;

struct intTag{};
struct charTag{};

int n = 10;
int main()
{

    lz::Queue<int> q;




    using EP = Property<intTag, int,
                Property<charTag, char>>;
    using G = AdjacencyList<DirectedGraphTag, NoProperty, EP>;
    using Vertex = typename G::VertexDescriptor;
    using Edge = typename G::EdgeDescriptor;
    G g(n);

    g.addEdge(0, 1, EP(make_tuple(333, 'z')));
    g.addEdge(1, 2, EP(make_tuple(5, 'y')));
    g.addEdge(0, 2, EP(make_tuple(22, 'J')));

    using namespace BreadthFirstSearchKeywords;
    breadthFirstSearch(g, 0, (treeEdge = [](Edge e, Vertex source, Vertex target) {
        cout << "tree edge: " << source << " " << target << endl;


    }));







	return 0;
}
