#include <iostream>
#include <tuple>

#include "adjacency_list.h"

using namespace std;
using namespace lz;


int main()
{
	typedef AdjacencyList<> G;
	G g(10);
	g.addEdge(0, 1);
	g.addEdge(0, 3);
	g.addEdge(0, 1);
	g.addEdge(0, 8);
	G::OutEdgeIterator o, o_end;

	g.outEdges(0);

	for(tie(o, o_end) = g.outEdges(0); o != o_end; ++ o)
	{
		cout << g.source(*o) <<  " " << g.target(*o) << endl;
	}


	return 0;
}