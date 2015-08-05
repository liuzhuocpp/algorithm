#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H


#include <vector>
#include <tuple>
#include <algorithm>
#include "property.h"
#include "graph_utility.h"
#include "graph_traits.h"
namespace lz {


using std::vector;
using std::cout;
using std::endl;
using std::pair;
using std::make_pair;

    namespace AdjacencyListPrivate {

    // vertex data
    template<typename VP>
    struct VertexData
    {
        int head;
        VP vp;
        VertexData():head(-1){}
        VertexData(int head, const VP &vp):head(head), vp(vp) {}
    };
    template<>
    struct VertexData<NoProperty>
    {
        int head;
        VertexData():head(-1){}
        VertexData(int head, const NoProperty &vp):head(head) {}
    };

    // edge data
    template<typename EP>
    struct EdgeData
    {
        int source, target, next;
        EP ep;
        EdgeData() = default;
        EdgeData(int source, int target, int next, const EP & ep)
            :source(source), target(target), next(next), ep(ep){}
    };
    template<>
    struct EdgeData<NoProperty>
    {
        int source, target, next;
        EdgeData() = default;
        EdgeData(int source, int target, int next, const NoProperty & ep)
            :source(source), target(target), next(next){}
    };

    // graph data
    template<typename VP, typename EP, typename GP>
    struct GraphData
    {
        vector<VertexData<VP> > v;
        vector<EdgeData<EP> > e;
        GraphData gp;
    };

    template<typename VP, typename EP>
    struct GraphData<VP, EP, NoProperty>
    {
        vector<VertexData<VP> > v;
        vector<EdgeData<EP> > e;
    };

    } // AdjacencyListPrivate


template<typename Directed = Directed,
         typename VP = NoProperty, 
         typename EP = NoProperty, 
         typename GP = NoProperty>
class AdjacencyList: private AdjacencyListPrivate::GraphData<VP, EP, GP>
{
    typedef AdjacencyListPrivate::VertexData<VP> VertexData;
    typedef AdjacencyListPrivate::EdgeData<EP> EdgeData;
    
public:
    typedef VP VertexProperties;
    typedef EP EdgeProperties;
    typedef GP GraphProperties;
    
    typedef int EdgeDescriptor;
    typedef int VertexDescriptor;


    explicit AdjacencyList(int n = 0, const VP & vp = VP())
    { 
        this->v.assign(n, VertexData(-1, vp));
    }
    void clear()
    {
        this->v.clear();
        this->e.clear(); 
    }
    // void assignVertex(int n = 0, const VP & vp = VP())
    // {
    //     this->v.assign(n, VD(-1, vp));
    // }
    int addVertex(const VP &vp = VP())
    {
        this->v.push_back(VertexData(-1, vp));
        return this->v.size() - 1;
    }

    const VP& vertexProperties(int u) const { return this->v[u].vp; }
    VP& vertexProperties(int u) { return this->v[u].vp; }

    const GP& graphProperties() const { return this->gp; }
    GP& graphProperties() { return this->gp; }
    
    int source(int e) const { return this->e[e].source; }
    int target(int e) const { return this->e[e].target; }
    const EP& edgeProperties(int e) const { return this->e[e].ep; }
    EP& edgeProperties(int e) { return this->e[e].ep; }

    int vertexNumber() const { return this->v.size(); }
    int edgeNumber() const { return this->e.size(); }

    class OutEdgeIterator
    {
    protected:
        friend class AdjacencyList;
        int i;
        const vector<EdgeData> *e;
    
        OutEdgeIterator(int i, const vector<EdgeData> *e): i(i), e(e) {}
    public:
        OutEdgeIterator():i(0), e(nullptr) {}

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
    void addEdge(int a, int b, const EP &ep = EP())
    {
        this->e.push_back(EdgeData(a, b, this->v[a].head, ep));
        this->v[a].head = int(this->e.size()) - 1;
    }
};




template<typename VP, typename EP, typename GP>
class AdjacencyList<Undirected, VP, EP, GP>: private AdjacencyListPrivate::GraphData<VP, EP, GP>
{
    typedef AdjacencyListPrivate::VertexData<VP> VertexData;
    typedef AdjacencyListPrivate::EdgeData<EP> EdgeData;

public:
    typedef VP VertexProperties;
    typedef EP EdgeProperties;
    typedef GP GraphProperties;

    typedef int EdgeDescriptor;
    typedef int VertexDescriptor;


    explicit AdjacencyList(int n = 0, const VP & vp = VP())
    {
        this->v.assign(n, VertexData(-1, vp));
    }
    void clear()
    {
        this->v.clear();
        this->e.clear();
    }
    // void assignVertex(int n = 0, const VP & vp = VP())
    // {
    //     this->v.assign(n, VD(-1, vp));
    // }
    int addVertex(const VP &vp = VP())
    {
        this->v.push_back(VertexData(-1, vp));
        return this->v.size() - 1;
    }

    const VP& vertexProperties(int u) const { return this->v[u].vp; }
    VP& vertexProperties(int u) { return this->v[u].vp; }

    const GP& graphProperties() const { return this->gp; }
    GP& graphProperties() { return this->gp; }

    int source(int e) const { return this->e[e << 1].source; }
    int target(int e) const { return this->e[e << 1].target; }
    const EP& edgeProperties(int e) const { return this->e[e << 1].ep; }
    EP& edgeProperties(int e) { return this->e[e << 1].ep; }

    int vertexNumber() const { return this->v.size(); }
    int edgeNumber() const { return this->e.size() >> 1; }

    class OutEdgeIterator
    {
        friend class AdjacencyList;
        int i;
        const vector<EdgeData> *e;

        OutEdgeIterator(int i, const vector<EdgeData> *e): i(i), e(e) {}
    public:
        OutEdgeIterator():i(-1), e(nullptr) {}

        int operator*() const
        {
        	if(i == -1) return -1;
        	return i >> 1;
        }
        OutEdgeIterator& operator++() { i = (*e)[i].next;  return *this; }
        OutEdgeIterator operator++(int) { OutEdgeIterator t(*this); ++*this;  return t; }
        bool operator==(const OutEdgeIterator &o) const
    	{
        	return i == o.i && e == o.e;
    	}
        bool operator!=(const OutEdgeIterator &o) const { return !(*this == o); }
    };
    pair<OutEdgeIterator, OutEdgeIterator> outEdges(int u) const
    {
        return make_pair(OutEdgeIterator(this->v[u].head, &this->e),
                         OutEdgeIterator(-1, &this->e ) ) ;
    }
    void addEdge(int a, int b, const EP &ep = EP())
    {
    	_addEdge(a, b, ep);
    	_addEdge(b, a, ep);
    }
private:
    void _addEdge(int a, int b, const EP &ep = EP())
    {
        this->e.push_back(EdgeData(a, b, this->v[a].head, ep));
        this->v[a].head = int(this->e.size()) - 1;
    }
};



















} // namespace lz


#endif // ADJACENCY_LIST_H
