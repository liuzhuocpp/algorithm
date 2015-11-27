/*
 * main.cpp
 *
 *  Created on: 2015Äê11ÔÂ26ÈÕ
 *      Author: LZ
 */

#include <iostream>
#include <cstdio>
#include <cstring>
#include <tuple>
#include <vector>

#include <lz/adjacency_matrix.hpp>

using namespace std;
using namespace lz;

int main()
{
	using G = AdjacencyMatrix<>;
	G g;

	g.assignVertex(6);
	g.addEdge(0, 1);

	g.addEdge(3, 1);
	auto ed = g.addEdge(2, 5);

	g.removeEdge(ed);

	auto i = g.edge(2, 5);


	cout << i.second << endl;
//	cout << i.first.target << endl;







    return 0;
}
