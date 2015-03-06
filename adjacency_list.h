#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H


#include <vector>
#include <tuple>

namespace lz 
{

using std::tuple;
using std::vector;
using std::get;
using std::cout;
using std::endl;

struct NoProperty
{

};

template<typename VertexProperty>
struct AdjacencyListVertex
{
    tuple<int, VertexProperty> i;
    AdjacencyListVertex():i(-1, VertexProperty()){};
    AdjacencyListVertex(int h, const VertexProperty &vp): i(h, vp){}

};
template<>
struct AdjacencyListVertex<NoProperty>
{
    tuple<int> i;
    AdjacencyListVertex():i(-1){};
    AdjacencyListVertex(int h):i(h){}
};

template<typename EdgeProperty>
struct AdjacencyListEdge
{
    tuple<int, int, EdgeProperty> i; //0 for to, 1 for next, 2 for EdgeProperty
    AdjacencyListEdge() = default;
    AdjacencyListEdge(int a, int b, const EdgeProperty &ep):i(a, b, ep){}
};
template<>
struct AdjacencyListEdge<NoProperty>
{
    tuple<int, int> i; //0 for to, 1 for next, 2 for EdgeProperty
    AdjacencyListEdge() = default;
    AdjacencyListEdge(int a, int b, const NoProperty &ep):i(a, b){}
};




template<typename VertexProperty = NoProperty, typename EdgeProperty = NoProperty>
class AdjacencyList
{
public:
    explicit AdjacencyList(int n = 0):
        v(n, AdjacencyListVertex<VertexProperty>() )  { }
    void clear(int n) 
    {
        v.assign(n, AdjacencyListVertex<VertexProperty>());
    }

    inline int begin(int vid) const { return get<0>(v[vid].i); }
    inline VertexProperty vertexProperty(int vid) const { return get<1>(v[vid].i); }
    inline void setVertexProperty(int vid, const VertexProperty &vp) const 
        { get<1>(v[vid].i) = vp; }

    inline int to(int eid) const { return get<0>(e[eid].i); }
    inline int next(int eid) const { return get<1>(e[eid].i); }
    inline EdgeProperty edgeProperty(int eid) const { return get<2>(e[eid].i); }
    inline void setEdgeProperty(int eid, const EdgeProperty &ep) const 
        { get<2>(e[eid].i) = ep; }

    inline int vertexNumber() const { return v.size(); }
    inline int edgeNumber() const { return e.size(); }
    
    void addEdge(int a, int b, const EdgeProperty &ep = EdgeProperty() )
    {
        e.push_back(AdjacencyListEdge<EdgeProperty>(b, get<0>(v[a].i), ep));
        get<0>(v[a].i) = int(e.size()) - 1;
    }
    void addBidirectionalEdge(int a, int b, const EdgeProperty &ep = EdgeProperty())
    {
        addEdge(a, b, ep);
        addEdge(b, a, ep);
    }
    void outputGraph()
    {
        int n = vertexNumber();
        for(int i = 0; i < n; ++ i)
        {
            cout << "cnt: " << i<< endl;
            for(int j = begin(i); ~j; j = next(j))
            {
                cout << to(j) << " ";
            }
            cout << endl;
        }
    }

private:
    vector<AdjacencyListVertex<VertexProperty> > v;
    vector<AdjacencyListEdge<EdgeProperty> > e;
};


} // namespace lz


#endif //ADJACENCY_LIST_H