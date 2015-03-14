#ifndef GRAPH_BASE_H
#define GRAPH_BASE_H



/*
	this header file define some base classes and functions
*/

struct NoProperty{};

/*
	for an adjacency edge, from vertex called begin, to vertex called end
*/ 
template<typename EP>
struct AdjacencyEdge
{
	int end;
	EP property;
	AdjacencyEdge(){}
	AdjacencyEdge(const int& _end, const EP& ep):
				end(_end), property(ep){}
	AdjacencyEdge(int&& _end, EP&& ep):
				end(_end), property(ep){}
};
template<>
struct AdjacencyEdge<NoProperty>
{
	int end;
	AdjacencyEdge(){}
	AdjacencyEdge(const int& _end, const NoProperty& ep):
				end(_end){}
	AdjacencyEdge(int&& _end, NoProperty&& ep):
				end(_end){}
};



#endif // GRAPH_BASE_H
