#include <iostream>
#include <cstdio>
#include <cstring>
#include <tuple>
#include <list>
#include <algorithm>
#include <vector>
#include <cassert>
#include "lz/depth_first_search.h"


#include "lz/adjacency_list.h"
#include "lz/utility.h"



using namespace std;


using namespace lz;


using namespace DepthFirstSearchKeywords;


using G = AdjacencyList<UndirectedGraphTag>;
G g;
using V = GraphTraits<G>::VertexDescriptor;
using E = typename GraphTraits<G>::EdgeDescriptor;
int n = 5;
auto indexMap = vertexPropertyMap(g, vertexIndexTag);
int testDFS()
{
    depthFirstSearch(g, (
            treeEdge = [&](E e, V u, V to)
            {

                cout << "Tree Edge!!: " << u << " " << to << endl;
            },
            notTreeEdge = [&](E e, V u, V to)
            {

                cout <<"not Tree Edge: "  << u << " " << to << endl;
            },
            treeEdgeReturn = [&](E e, V u, V to)
            {
                cout <<"Tree Ruturn Edge: " << u << " " << to << endl;
            }
            ,enterVertex = 0
            ,marker = IndexMarker<decltype(indexMap)>(indexMap, n)

            ));

    return 1;
}

int testAdjacencyDFS()
{
    adjacencyDFS(g, (
            treeEdge = [&](V u, V to)
            {

                cout << "Tree Edge!!: " << u << " " << to << endl;
            },
            notTreeEdge = [&]( V u, V to)
            {

                cout <<"not Tree Edge: "  << u << " " << to << endl;
            },
            treeEdgeReturn = [&](V u, V to)
            {
                cout <<"Tree Ruturn Edge: " << u << " " << to << endl;
            }
            ,enterVertex = 0
            ,marker = IndexMarker<decltype(indexMap)>(indexMap, n)

            ));

    return 1;
}

G tree;
int testTreeDFS()
{

    for(auto e: outEdges(tree, 0))
    {
        cout << source(tree, e) << " " << target(tree, e) << endl;
    }


    for(auto e: outEdges(tree, 1))
    {
        cout << source(tree, e) << " " << target(tree, e) << endl;
    }

    cout << string(100, '-') << endl;
    cout << string(100, '-') << endl;

    treeDFS(tree, (
            treeEdge = [&](E e, V u, V to)
            {

            },
            notTreeEdge = [&](E e,  V u, V to)
            {

            },
            treeEdgeReturn = [&](E e, V u, V to)
            {

            }
            ,enterVertex = 0
            ));

    return 1;
}

int testAdjacencyTreeDFS()
{
    std::vector<pair<int, int>> treeEdges;
    adjacencyTreeDFS(tree, (
            treeEdge = [&](V u, V to)
            {
                treeEdges.push_back(make_pair(u, to));
            },
            notTreeEdge = [&](V u, V to)
            {

            },
            treeEdgeReturn = [&](V u, V to)
            {

            }
            ,enterVertex = 0
            ));


    assert(treeEdges.size() == 4);
    assert(treeEdges[0] == make_pair(0, 2));
    return 0;
}




int main()
{
    for(int i = 0; i < n; ++ i) addVertex(g);
    addEdge(g, 0, 2);
    addEdge(g, 2, 3);
    addEdge(g, 0, 1);
    addEdge(g, 1, 4);
    addEdge(g, 0, 4);
    addEdge(g, 1, 3);


    for(int i = 0; i < n; ++ i) addVertex(tree);
    addEdge(tree, 0, 1);
    addEdge(tree, 0, 2);
    addEdge(tree, 1, 3);
    addEdge(tree, 1, 4);








    testDFS();
    testAdjacencyDFS();

    testTreeDFS();
    testAdjacencyTreeDFS();


















	return 0;
}















