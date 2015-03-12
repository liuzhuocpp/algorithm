#ifndef GRAPH_H
#define GRAPH_H

/*
	this header just descripte the Template graph concept,
	this not for use
*/

#include "graph_utility.h"

namespace lz {


template<typename _VertexProperty = NoProperty, 
		 typename _EdgeProperty = NoProperty, 
		 typename _GraphProperty= NoProperty >
class Graph
{
public:
	typedef _VertexProperty VertexProperty;
	typedef _EdgeProperty EdgeProperty;
	typedef _GraphProperty GraphProperty;	

	explicit Graph(int n = 0, GraphProperty _gp);
	Graph(const Graph&);
	Graph(Graph&&);
	Graph& operator=(const Graph&);
	Graph& operator=(Graph&&);
	~Graph();

	void clear(int n);
	VertexProperty& vertexProperty(int v);
	GraphProperty& graphProperty();	

	void add(int a, int b, const EdgeProperty & ep);
	void add(int a, int b, EdgeProperty && ep);
	void addBidirection(int a, int b, const EdgeProperty & ep);
	inline int vertexNumber() const;
	inline int edgeNumber() const ;	

	// AdjacencyEdge
	class AdjacencyEdgeIterator
	{
	public:
		AdjacencyEdgeIterator();
		AdjacencyEdgeIterator operator++();
		AdjacencyEdgeIterator operator++(int);
		bool operator==(const AdjacencyEdgeIterator &o);
		bool operator!=(const AdjacencyEdgeIterator &o);
		AdjacencyEdge<EdgeProperty>& operator*();
		AdjacencyEdge<EdgeProperty>* operator->();
	};
	AdjacencyEdgeIterator beginAdjacencyEdge(int v) const;
	AdjacencyEdgeIterator endAdjacencyEdge(int v) const;
};


}
#endif // GRAPH_H
