#ifndef GRAPH_TRAITS_H
#define GRAPH_TRAITS_H


template<typename Graph>
struct GraphTraits
{

    typedef typename Graph::VertexProperties VertexProperties;
    typedef typename Graph::EdgeProperties EdgeProperties;
    typedef typename Graph::GraphProperties GraphProperties;
    
    typedef typename Graph::EdgeDescriptor EdgeDescriptor;
    typedef typename Graph::VertexDescriptor VertexDescriptor;
    typedef typename Graph::OutEdgeIterator OutEdgeIterator;

};




#endif // GRAPH_TRAITS_H
