#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H


#include <vector>
#include <algorithm>
#include <tuple>
#include <memory>
using std::allocator;
using std::vector;
using std::endl;
using std::cout;
using std::pair;
using std::make_pair;
using std::ostream;
using std::tuple;
using std::get;
using std::make_tuple;


// class AdjacencyList
// {
// public:
//     explicit AdjacencyList(int n = 0):head(n, -1) {}
//     void clear(int n) { head.assign(n, -1); }
//     inline int begin(int vid) const { return head[vid]; }
//     inline int to(int eid) const { return e[eid].first; }
//     inline int next(int eid) const { return e[eid].second; }
//     inline int vertexNumber() const { return head.size(); }
//     inline int edgeNumber() const { return e.size(); }
//     void addEdge(int a, int b)
//     {
//         e.push_back(make_pair(b, head[a]));
//         head[a] = int(e.size()) - 1;
//     }
//     void addBidirectionalEdge(int a, int b)
//     {
//         addEdge(a, b);
//         addEdge(b, a);
//     }
//     friend ostream& operator<<(ostream &out, const AdjacencyList & g)
//     {
//         out << "AdjacencyList:" << endl;
//         for(int i = 0; i < g.vertexNumber(); ++ i)
//         {
//             out << "i=" << i << ": ";
//             for(int j = g.begin(i); ~j; j = g.next(j))
//             {
//                 out << g.to(j) << " ,";
//             }
//             out << endl;
//         }
//     }
// private:
//     vector<int> head;
//     vector<pair<int, int> > e;
// };

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
            std::cout << "cnt: " << i<< std::endl;
            for(int j = begin(i); ~j; j = next(j))
            {
                std::cout << to(j) << " ";
            }
            std::cout << std::endl;
        }
    }

private:  

    

    vector<AdjacencyListVertex<VertexProperty> > v;
    vector<AdjacencyListEdge<EdgeProperty> > e;
};



















// template<typename VertexProperty>
// class AdjacencyList<NoProperty, VertexProperty>
// {
// public:
//     explicit AdjacencyList(int n = 0):head(n, make_tuple(-1, VertexProperty()) ) {}
//     void clear(int n) { head.assign(n, make_tuple(-1, VertexProperty()) ); }
//     inline int begin(int vid) const { return get<0>(head[vid]); }
//     inline VertexProperty vertexProperty(int vid) const { return get<1>(head[vid]); }
//     inline VertexProperty setVertexProperty(int vid, const VertexProperty &vp) const 
//         { return get<1>(head[vid]) = vp; }

//     inline int to(int eid) const { return get<0>(e[eid]); }
//     inline int next(int eid) const { return get<1>(e[eid]); }


//     inline int vertexNumber() const { return head.size(); }
//     inline int edgeNumber() const { return e.size(); }
    
//     void addEdge(int a, int b)
//     {
//         e.push_back(make_tuple(a, b));
//         get<0>(head[a]) = int(e.size()) - 1;
//     }
//     void addBidirectionalEdge(int a, int b)
//     {
//         addEdge(a, b);
//         addEdge(b, a);
//     }
// private:

//     vector<tuple<int, VertexProperty> > head; 
//     vector<tuple<int, int> > e;

// };

// template<typename EdgeProperty>
// class AdjacencyList<EdgeProperty, NoProperty>
// {
// public:
//     explicit AdjacencyList(int n = 0):head(n, -1) {}
//     void clear(int n) { head.assign(n, -1); }
//     inline int begin(int vid) const { return (head[vid]); }
    

//     inline int to(int eid) const { return get<0>(e[eid]); }
//     inline int next(int eid) const { return get<1>(e[eid]); }
//     inline EdgeProperty edgeProperty(int eid) const { return get<2>(e[eid]); }
//     inline EdgeProperty setEdgeProperty(int eid, const EdgeProperty &ep) const 
//         { return get<2>(e[eid]) = ep; }

//     inline int vertexNumber() const { return head.size(); }
//     inline int edgeNumber() const { return e.size(); }
    
//     void addEdge(int a, int b, const EdgeProperty &ep)
//     {
//         e.push_back(make_tuple(a, b, ep));
//         head[a] = int(e.size()) - 1;
//     }
//     void addBidirectionalEdge(int a, int b, const EdgeProperty &ep)
//     {
//         addEdge(a, b, ep);
//         addEdge(b, a, ep);
//     }
// private:

//     vector<int > head; //0 for head, 1 for VertexProperty
//     vector<tuple<int, int, EdgeProperty> > e; //0 for to, 1 for next, 2 for EdgeProperty

// };




// template<>
// class AdjacencyList<NoProperty, NoProperty>
// {
// public:
//     explicit AdjacencyList(int n = 0): head(n, -1)  {}
//     void clear(int n) { head.assign(n, -1); }
//     inline int begin(int vid) const { return head[vid]; }
//     inline int to(int eid) const { return get<0>(e[eid]); }
//     inline int next(int eid) const { return get<1>(e[eid]); }
//     inline int vertexNumber() const { return head.size(); }
//     inline int edgeNumber() const { return e.size(); }    
//     void addEdge(int a, int b)
//     {
//         e.push_back(make_tuple(a, b));
//         head[a] = int(e.size()) - 1;
//     }
//     void addBidirectionalEdge(int a, int b)
//     {
//         addEdge(a, b);
//         addEdge(b, a);
//     }
// private:
//     vector<int > head;
//     vector<tuple<int, int> > e;
// };






































#endif //ADJACENCY_LIST_H