#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H


#include <vector>
#include <tuple>

#include "graph_utility.h"

namespace lz {

using std::tuple;
using std::vector;
using std::get;
using std::cout;
using std::endl;

// struct NoProperty
// {

// };

// template<typename VertexProperty>
// struct AdjacencyListVertex
// {
//     tuple<int, VertexProperty> i;
//     AdjacencyListVertex():i(-1, VertexProperty()){};
//     AdjacencyListVertex(int h, const VertexProperty &vp): i(h, vp){}

// };
// template<>
// struct AdjacencyListVertex<NoProperty>
// {
//     tuple<int> i;
//     AdjacencyListVertex():i(-1){};
//     AdjacencyListVertex(int h):i(h){}
// };

// template<typename EdgeProperty>
// struct AdjacencyListEdge
// {
//     tuple<int, int, EdgeProperty> i; //0 for to, 1 for next, 2 for EdgeProperty
//     AdjacencyListEdge() = default;
//     AdjacencyListEdge(int a, int b, const EdgeProperty &ep):i(a, b, ep){}
// };
// template<>
// struct AdjacencyListEdge<NoProperty>
// {
//     tuple<int, int> i; //0 for to, 1 for next, 2 for EdgeProperty
//     AdjacencyListEdge() = default;
//     AdjacencyListEdge(int a, int b, const NoProperty &ep):i(a, b){}
// };




// template<typename VertexProperty = NoProperty, typename EdgeProperty = NoProperty>
// class AdjacencyList
// {
// public:
//     explicit AdjacencyList(int n = 0):
//         v(n, AdjacencyListVertex<VertexProperty>() )  { }
//     void clear(int n) 
//     {
//         v.assign(n, AdjacencyListVertex<VertexProperty>());
//     }

//     inline int begin(int vid) const { return get<0>(v[vid].i); }
//     inline VertexProperty vertexProperty(int vid) const { return get<1>(v[vid].i); }
//     inline void setVertexProperty(int vid, const VertexProperty &vp) const 
//         { get<1>(v[vid].i) = vp; }

//     inline int to(int eid) const { return get<0>(e[eid].i); }
//     inline int next(int eid) const { return get<1>(e[eid].i); }
//     inline EdgeProperty edgeProperty(int eid) const { return get<2>(e[eid].i); }
//     inline void setEdgeProperty(int eid, const EdgeProperty &ep) const 
//         { get<2>(e[eid].i) = ep; }

//     inline int vertexNumber() const { return v.size(); }
//     inline int edgeNumber() const { return e.size(); }
    
//     void addEdge(int a, int b, const EdgeProperty &ep = EdgeProperty() )
//     {
//         e.push_back(AdjacencyListEdge<EdgeProperty>(b, get<0>(v[a].i), ep));
//         get<0>(v[a].i) = int(e.size()) - 1;
//     }
//     void addBidirectionalEdge(int a, int b, const EdgeProperty &ep = EdgeProperty())
//     {
//         addEdge(a, b, ep);
//         addEdge(b, a, ep);
//     }
//     void outputGraph()
//     {
//         int n = vertexNumber();
//         for(int i = 0; i < n; ++ i)
//         {
//             cout << "cnt: " << i<< endl;
//             for(int j = begin(i); ~j; j = next(j))
//             {
//                 cout << to(j) << " ";
//             }
//             cout << endl;
//         }
//     }

// private:
//     vector<AdjacencyListVertex<VertexProperty> > v;
//     vector<AdjacencyListEdge<EdgeProperty> > e;
// };



// define vertex info in AdjacencyList
template<typename VertexProperty>
struct _ALVertex
{
    int head;
    VertexProperty vp;
    _ALVertex(const int& _head, const VertexProperty &_vp):
        head(_head), vp(_vp){}
    _ALVertex(int && _head, VertexProperty &&_vp):
        head(_head), vp(_vp){}
};
template<>
struct _ALVertex<NoProperty>
{
    int head;
    _ALVertex(const int& _head, const NoProperty &_vp):
        head(_head){}
    _ALVertex(int && _head, NoProperty &&_vp):
        head(_head){}

};

// define edge info in AdjacencyList
template<typename EdgeProperty>
struct _ALEdge
{
    AdjacencyEdge<EdgeProperty> ae;
    int next;
};




template<typename _VertexProperty = NoProperty, 
         typename _EdgeProperty = NoProperty, 
         typename _GraphProperty= NoProperty >
class AdjacencyList
{
public:
    typedef _VertexProperty VertexProperty;
    typedef _EdgeProperty EdgeProperty;
    typedef _GraphProperty GraphProperty;   

    explicit AdjacencyList(int n = 0, GraphProperty _gp = GraphProperty()):
        v(n, _ALVertex<VertexProperty>(-1, VertexProperty())), gp(_gp) {}
    // AdjacencyList(const Graph&);
    // AdjacencyList(Graph&&);
    // AdjacencyList& operator=(const Graph&);
    // AdjacencyList& operator=(Graph&&);
    // ~AdjacencyList();

    void clear(int n)
    {
        v.assign(n, _ALVertex<VertexProperty>(-1, VertexProperty()));
        e.clear();
    }
    VertexProperty& vertexProperty(int i) { return v[i].vp; }
    GraphProperty& graphProperty() { return gp; }; 

    void add(int a, int b, const EdgeProperty & ep)
    {
        _ALEdge<EdgeProperty> ale;
        ale.ae = AdjacencyEdge<EdgeProperty>(v[a].head, ep);
        ale.next = int(e.size()) - 1;
        e.push_back(std::move(ale));
    }
    void add(int a, int b, EdgeProperty && ep)
    {
        _ALEdge<EdgeProperty> ale;
        ale.ae = AdjacencyEdge<EdgeProperty>(std::move(v[a].head), ep);
        ale.next = int(e.size()) - 1;
        e.push_back(std::move(ale));
    }
    void addBidirection(int a, int b, const EdgeProperty & ep) 
    {
        add(a, b, ep);
        add(b, a, ep);
    }
    inline int vertexNumber() const { return v.size(); }
    inline int edgeNumber() const { return e.size(); }

    // AdjacencyEdge
    class AdjacencyEdgeIterator
    {
        vector<_ALEdge<EdgeProperty> > *e;
        int eid;
        AdjacencyEdgeIterator(vector<_ALEdge<EdgeProperty> > *_e, int _eid):
            e(_e), eid(_eid){}
    public:
        AdjacencyEdgeIterator():e(0), eid(-1){};
        AdjacencyEdgeIterator& operator++()
        {
            eid = (*e)[eid].next;
            return *this;
        }
        AdjacencyEdgeIterator operator++(int)
        {
            AdjacencyEdgeIterator t(e, eid);
            eid = (*e)[eid].next;
            return t;
        }
        bool operator==(const AdjacencyEdgeIterator &o) const { return e == o.e && eid == o.eid; }
        bool operator!=(const AdjacencyEdgeIterator &o) const { return !(*this == o); }
        AdjacencyEdge<EdgeProperty>& operator*() { return (*e)[eid].ae;  }
        AdjacencyEdge<EdgeProperty>* operator->() { return &((*e)[eid].ae); }
    };
    AdjacencyEdgeIterator beginAdjacencyEdge(int i) const 
        { return AdjacencyEdgeIterator(&e, v[i].head); }
    AdjacencyEdgeIterator endAdjacencyEdge(int i) const
        { return AdjacencyEdgeIterator(&e, -1); }



private:
    vector<_ALVertex<VertexProperty> > v;
    vector<_ALEdge<EdgeProperty> > e;
    GraphProperty gp;
};









} // namespace lz


#endif //ADJACENCY_LIST_H