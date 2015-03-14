#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H


#include <vector>
#include <tuple>

#include "graph_utility.h"

namespace lz {


using std::vector;
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
template<typename VP>
struct _ALVertex
{
    int head;
    VP vp;
    _ALVertex(const int& _head, const VP &_vp):
        head(_head), vp(_vp){}
    _ALVertex(int && _head, VP &&_vp):
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
template<typename EP>
struct _ALEdge
{
    AdjacencyEdge<EP> ae;
    int next;
};


template<typename VP = NoProperty, 
         typename EP = NoProperty, 
         typename GP = NoProperty >
class AdjacencyList
{
public:
    typedef VP VertexProperty;
    typedef EP EdgeProperty;
    typedef GP GraphProperty;

    explicit AdjacencyList(int n = 0, const GP _gp = GP()):
        v(n, _ALVertex<VP>(-1, VP())), gp(_gp) {}
    void clear(int n)
    {
        v.assign(n, _ALVertex<VP>(-1, VP()));
        e.clear();
    }
    VP& vertexProperty(int i) { return v[i].vp; }
    GP& graphProperty() { return gp; }; 

    void add(int a, int b, const EP & ep = EP())
    {
        _ALEdge<EP> ale;
        ale.ae = AdjacencyEdge<EP>(b, ep);
        ale.next = v[a].head;
        e.push_back(std::move(ale));
        v[a].head = int(e.size()) - 1;
    }
    void add(int a, int b, EP && ep)
    {
        _ALEdge<EP> ale;
        ale.ae = AdjacencyEdge<EP>(b, std::move(ep));
        ale.next = v[a].head;
        e.push_back(std::move(ale));
        v[a].head = int(e.size()) - 1;
    }
    void addBidirection(int a, int b, const EP & ep = EP()) 
    {
        add(a, b, ep);
        add(b, a, ep);
    }
    inline int vertexNumber() const { return v.size(); }
    inline int edgeNumber() const { return e.size(); }

    // adjacency edge iterator
    class Iterator
    {
        friend class AdjacencyList<VP, EP, GP>;
        vector<_ALEdge<EP> > *e;
        int eid;
        Iterator(vector<_ALEdge<EP> > *_e, int _eid):
            e(_e), eid(_eid){}
    public:
        Iterator():e(0), eid(-1){};
        Iterator& operator++()
        {
            eid = (*e)[eid].next;
            return *this;
        }
        Iterator operator++(int)
        {
            Iterator t(e, eid);
            eid = (*e)[eid].next;
            return t;
        }
        bool operator==(const Iterator &o) const { return e == o.e && eid == o.eid; }
        bool operator!=(const Iterator &o) const { return !(*this == o); }
        AdjacencyEdge<EP>& operator*() { return (*e)[eid].ae;  }
        AdjacencyEdge<EP>* operator->() { return &((*e)[eid].ae); }
    };
    Iterator begin(int i) { return Iterator(&e, v[i].head); }
    Iterator end(int i) { return Iterator(&e, -1); }
private:
    vector<_ALVertex<VP> > v;
    vector<_ALEdge<EP> > e;
    GP gp;
};









} // namespace lz


#endif //ADJACENCY_LIST_H