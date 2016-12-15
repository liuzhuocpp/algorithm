#ifndef DEPTH_FIRST_SEARCH_H
#define DEPTH_FIRST_SEARCH_H

#include <tuple>
#include <algorithm>
#include <vector>
#include <type_traits>
#include <iostream>
#include <memory>


#include "lz/utility.h"

#include "lz/graph_utility.h"
#include "lz/std_utility.h"
#include "lz/map.h"
#include "lz/parameter.h"

namespace lz {

using std::tie;
using std::pair;
using std::vector;
using std::cout;
using std::endl;


namespace DepthFirstSearchKeywords {

    LZ_PARAMETER_KEYWORD(tag, startVertex)
    LZ_PARAMETER_KEYWORD(tag, discoverVertex)
    LZ_PARAMETER_KEYWORD(tag, examineEdge)
    LZ_PARAMETER_KEYWORD(tag, treeEdge)
    LZ_PARAMETER_KEYWORD(tag, treeEdgeReturn)
    LZ_PARAMETER_KEYWORD(tag, notTreeEdge)
    LZ_PARAMETER_KEYWORD(tag, finishEdge)
    LZ_PARAMETER_KEYWORD(tag, finishVertex)

    LZ_PARAMETER_KEYWORD(tag, enterVertex)

    // only for graph DFS
    LZ_PARAMETER_KEYWORD(tag, marker)
    LZ_PARAMETER_KEYWORD(tag, vertexIndexMap)

}

namespace DepthFirstSearchPrivate {

    using namespace DepthFirstSearchKeywords;

    template<typename G, typename ParamPack, typename Marker>
    void dfs(const G &g, const ParamPack &p, Marker &marker, typename GraphTraits<G>::VertexDescriptor u)
    {
        p[startVertex | emptyFunction](u);
        for(auto e: outEdges(g, u))
        {
            typename GraphTraits<G>::VertexDescriptor to = opposite(g, e, u);
            p[examineEdge | emptyFunction](e, u, to);
            if(!marker.isMark(to))
            {
                p[treeEdge | emptyFunction](e, u, to);
                marker.mark(to);
                dfs(g, p, marker, to);
                p[treeEdgeReturn | emptyFunction](e, u, to);
            }
            else
            {
                p[notTreeEdge | emptyFunction](e, u, to);
            }
            p[finishEdge | emptyFunction](e, u, to);
        }
        p[finishVertex | emptyFunction](u);
    }

    template<typename G, typename ParamPack>
    void treeDFS(const G &g, const ParamPack &p,
            typename GraphTraits<G>::VertexDescriptor u,
            typename GraphTraits<G>::VertexDescriptor fa)
    {
        p[startVertex | emptyFunction](u);
        for(auto e: outEdges(g, u))
        {
            typename GraphTraits<G>::VertexDescriptor to = opposite(g, e, u);
            p[examineEdge | emptyFunction](e, u, to);
            if(to != fa)
            {
                p[treeEdge | emptyFunction](e, u, to);
                treeDFS(g, p, to, u);
                p[treeEdgeReturn | emptyFunction](e, u, to);
            }
            else
            {
                p[notTreeEdge | emptyFunction](e, u, to);
            }
            p[finishEdge | emptyFunction](e, u, to);
        }
        p[finishVertex | emptyFunction](u);
    }

}


template<typename G, typename ParamPack>
void depthFirstSearch(const G &g, const ParamPack &p)
{
    namespace Keys = DepthFirstSearchKeywords;

    auto indexMap = p[Keys::vertexIndexMap | vertexPropertyMap(g, vertexIndexTag)];
    auto n = verticesNumber(g);

    decltype(auto) marker = p[Keys::marker || [&]() {
        return IndexMarker<decltype(indexMap)>(indexMap, n);
    }];

    auto enterVertex = p[Keys::enterVertex | GraphTraits<G>::nullVertex()];
    if(enterVertex != GraphTraits<G>::nullVertex())
    {
        p[Keys::startVertex | emptyFunction];
        marker.mark(enterVertex);
        DepthFirstSearchPrivate::dfs(g, p, marker, enterVertex);
    }
    else
    {
        for(auto u: vertices(g))
        {
            if(!marker.isMark(u))
            {
                p[Keys::startVertex | emptyFunction];
                marker.mark(u);
                DepthFirstSearchPrivate::dfs(g, p, marker, u);
            }
        }
    }
}

template<typename G, typename ParamPack>
void treeDFS(const G &g, const ParamPack &params)
{
    namespace Keys = DepthFirstSearchKeywords;
    auto enterVertex = params[Keys::enterVertex | *vertices(g).first];

    params[Keys::startVertex | emptyFunction];
    DepthFirstSearchPrivate::treeDFS(g, params, enterVertex, enterVertex);
}






//
//
//
//namespace DepthFirstSearchKeywords {
//
//	LZ_PARAMETER_KEYWORD(tag, initializeVertex)
//	LZ_PARAMETER_KEYWORD(tag, startVertex)
//	LZ_PARAMETER_KEYWORD(tag, discoverVertex)
//	LZ_PARAMETER_KEYWORD(tag, examineEdge)
//	LZ_PARAMETER_KEYWORD(tag, treeEdge)
//	LZ_PARAMETER_KEYWORD(tag, treeEdgeReturn)
//	LZ_PARAMETER_KEYWORD(tag, notTreeEdge)
//	LZ_PARAMETER_KEYWORD(tag, finishEdge)
//	LZ_PARAMETER_KEYWORD(tag, finishVertex)
//
//
//	LZ_PARAMETER_KEYWORD(tag, colorMap)
//	LZ_PARAMETER_KEYWORD(tag, white)
//	LZ_PARAMETER_KEYWORD(tag, black)
//	LZ_PARAMETER_KEYWORD(tag, vertexIndexMap)
//
//
//	LZ_PARAMETER_KEYWORD(tag, isInit)
//	LZ_PARAMETER_KEYWORD(tag, enterVertex)
//
//	//	LZ_PARAMETER_KEYWORD(tag, isTree)
//	//	LZ_PARAMETER_KEYWORD(tag, outEdges)
//}
//
//
//
//	namespace DepthFirstSearchPrivate {
//
//
//	using namespace DepthFirstSearchKeywords;
//
//
//	template<typename G, typename ParamPack>
//	struct Impl
//	{
//		using Vertex = typename GraphTraits<G>::VertexDescriptor;
//		using Edge = typename GraphTraits<G>::EdgeDescriptor;
//		using OutEdgeIterator = typename GraphTraits<G>::OutEdgeIterator;
////		using DirectedTag = typename GraphTraits<G>::DirectedCategory;
//
//		const G &g;
//		const ParamPack &p;
//
//
//		Impl(const G &g, const ParamPack &p):g(g), p(p)
//		{
//			init(p[isInit]);
////			cout << "II" << endl;
//			chooseEnterVertex(p[enterVertex]);
//		}
//
//
//		void dfsNotRecursive(Vertex u)
//		{
//			std::vector<std::pair<Vertex, OutEdgeIterator > > stack;
//
//			p[discoverVertex](u);
//			p[colorMap][u] = p[black]();
//			stack.push_back(std::make_pair(u,  g.outEdges(u).first)   );
//
//
//			while(!stack.empty())
//			{
//				auto u = stack.back().first;
//				auto& ei = stack.back().second;
//
//				for(;ei !=  g.outEdges(u).second; ++ ei)
//				{
//					auto e = *ei;
//					auto to = opposite(g, e, u);
//					p[examineEdge](e, u);
//					if(p[colorMap][to] == p[white]())
//					{
//						p[treeEdge](e, u);
//
//						p[discoverVertex](to);
//						p[colorMap][to] = p[black]();
//						stack.push_back(std::make_pair(to,  g.outEdges(to).first));
//						break;
//					}
//					else
//					{
//						p[notTreeEdge](e, u);
//					}
//					p[finishEdge](e, u);
//				}
//				if(ei == g.outEdges(u).second)
//				{
//					p[finishVertex](u);
//
//					stack.pop_back();
//					if(!stack.empty())
//					{
//						p[treeEdgeReturn](*stack.back().second, stack.back().first);
//						++stack.back().second;
//					}
//				}
//
//			}
//
//		}
//
//
//
//
//
//
//		void dfs(Vertex u)
//		{
//			p[colorMap][u] = p[black]();
//			p[discoverVertex](u);
//			std::pair<OutEdgeIterator, OutEdgeIterator> ei_pair = g.outEdges(u);
//			for(auto ei = ei_pair.first; ei != ei_pair.second; ++ ei)
//			{
//				Edge e = *ei;
//				Vertex to = opposite(g, e, u);
//				p[examineEdge](e, u);
//				if(p[colorMap][to] == p[white]())
//				{
//					p[treeEdge](e, u);
//					dfs(to);
//					p[treeEdgeReturn](e, u);
//				}
//				else
//				{
//					p[notTreeEdge](e, u);
//				}
//				p[finishEdge](e, u);
//			}
//			p[finishVertex](u);
//		}
////		void dfsPolytree(Vertex u)
////		{
////			p[discoverVertex](u);
////			for(auto ei = p[outEdges](g, u); ei.first != ei.second; ++ ei.first)
////			{
////				auto e = *ei.first;
////				auto to = g.target(e);
////				p[examineEdge](e, u);
////				p[treeEdge](e, u);
////				dfsPolytree(to);
////				p[treeEdgeReturn](e, u);
////				p[finishEdge](e, u);
////			}
////			p[finishVertex](u);
////		}
////
////		void dfsTree(Vertex u, Vertex fa)
////		{
////			p[discoverVertex](u);
////			for(auto ei = p[outEdges](g, u); ei.first != ei.second; ++ ei.first)
////			{
////				auto e = *ei.first;
////				auto to = lz::opposite(g, e, u);
////				p[examineEdge](e, u);
////				if(to != fa)
////				{
////					p[treeEdge](e, u);
////					dfsTree(to, u);
////					p[treeEdgeReturn](e, u);
////				}
////				else
////				{
////					p[notTreeEdge](e, u);
////				}
////				p[finishEdge](e, u);
////			}
////			p[finishVertex](u);
////		}
//
//		template<typename IsInit>
//		void init(IsInit) {}
//		void init(std::true_type)
//		{
//			for(auto vi = g.vertices(); vi.first != vi.second; ++vi.first)
//			{
//				auto u = *vi.first;
//				p[colorMap][u] = p[white]();
//				p[initializeVertex](u);
//			}
//		}
//
////		void init(std::true_type, std::true_type)
////		{
////			for(auto vi = g.vertices(); vi.first != vi.second; ++vi.first)
////			{
////				auto u = *vi.first;
////				p[initializeVertex](u);
////			}
////		}
//
//
//
//
//
//		template<typename EnterVertex>
//		void chooseEnterVertex(EnterVertex u)
//		{
//			p[startVertex](u);
//			dfsDispatch(u);
//		}
//		void chooseEnterVertex(ParamNotFound )
//		{
//			for(auto vi = g.vertices();vi.first != vi.second; ++vi.first)
//			{
//				auto u = *vi.first;
//				if(p[colorMap][u] == p[white]())
//				{
//					p[startVertex](u);
//					dfsDispatch(u);
//				}
//			}
//		}
//
////		void chooseEnterVertex(ParamNotFound, std::true_type )
////		{
////			auto u = *g.vertices().first;
////			p[startVertex](u);
////			dfsDispatch(u, DirectedTag(), p[isTree]);
////		}
//
//
//
//
//
//		template<typename EnterVertex>
//		void dfsDispatch(EnterVertex u)
//		{
////			dfsNotRecursive(u);
//			dfs(u);
//		}
//
////		template<typename EnterVertex>
////		void dfsDispatch(EnterVertex u, DirectedGraphTag, std::true_type)
////		{
////			dfsPolytree(u);
////		}
////
////		template<typename EnterVertex>
////		void dfsDispatch(EnterVertex u, UndirectedGraphTag, std::true_type)
////		{
////			dfsTree(u, u);
////		}
//
//
//
//	};
//
//
//
//
//	template<typename G, typename ParamPack>
//	void dispatch(const G &g, const ParamPack &p )
//	{
////		cout << "SB33333" << endl;
//		auto _white = p[white | ColorTraits<>::white];
//		auto _black = p[black | ColorTraits<>::black];
////		auto _vertexIndexMap = p[vertexIndexMap | g.vertexPropertyMap(VertexIndexTag())];
//		auto n = g.verticesNumber();
//		auto indexMap = p[vertexIndexMap | g.vertexPropertyMap(VertexIndexTag())];
//		auto _colorMap = p[colorMap ||
//						   std::bind(makeVertexIndexComposeMap<decltype(_white()), decltype(indexMap), decltype(n) >,
//								   indexMap, n)];
//
//		auto _enterVertex = p[enterVertex];
//		auto _isInit = p[isInit | std::true_type()];
////		auto _outEdges = p[outEdges | std::mem_fn(&G::outEdges)];
////		auto _isTree = std::false_type();
//
//
//		using V = typename GraphTraits<G>::VertexDescriptor;
//		using E = typename GraphTraits<G>::EdgeDescriptor;
//
//		auto actionVertex = [](V u){};
//		auto actionEdge = [](E e, V u){};
//
//		auto newParamPack = (
//				initializeVertex = p[initializeVertex | actionVertex],
//				startVertex = p[startVertex | actionVertex],
//				discoverVertex = p[discoverVertex | actionVertex],
//				examineEdge = p[examineEdge | actionEdge],
//				treeEdge = p[treeEdge | actionEdge],
//				treeEdgeReturn = p[treeEdgeReturn | actionEdge],
//				notTreeEdge = p[notTreeEdge | actionEdge],
//				finishEdge = p[finishEdge | actionEdge],
//				finishVertex = p[finishVertex | actionVertex],
//
//
//
//				white = _white,
//				black = _black,
//				colorMap = _colorMap,
//
////				isTree = _isTree,
////				outEdges = _outEdges,
//				isInit = _isInit,
//				enterVertex =  _enterVertex);
//
//
//
//
//		DepthFirstSearchPrivate::Impl <G, decltype(newParamPack)> impl(g, newParamPack);
//
//
//	}
//
//	// this graph is a tree;
////	template<typename G, typename ParamPack>
////	void dispatch(const G &g, const ParamPack &p, std::true_type )
////	{
////		auto _enterVertex = p[enterVertex];
////		auto _isInit = p[isInit | std::true_type()];
////		auto _outEdges = p[outEdges | std::mem_fn(&G::outEdges)];
////		auto _isTree = std::true_type();
////
////		using V = typename GraphTraits<G>::VertexDescriptor;
////		using E = typename GraphTraits<G>::EdgeDescriptor;
////
////		auto actionVertex = [](V u) {};
////		auto actionEdge = [](E e, V u) {};
////
////		auto newParamPack = (
////				initializeVertex = p[initializeVertex | actionVertex],
////				startVertex = p[startVertex | actionVertex],
////				discoverVertex = p[discoverVertex | actionVertex],
////				examineEdge = p[examineEdge | actionEdge],
////				treeEdge = p[treeEdge | actionEdge],
////				treeEdgeReturn = p[treeEdgeReturn | actionEdge],
////				notTreeEdge = p[notTreeEdge | actionEdge],
////				finishEdge = p[finishEdge | actionEdge],
////				finishVertex = p[finishVertex | actionVertex],
////
////				isTree = _isTree,
////				outEdges = _outEdges,
////				isInit = _isInit,
////				enterVertex =  _enterVertex);
////
////		DepthFirstSearchPrivate::Impl <G, decltype(newParamPack)>
////		impl(g, newParamPack);
////
////
////	}
//
//
//
//	}
//
//
//template<typename G, typename ParamPack>
//void depthFirstSearch(const G &g, const ParamPack &p)
//{
//	DepthFirstSearchPrivate::dispatch(g, p);
//}
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//



} // namesace lz
#endif // DEPTH_FIRST_SEARCH_H
