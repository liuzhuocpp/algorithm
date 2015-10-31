#include <iostream>
#include <cstdio>
#include <cstring>
#include <tuple>
#include <vector>


#include "lz/adjacency_list.h"
#include "lz/map.h"

using namespace std;
using namespace lz;

struct VP
{
	int a;
	double b;
	int c;
};

int main()
{
	AdjacencyList<Directed, ClassMap<VP> > g;
	int n = 5;
	for(int i = 0; i < n; ++ i)
	{
		VP vp = VP{i, 1.222 + i, 33 + i};

		g.addVertex(makeClassMap(vp));

	}


	auto mpA = g.getVertexPropertyMap(&VP::a);
	for(int i = 0; i < n; ++ i)
	{
		cout << mpA[i] << endl;
	}

	auto mpB = g.getVertexPropertyMap(&VP::b);
	for(int i = 0; i < n; ++ i)
	{
		cout << mpB[i] << endl;
	}

	auto mpC = g.getVertexPropertyMap(&VP::c);
	for(int i = 0; i < n; ++ i)
	{
		cout << mpC[i] << endl;
	}



    return 0;
}
