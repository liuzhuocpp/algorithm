#ifndef DIJKSTRA_SHORTEST_PATHS
#define DIJKSTRA_SHORTEST_PATHS

#include <vector>
#include <algorithm>
#include <tuple>
// #include "adjacency_list.h"
#include "heap.h"

namespace lz {



using std::vector;
using std::get;
template<typename Graph, typename Dist, int EdgeKey,
		 typename Plus, typename Less = std::less<Dist>  >
class DijkstraShortestPaths
{
	vector<int> p;
	vector<Dist> d;
	int s;
	struct More
	{
		bool operator()(const Dist &a, const Dist &b)
		{
			return Less()(b, a);
		}
	};
public:
	DijkstraShortestPaths(){}
	DijkstraShortestPaths(const Graph & g, int source, const Dist &zero_dist, const Dist &inf_dist) 
		{ calculate(g, source, zero_dist, inf_dist); }

	void calculate(const Graph & g, int source, const Dist &zero_dist, const Dist &inf_dist) 
	{
		s = source;
		int n = g.vertexNumber();
		p.assign(n, -1);
		d.assign(n, inf_dist);
		d[source] = zero_dist;
		Heap<Dist, More> h(d);
		for(int t = 1; t < n; ++ t)
		{
			int u = h.heapIdToInputId(0);
			if(!Less()(d[u], inf_dist)) return ;
			h.pop();
			typename Graph::ConstIterator eit;
			for(eit = g.begin(u);eit != g.end(u); ++ eit)			
			{
				int to = eit->end;
				Dist tmp = Plus()(d[u], get<EdgeKey>(eit->property));
				if(h.inputIdToHeapId(to) != -1 && Less()(tmp, d[to]))
				{
					p[to] = u;
					d[to] = tmp;
					h.modify(to, tmp);
				}
			}
		}
	}
	inline int predecessor(int u) const { return p[u]; }
	inline Dist distance(int u) const { return d[u]; }
	inline int source() const { return s; }
	vector<int> getPath(int u)
	{
		vector<int> r;
		for(;u != -1; u = p[u]) r.push_back(u);
		std::reverse(r.begin(), r.end());
		return r;
	}

};



}

#endif // DIJKSTRA_SHORTEST_PATHS