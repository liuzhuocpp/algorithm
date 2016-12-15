#include <iostream>
#include <cstdio>
#include <cstring>
#include <tuple>
#include <vector>

#include "lz/adjacency_list.h"
#include "lz/map.h"
#include "lz/property.h"

// property graph test

using namespace std;
using namespace lz;

struct int_tag
{
};
struct char_tag
{
};
struct double_tag
{
};
int main()
{
    using VP = Property<int_tag, int,
    Property<char_tag, char,
    Property<double_tag, double >> >;

    using EP = Property<int_tag, int,
    Property<char_tag, char> >;

    using G = AdjacencyList<UndirectedGraphTag, VP, EP>;
    G g;

    int n = 2;
    g.addVertex(VP(make_tuple(10, 'z', 333.3)));
    g.addVertex(VP(make_tuple(99, '^', 11.11)));

    g.addEdge(0, 1, EP(5555, '!'));

    G ng = g;

    auto vi_mp = ng.vertexPropertyMap(char_tag());
    auto mp = ng.vertexPropertyMap(int_tag());
    auto edge_mp = ng.edgePropertyMap(char_tag());

    vi_mp[0] = '*';
    cout << "imap: " << vi_mp[0] << endl;
    cout << "imap: " << vi_mp[1] << endl;

//	cout << mp[0] << endl;

//	cout << edge_mp[0] << endl;
//
//	G::VertexIterator vi, vi_end;
//
//	for(tie(vi, vi_end) = ng.vertices(); vi != vi_end; ++ vi)
//	{
////		mp[*vi] = 4444444;
//		cout << mp[*vi] << endl;
//	}

    return 0;
}
