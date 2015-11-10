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

struct int_tag{};
struct char_tag{};
int main()
{
	using VP = Property<int_tag, int,
			   Property<char_tag, char> >;


	using G = AdjacencyList<DirectedGraphTag, VP>;
	G g;
	int n = 2;
	g.addVertex(VP(10, 'z'));
	g.addVertex(VP(2222, '$'));

	const G ng = g;
	auto mp = ng.vertexPropertyMap(int_tag());

	G::VertexIterator vi, vi_end;

	for(tie(vi, vi_end) = ng.vertices(); vi != vi_end; ++ vi)
	{
//		mp[*vi] = 4444444;
		cout << mp[*vi] << endl;
	}





    return 0;
}
