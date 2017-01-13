/*
 * main.cpp
 *
 *  Created on: 2015��11��26��
 *      Author: LZ
 */

#include <lz/adjacency_matrix.h>
#include <lz/std_utility.h>

#include <bits/stdc++.h>

using namespace std;
using namespace lz;


void testUndirected()
{
    cout << __FUNCTION__ << " " << string(100, '-') <<  endl;

    using G = AdjacencyMatrix<UndirectedGraphTag>;
    using EdgeDescriptor = G::EdgeDescriptor;
    G g;
    int n = 10;
    for(int i = 0; i < n; ++ i)
        addVertex(g);

    for(auto u : vertices(g))
    {
        cout << "u " << u << endl;
    }

    addEdge(g, 0, 1);
    addEdge(g, 3, 1);
    addEdge(g, 2, 4);
    addEdge(g, 1, 2);


    cout << edge(g, 0, 1) << endl;
    cout << edge(g, 1, 0) << endl;
    cout << edge(g, 3, 1) << endl;
    cout << edge(g, 1, 2) << endl;
    cout << edge(g, 1, 9) << endl;

    cout << verticesNumber(g) << endl;



    assert(edge(g, 0, 1) == EdgeDescriptor(1, 0));
    assert(edge(g, 1, 0) == EdgeDescriptor(1, 0));

}

void testVPAndEP()
{
    cout << __FUNCTION__ << " " << string(100, '-') <<  endl;
    struct int_tag{};
    struct char_tag{};
    using P = Property<int_tag, int,
              Property<char_tag, char>>;

    using G = AdjacencyMatrix<DirectedGraphTag, P, P>;
    G g;
    int n = 10;
    for(int i = 0; i < n; ++ i)
        addVertex(g);

    for(auto u : vertices(g))
    {
        cout << "u " << u << endl;
    }

    addEdge(g, 0, 1);
    addEdge(g, 3, 1);
    addEdge(g, 2, 4);
    addEdge(g, 1, 2);


    cout << edge(g, 0, 1) << endl;
    cout << edge(g, 1, 0) << endl;
    cout << edge(g, 1, 3) << endl;
    cout << edge(g, 1, 2) << endl;
    cout << verticesNumber(g) << endl;

    auto vertexIntMap = vertexPropertyMap(g, int_tag());

    vertexIntMap[0] = 233;
    vertexIntMap[3] = 6666;

    cout << vertexIntMap[0] << endl;
    cout << vertexIntMap[3] << endl;
    cout << vertexIntMap[4] << endl;

    assert(vertexIntMap[0] == 233);
    assert(vertexIntMap[3] == 6666);




}

int main()
{

    testUndirected();
    testVPAndEP();


    return 0;
}
