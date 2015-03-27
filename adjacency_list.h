#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H


#include <vector>
#include <tuple>
#include <algorithm>
#include "graph_utility.h"

namespace lz {


using std::vector;
using std::cout;
using std::endl;
using std::pair;
using std::make_pair;


// // define vertex info in AdjacencyList
// template<typename VP>
// struct ALVertex
// {
//     int head;
//     VP vp;
//     ALVertex(const int& _head, const VP &_vp):
//         head(_head), vp(_vp){}
//     ALVertex(int && _head, VP &&_vp):
//         head(_head), vp(_vp){}
// };
// template<>
// struct ALVertex<NoProperty>
// {
//     int head;
//     ALVertex(const int& _head, const NoProperty &_vp):
//         head(_head){}
//     ALVertex(int && _head, NoProperty &&_vp):
//         head(_head){}
// };
// // define edge info in AdjacencyList
// template<typename EP>
// struct ALEdge
// {
//     AdjacencyEdge<EP> ae;
//     int next;
// };


// template<typename VP = NoProperty, 
//          typename EP = NoProperty, 
//          typename GP = NoProperty >
// class AdjacencyList
// {
// public:
//     typedef VP VertexProperty;
//     typedef EP EdgeProperty;
//     typedef GP GraphProperty;

//     explicit AdjacencyList(int n = 0, const GP _gp = GP(), int expect_m = 0):
//         v(n, ALVertex<VP>(-1, VP())), gp(_gp) 
//         {
//             e.reserve(expect_m);
//         }
//     void clear(int n, int expect_m = 0)
//     {
//         v.assign(n, ALVertex<VP>(-1, VP()));
//         e.clear();
//         e.reserve(expect_m);
//     }
//     VP& vertexProperty(int i) { return v[i].vp; }
//     GP& graphProperty() { return gp; }

//     void add(int a, int b, const EP & ep = EP())
//     {
//         ALEdge<EP> ale;
//         ale.ae = AdjacencyEdge<EP>(b, ep);
//         ale.next = v[a].head;
//         e.push_back(std::move(ale));
//         v[a].head = int(e.size()) - 1;
//     }
//     void add(int a, int b, EP && ep)
//     {
//         ALEdge<EP> ale;
//         ale.ae = AdjacencyEdge<EP>(std::move(b), std::move(ep));
//         ale.next = v[a].head;
//         e.push_back(std::move(ale));
//         v[a].head = int(e.size()) - 1;
//     }
//     void addBidirection(int a, int b, const EP & ep = EP()) 
//     {
//         add(a, b, ep);
//         add(b, a, ep);
//     }
//     inline int vertexNumber() const { return v.size(); }
//     inline int edgeNumber() const { return e.size(); }

//     // adjacency edge iterator    
//     class Iterator
//     {
//         friend class AdjacencyList<VP, EP, GP>;
//         vector<ALEdge<EP> > *e;
//         int eid;
//         Iterator(vector<ALEdge<EP> > *_e, int _eid):
//             e(_e), eid(_eid){}
//     public:
//         Iterator():e(0), eid(-1){}
//         Iterator& operator++()
//         {
//             eid = (*e)[eid].next;
//             return *this;
//         }
//         Iterator operator++(int)
//         {
//             Iterator t(e, eid);
//             eid = (*e)[eid].next;
//             return t;
//         }
//         bool operator==(const Iterator &o) const { return e == o.e && eid == o.eid; }
//         bool operator!=(const Iterator &o) const { return !(*this == o); }
//         AdjacencyEdge<EP>& operator*() { return (*e)[eid].ae;  }
//         AdjacencyEdge<EP>* operator->() { return &((*e)[eid].ae); }
//     };
//     Iterator begin(int i) { return Iterator(&e, v[i].head); }
//     Iterator end(int i) { return Iterator(&e, -1); }


//     class ConstIterator
//     {
//         friend class AdjacencyList<VP, EP, GP>;
//         const vector<ALEdge<EP> > *e;
//         int eid;
//         ConstIterator(const vector<ALEdge<EP> > *_e, int _eid):
//             e(_e), eid(_eid){}
//     public:
//         ConstIterator():e(0), eid(-1){}
//         ConstIterator& operator++()
//         {
//             eid = (*e)[eid].next;
//             return *this;
//         }
//         ConstIterator operator++(int)
//         {
//             ConstIterator t(e, eid);
//             eid = (*e)[eid].next;
//             return t;
//         }
//         bool operator==(const ConstIterator &o) const { return e == o.e && eid == o.eid; }
//         bool operator!=(const ConstIterator &o) const { return !(*this == o); }
//         const AdjacencyEdge<EP>& operator*() const { return (*e)[eid].ae;  }
//         const AdjacencyEdge<EP>* operator->() const { return &((*e)[eid].ae); }
//     };
//     ConstIterator begin(int i) const { return ConstIterator(&e, v[i].head); }    
//     ConstIterator end(int i) const { return ConstIterator(&e, -1); }

//     typedef Iterator iterator;
//     typedef ConstIterator const_iterator;







//     AdjacencyEdge<EP>& edge(int eid) { return e[eid].ae; }
// private:
//     vector<ALVertex<VP> > v;
//     vector<ALEdge<EP> > e;
//     GP gp;
// };


    namespace AdjacencyListPrivate {

    template<typename VP>
    struct V
    {
        int head;
        VP vp;
        V() = default;
        V(int _h, const VP &_vp):head(_h), vp(_vp) {}
    };
    template<>
    struct V<NoProperty>
    {
        int head;
        V() = default;
        V(int _h, const NoProperty &_vp):head(_h) {}
    };

    template<typename EP>
    struct E
    {
        int to, next;
        EP ep;
        E() = default;
        E(int _to, int _next, const EP & _ep):to(_to), next(_next), ep(_ep){}
    };
    template<>
    struct E<NoProperty>
    {
        int to, next;
        E() = default;
        E(int _to, int _next, const NoProperty & _ep):to(_to), next(_next){}
    };


    template<typename VP, typename EP, typename GP>
    struct G
    {
        vector<V<VP> > v;
        vector<E<EP> > e;
        GP gp;
    };

    template<typename VP, typename EP>
    struct G<VP, EP, NoProperty>
    {
        vector<V<VP> > v;
        vector<E<EP> > e;
    };


    } // AdjacencyListPrivate





template<typename VP = NoProperty, 
         typename EP = NoProperty, 
         typename GP = NoProperty >
class AdjacencyList: public AdjacencyListPrivate::G<VP, EP, GP>
{
    typedef AdjacencyListPrivate::V<VP> V;
    typedef AdjacencyListPrivate::E<EP> E;
    typedef AdjacencyListPrivate::G<VP, EP, GP> G;
    
public:
    typedef VP VertexProperties;
    typedef EP EdgeProperties;
    typedef GP GraphProperties;

    explicit AdjacencyList(int n = 0, const VP & vp = VP())
    { 
        this->v.assign(n, V(-1, vp));
    }

    void clear() { this->v.clear(); this->e.clear(); }
    void resize(int n, const VP &vp = VP()) { this->e.clear(); this->v.assign(n, vp); }

    const VP& vertexProperties(int u) const { return this->v[u].vp; }
    VP& vertexProperties(int u) { return this->v[u].vp; }

    const GP& graphProperties(int u) const { return this->gp; }
    GP& graphProperties(int u) { return this->gp; }


    class EdgeIndex
    {
        friend class AdjacencyList<VP, EP, GP>;
        int from, id;
        EdgeIndex() = default;
        EdgeIndex(int _from, int _id):from(_from), id(_id){}
    public:
        bool operator==(const EdgeIndex &o)const { return id == o.id && from == o.from; }
        bool operator!=(const EdgeIndex &o)const { return !(*this == o); }
    };
    
    int source(const EdgeIndex &eid) const { return eid.from; }
    const int& target(const EdgeIndex &eid) const { return this->e[eid.id].to; }
    int& target(const EdgeIndex &eid) { return this->e[eid.id].to; }

    const EP& edgeProperties(const EdgeIndex &eid) const { return this->e[eid.id].ep; }
    EP& edgeProperties(const EdgeIndex &eid) { return this->e[eid.id].ep; }

    // only work when add edge and inverseEdge continuously, 
    // and first edge number is even number.
    // otherwise, return will not guarantee right.
    EdgeIndex inverseEdge(const EdgeIndex &eid) const { return EdgeIndex(this->e[eid.id].to, eid.id ^ 1);}

    int vertexNumber() const { return v.size(); }
    int edgeNumber() const { return e.size(); }
    

    class OutEdgeIterator
    {
        friend class AdjacencyList<VP, EP, GP>;
        EdgeIndex i;
        vector<E> *e;
        OutEdgeIterator(const EdgeIndex &_i, vector<E> *_e):i(_i), e(_e) {}
    public:
        OutEdgeIterator() = default;

        const EdgeIndex& operator*() const { return i; }

        OutEdgeIterator& operator++() { i.id = (*e)[i.id].next;  return *this; }
        OutEdgeIterator operator++(int) { OutEdgeIterator t(*this); ++*this;  return t; }
        bool operator==(const OutEdgeIterator &o) const { return i == o.i; }
        bool operator!=(const OutEdgeIterator &o) const { return !(*this == o); }
    };

    pair<OutEdgeIterator, OutEdgeIterator> outEdges(int u)
    {

        return make_pair(OutEdgeIterator(EdgeIndex(u, G::v[u].head), &this->e), 
                         OutEdgeIterator(EdgeIndex(u, -1), &this->e) );
    }
    void addEdge(int a, int b, const EP &ep = EP())
    {
        this->e.push_back(E(b, this->v[a].head, ep));
        this->v[a].head = int(this->e.size()) - 1;
    }








};






















} // namespace lz


#endif // ADJACENCY_LIST_H