#ifndef GRAPH_BASE_H
#define GRAPH_BASE_H



/*
	this header file define some base classes and function
*/

struct NoProperty{};

/*
	for an edge, from vertex called begin, to vertex called end
*/ 
template<typename _EdgeProperty>
struct AdjacencyEdge
{
	int end;
	_EdgeProperty property;
	AdjacencyEdge(){}
	AdjacencyEdge(const int& _end, const _EdgeProperty& ep):
				end(_end), property(ep){}
	AdjacencyEdge(int&& _end, _EdgeProperty&& ep):
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
