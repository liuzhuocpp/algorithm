#ifndef DIJKSTRA_SHORTEST_PATHS
#define DIJKSTRA_SHORTEST_PATHS

#include <vector>
#include <algorithm>

#include "adjacency_list.h"
#include "heap.h"
using std::vector;
// AdjacencyList<> g;
template<typename Value, typename Less = std::less<Value>, typename Plus = std::plus<Value> >
class DijkstraShortestPaths
{
	vector<int> p;
	vector<Value> d;
	int s;
	struct More
	{
		bool operator()(const Value &a, const Value &b)
		{
			return Less()(b, a);
		}
	};
public:
	DijkstraShortestPaths(){}
	DijkstraShortestPaths(const AdjacencyList<NoProperty, Value> & g, 
						int source, const Value &zero_value, const Value &inf_value) 
		{ calculate(g, source, zero_value, inf_value); }

	void calculate(const AdjacencyList<NoProperty, Value> & g, 
					int source, const Value &zero_value, const Value &inf_value) 
	{
		s = source;
		int n = g.vertexNumber();
		p.assign(n, -1);				
		d.assign(n, inf_value);
		d[source] = zero_value;
		Heap<Value, More> h(d);
		for(int t = 1; t < n; ++ t)
		{
			int u = h.heapIdToInputId(0);
			if(!Less()(d[u], inf_value)) return ;
			h.pop();
			for(int i = g.begin(u); ~i; i = g.next(i))
			{
				int to = g.to(i);
				Value tmp = Plus()(d[u], g.edgeProperty(i));
				if(h.inputIdToHeapId(to) != -1 && Less()(tmp, d[to]))
				{
					p[to] = u;
					d[to] = tmp;
					h.modify(to, tmp);
				}
			}
		}
	}
	inline int parent(int u) const { return p[u]; }
	inline Value distance(int u) const { return d[u]; }
	inline int source() const { return s; }
	vector<int> queryPath(int u)
	{
		vector<int> r;
		for(;u != -1; u = p[u]) r.push_back(u);
		std::reverse(r.begin(), r.end());
		return std::move(r);
	}

	// vector<Value>& distance() const{ return d; }
private:
	



};




#endif // DIJKSTRA_SHORTEST_PATHS