#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H


#include <vector>
#include <algorithm>
using std::vector;
using std::endl;
using std::cout;
using std::pair;
using std::make_pair;
using std::ostream;
class AdjacencyList
{
public:
    AdjacencyList(int n = 0):head(n, -1) {}
    void clear(int n) { head.assign(n, -1); }
    inline int begin(int vid) const { return head[vid]; }
    inline int to(int eid) const { return e[eid].first; }
    inline int next(int eid) const { return e[eid].second; }
    inline int vertexNumber() const { return head.size(); }
    inline int edgeNumber() const { return e.size(); }
    void addEdge(int a, int b)
    {
        e.push_back(make_pair(b, head[a]));
        head[a] = int(e.size()) - 1;
    }
    void addBidirectionalEdge(int a, int b)
    {
        addEdge(a, b);
        addEdge(b, a);
    }
    friend ostream& operator<<(ostream &out, const AdjacencyList & g)
    {
        out << "AdjacencyList:" << endl;
        for(int i = 0; i < g.vertexNumber(); ++ i)
        {
            out << "i=" << i << ": ";
            for(int j = g.begin(i); ~j; j = g.next(j))
            {
                out << g.to(j) << " ,";
            }
            out << endl;
        }
    }
private:
    vector<int> head;
    vector<pair<int, int> > e;
};
#endif //ADJACENCY_LIST_H