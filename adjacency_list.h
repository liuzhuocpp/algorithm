#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H


#include <vector>
#include <tuple>

#include "graph_utility.h"

namespace lz {


using std::vector;
using std::cout;
using std::endl;



// define vertex info in AdjacencyList
template<typename VP>
struct ALVertex
{
    int head;
    VP vp;
    ALVertex(const int& _head, const VP &_vp):
        head(_head), vp(_vp){}
    ALVertex(int && _head, VP &&_vp):
        head(_head), vp(_vp){}
};
template<>
struct ALVertex<NoProperty>
{
    int head;
    ALVertex(const int& _head, const NoProperty &_vp):
        head(_head){}
    ALVertex(int && _head, NoProperty &&_vp):
        head(_head){}
};
// define edge info in AdjacencyList
template<typename EP>
struct ALEdge
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

    explicit AdjacencyList(int n = 0, const GP _gp = GP(), int expect_m = 0):
        v(n, ALVertex<VP>(-1, VP())), gp(_gp) 
        {
            e.reserve(expect_m);
        }
    void clear(int n, int expect_m = 0)
    {
        v.assign(n, ALVertex<VP>(-1, VP()));
        e.clear();
        e.reserve(expect_m);
    }
    VP& vertexProperty(int i) { return v[i].vp; }
    GP& graphProperty() { return gp; }

    void add(int a, int b, const EP & ep = EP())
    {
        ALEdge<EP> ale;
        ale.ae = AdjacencyEdge<EP>(b, ep);
        ale.next = v[a].head;
        e.push_back(std::move(ale));
        v[a].head = int(e.size()) - 1;
    }
    void add(int a, int b, EP && ep)
    {
        ALEdge<EP> ale;
        ale.ae = AdjacencyEdge<EP>(std::move(b), std::move(ep));
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
        vector<ALEdge<EP> > *e;
        int eid;
        Iterator(vector<ALEdge<EP> > *_e, int _eid):
            e(_e), eid(_eid){}
    public:
        Iterator():e(0), eid(-1){}
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


    class ConstIterator
    {
        friend class AdjacencyList<VP, EP, GP>;
        const vector<ALEdge<EP> > *e;
        int eid;
        ConstIterator(const vector<ALEdge<EP> > *_e, int _eid):
            e(_e), eid(_eid){}
    public:
        ConstIterator():e(0), eid(-1){}
        ConstIterator& operator++()
        {
            eid = (*e)[eid].next;
            return *this;
        }
        ConstIterator operator++(int)
        {
            ConstIterator t(e, eid);
            eid = (*e)[eid].next;
            return t;
        }
        bool operator==(const ConstIterator &o) const { return e == o.e && eid == o.eid; }
        bool operator!=(const ConstIterator &o) const { return !(*this == o); }
        const AdjacencyEdge<EP>& operator*() const { return (*e)[eid].ae;  }
        const AdjacencyEdge<EP>* operator->() const { return &((*e)[eid].ae); }
    };
    ConstIterator begin(int i) const { return ConstIterator(&e, v[i].head); }    
    ConstIterator end(int i) const { return ConstIterator(&e, -1); }

    typedef Iterator iterator;
    typedef ConstIterator const_iterator;







    AdjacencyEdge<EP>& edge(int eid) { return e[eid].ae; }
private:
    vector<ALVertex<VP> > v;
    vector<ALEdge<EP> > e;
    GP gp;
};









} // namespace lz


#endif // ADJACENCY_LIST_H