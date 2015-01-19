#ifndef GRAPH_H
#define GRAPH_H


#include <vector>

struct NoProperty{};
template<typename outEdgeProperty = NoProperty>
class AdjacencyList
{
public:	
	AdjacencyList(int n = 0):n(n) 
	{

	}	
	void clear()
	{
		
	}
	~AdjacencyList() {}
	void addEdge(int a, int b, int t = NoType()){}
private:




	
};









#endif //GRAPH_H