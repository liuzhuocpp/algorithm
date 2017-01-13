#include <bits/stdc++.h>
#include <lz/floyd_shortest_paths.h>
#include <lz/adjacency_list.h>
#include <lz/iterator_range.h>

using namespace lz;
using namespace std;

template<typename IndexType, typename ValueType>
struct MatrixMap: public MapFacade<std::pair<IndexType, IndexType>, ValueType>
{
    MatrixMap(vector<vector<int>>& data):data(data)
    {

    }
    vector<vector<int>> &data;

    ValueType& operator[](const pair<IndexType, IndexType>& i)
    {
        return data[i.first][i.second];
    }
};
void test()
{
    cout << __FUNCTION__ << " " << string(100, '-') << endl;
//    struct int_tag{};
    using EP = Property<EdgeWeightTag, int>;

    using G = AdjacencyList<DirectedGraphTag, NoProperty, EP>;
    G g;
    int n = 5;
    for(auto i: irange(n))
    {
        addVertex(g);
    }

    addEdge(g, 0, 1, 2);
    addEdge(g, 2, 0, 3);
    addEdge(g, 3, 1, 20);
    addEdge(g, 0, 1, 300);
    addEdge(g, 1, 2, 300);
    addEdge(g, 0, 2, 1000);

    vector<vector<int>> data(n, vector<int>(n));
    MatrixMap<int, int> mp(data);

    using namespace FloydShortestPathKeywords;
    floydShortestPaths(g, (
        distanceMap = mp));

    for(int i = 0; i < n; ++ i)
    {
        for(int j = 0; j < n; ++ j)
        {
            printf("([%d][%d]=%010d), ", i, j, data[i][j]);
        }
        puts("");
    }

    assert(data[0][2] == 302);
}




void testInitialized()
{
    cout << __FUNCTION__ << " " << string(100, '-') << endl;

    using EP = Property<EdgeWeightTag, int>;
    using G = AdjacencyList<DirectedGraphTag, NoProperty, EP>;
    G g;
    int n = 5;
    for(auto i: irange(n))
    {
        addVertex(g);
    }

    addEdge(g, 0, 1, 2);
    addEdge(g, 2, 0, 3);
    addEdge(g, 3, 1, 20);
    addEdge(g, 0, 1, 300);
    addEdge(g, 1, 2, 300);
    addEdge(g, 0, 2, 1000);

    vector<vector<int>> data(n, vector<int>(n));
    for(int i = 0; i < n; ++ i)
    {
        for(int j = 0; j < n; ++ j)
        {
            if(i == j) data[i][j] = 0;
            else data[i][j] = std::numeric_limits<int>::max();
        }
    }
    auto _edges = edges(g);
    auto edgeWeightMap = edgePropertyMap(g, edgeWeightTag);
    for(auto e: _edges)
    {
        data[source(g, e)][target(g, e)] = std::min(data[source(g, e)][target(g, e)], edgeWeightMap[e]);
    }


    MatrixMap<int, int> mp(data);

    using namespace FloydShortestPathKeywords;
    floydInitializedShortestPaths(g, (
        distanceMap = mp));

    for(int i = 0; i < n; ++ i)
    {
        for(int j = 0; j < n; ++ j)
        {
            printf("([%d][%d]=%010d), ", i, j, data[i][j]);
        }
        puts("");
    }

    assert(data[0][2] == 302);
}




int main()
{
    test();
    testInitialized();

	return 0;
}
