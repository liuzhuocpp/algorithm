#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H


#include <vector>
#include <tuple>
#include <algorithm>
// #include "graph_utility.h"
#include "property.h"

namespace lz {


using std::vector;
using std::cout;
using std::endl;
using std::pair;
using std::make_pair;


/**
 * 
 *
 *
 *
*/
    namespace AdjacencyListPrivate {

    // vertex data
    template<typename VP>
    struct VD
    {
        int head;
        VP vp;
        VD() = default;
        VD(int _h, const VP &_vp):head(_h), vp(_vp) {}
    };
    template<>
    struct VD<NoProperty>
    {
        int head;
        VD() = default;
        VD(int _h, const NoProperty &_vp):head(_h) {}
    };

    // edge data
    template<typename EP>
    struct ED
    {
        int source, target, next;
        EP ep;
        ED() = default;
        ED(int _source, int _target, int _next, const EP & _ep)
            :source(_source), target(_target), next(_next), ep(_ep){}
    };
    template<>
    struct ED<NoProperty>
    {
        int source, target, next;
        ED() = default;
        ED(int _source, int _target, int _next, const NoProperty & _ep)
            :source(_source), target(_target), next(_next){}
    };

    // graph data
    template<typename VP, typename EP, typename GP>
    struct GD
    {
        vector<VD<VP> > v;
        vector<ED<EP> > e;
        GP gp;
    };

    template<typename VP, typename EP>
    struct GD<VP, EP, NoProperty>
    {
        vector<VD<VP> > v;
        vector<ED<EP> > e;
    };


    

    struct ReverseEdge
    {
        int operator()(int e) const
        {
            return e ^ 1;
        }
    };


    } // AdjacencyListPrivate





template<typename VP = NoProperty, 
         typename EP = NoProperty, 
         typename GP = NoProperty,
         typename ReverseEdge = AdjacencyListPrivate::ReverseEdge >
class AdjacencyList: private AdjacencyListPrivate::GD<VP, EP, GP>
{
    typedef AdjacencyListPrivate::VD<VP> VD;
    typedef AdjacencyListPrivate::ED<EP> ED;
    typedef AdjacencyListPrivate::GD<VP, EP, GP> GD;
    
public:
    typedef VP VertexProperties;
    typedef EP EdgeProperties;
    typedef GP GraphProperties;
    
    typedef int EdgeDescriptor;
    typedef int VertexDescriptor;


    explicit AdjacencyList(int n = 0, const VP & vp = VP())
    { 
        this->v.assign(n, VD(-1, vp));
    }
    void clear()
    {
        this->v.clear();
        this->e.clear(); 
    }
    void assignVertex(int n = 0, const VP & vp = VP())
    {
        this->v.assign(n, VD(-1, vp));
    }
    int addVertex(const VP &vp = VP())
    {
        this->v.push_back(VD(-1, vp));
        return this->v.size() - 1;
    }

    const VP& vertexProperties(int u) const { return this->v[u].vp; }
    VP& vertexProperties(int u) { return this->v[u].vp; }

    const GP& graphProperties() const { return this->gp; }
    GP& graphProperties() { return this->gp; }
    
    int target(const int &eid) const { return this->e[eid].target; }
    int source(const int &eid) const { return this->e[eid].source; }

    int reverseEdge(const int eid) const { return ReverseEdge()(eid); }


    const EP& edgeProperties(const int &eid) const { return this->e[eid].ep; }
    EP& edgeProperties(const int &eid) { return this->e[eid].ep; }

    int vertexNumber() const { return this->v.size(); }
    int edgeNumber() const { return this->e.size(); }   

    class OutEdgeIterator
    {
        friend class AdjacencyList;
        int i;
        const vector<ED> *e;
        OutEdgeIterator(const int &_i, const vector<ED> *_e): i(_i), e(_e) {}
    public:
        OutEdgeIterator() = default;

        int operator*() const { return i; }
        OutEdgeIterator& operator++() { i = (*e)[i].next;  return *this; }
        OutEdgeIterator operator++(int) { OutEdgeIterator t(*this); ++*this;  return t; }
        bool operator==(const OutEdgeIterator &o) const { return i == o.i && e == o.e; }
        bool operator!=(const OutEdgeIterator &o) const { return !(*this == o); }
    };
    pair<OutEdgeIterator, OutEdgeIterator> outEdges(int u) const
    {
        return make_pair(OutEdgeIterator(this->v[u].head, &this->e), 
                         OutEdgeIterator(-1, &this->e ) ) ;
    }



    class EdgeIterator
    {
        friend class AdjacencyList ;
        int i;
        
        EdgeIterator(int _i): i(_i) {}
    public:
        EdgeIterator() = default;
        bool operator==(const EdgeIterator &o) const { return i == o.i; }
        bool operator!=(const EdgeIterator &o) const { return !(*this == o); }
        int operator*() const { return i; }
        EdgeIterator& operator++() { ++ i; return *this; }
        EdgeIterator operator++(int) { EdgeIterator t(*this); ++*this;  return t; }

        EdgeIterator& operator--() { -- i; return *this; }
        EdgeIterator operator--(int) { EdgeIterator t(*this); --*this;  return t; }


        friend EdgeIterator operator+(const EdgeIterator &it, int d)
            {   return EdgeIterator(it.i + d); }
        friend EdgeIterator operator+(int d, const EdgeIterator &it)
            {   return EdgeIterator(it.i + d); }

        EdgeIterator operator-(int d) 
            {   return EdgeIterator(i - d); }
        EdgeIterator operator-=(int d)
            {   i -= d;  return *this; }
        EdgeIterator operator+=(int d)
            {   i += d;  return *this; }
    };


    pair<EdgeIterator, EdgeIterator> edges() const
    {
        return make_pair(EdgeIterator(0), 
                         EdgeIterator(this->e.size()) ) ;
    }



    void addEdge(int a, int b, const EP &ep = EP())
    {
        this->e.push_back(ED(a, b, this->v[a].head, ep));
        this->v[a].head = int(this->e.size()) - 1;
    }









};
























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



















} // namespace lz


#endif // ADJACENCY_LIST_H