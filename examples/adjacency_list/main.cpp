#include <iostream>
#include <cstdio>
#include <cstring>
#include <tuple>
#include <vector>


#include "lz/adjacency_list.h"
#include "lz/map.h"
#include "lz/property.h"

using namespace std;
using namespace lz;

struct VP
{
	int a;
	double b;
	int c;
};
struct int_tag{};
struct char_tag{};
int main()
{
	using VP = Property<int_tag, int,
			   Property<char_tag, char> >;


	using G = AdjacencyList<Directed, VP >;
	G g;
	int n = 5;
	for(int i = 0; i < n; ++ i)
	{
		VP vp = VP(22 + i, 33 + i);
		cout << "YEYYE" << get(vp, int_tag()) << endl;
		g.addVertex(vp);
	}
	for(int i = 0; i < n; ++ i)
	{
		cout << "IIIIII" << get(g.v[i].vp, int_tag()) << endl;
	}
	G::VertexPropertyMap<int_tag> test = g.vertexPropertyMap(int_tag());
	for(int i = 0; i < n; ++ i)
	{

		cout << test[i] << endl;

	}





    return 0;
}
