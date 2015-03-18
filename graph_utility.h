#ifndef GRAPH_BASE_H
#define GRAPH_BASE_H



/*
	this header file define some base classes and functions
*/

struct NoProperty{};

/*
	for an edge, a vertex called begin, other vertex called end
*/ 

		
template<typename EP>
struct AdjacencyEdge
{
	int end;
	EP property;
	AdjacencyEdge() = default;
	AdjacencyEdge(const int &_end, const EP& ep): end(_end), property(ep){}
	AdjacencyEdge(const int &_end, EP&& ep): end(_end), property(ep){}
};
template<>
struct AdjacencyEdge<NoProperty>
{
	int end;
	AdjacencyEdge() = default;
	AdjacencyEdge(const int& _end, const NoProperty& ep): end(_end){}
	AdjacencyEdge(const int& _end, NoProperty&& ep): end(_end){}
};


template<typename EP>
struct Edge
{
	int begin, end;
	EP property;
	Edge() = default;
	Edge(const int _begin, const int _end, const EP & ep):
		begin(_begin), end(_end), property(ep){}
	Edge(const int _begin, const int _end, EP && ep): 
		begin(_begin), end(_end), property(ep){}
};
template<>
struct Edge<NoProperty>
{
	int begin, end;
	Edge() = default;
	Edge(const int _begin, const int _end, const NoProperty & ep):
		begin(_begin), end(_end){}
	Edge(const int _begin, const int _end, NoProperty && ep):
		begin(_begin), end(_end){}
};
/*
	a graph concept, of course, it will go on concept check, because I don`t know how to do
*/

/*
template<typename VP = NoProperty, 
		 typename EP = NoProperty, 
		 typename GP = NoProperty >
class Graph
{
public:
	typedef VP VertexProperty;
	typedef EP EdgeProperty;
	typedef GP GraphProperty;

	explicit Graph(int n = 0, const GP _gp = GP());
	void clear(int n);
	VP& vertexProperty(int v);
	GP& graphProperty();
	void add(int a, int b, const EP & ep = EP());
	void add(int a, int b, EP && ep);
	void addBidirection(int a, int b, const EP & ep);
	inline int vertexNumber() const;	

	// adjacency edge iterator
	class Iterator
	{
	public:
		Iterator();
		Iterator& operator++();
		Iterator operator++(int);
		bool operator==(const Iterator &o) const;
		bool operator!=(const Iterator &o) const;
		AdjacencyEdge<EP>& operator*();
		AdjacencyEdge<EP>* operator->();
	};
	Iterator begin(int v);
	Iterator end(int v);

	// adjacency edge const iterator
	class ConstIterator
	{
	public:
		ConstIterator();
		ConstIterator& operator++();
		ConstIterator operator++(int);
		bool operator==(const ConstIterator &o)const ;
		bool operator!=(const ConstIterator &o)const;
		const AdjacencyEdge<EP>& operator*() const;
		const AdjacencyEdge<EP>* operator->() const;
	};
	ConstIterator begin(int v);
	ConstIterator end(int v);
};

*/
#endif // GRAPH_BASE_H
