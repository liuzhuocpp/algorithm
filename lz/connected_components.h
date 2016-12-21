/*
 * connected_components.h
 *
 *  Created on: 2015.12.16
 *      Author: LZ
 */


#ifndef LZ_CONNECTED_COMPONENTS_H_
#define LZ_CONNECTED_COMPONENTS_H_

#include "lz/parameter.h"
#include "lz/depth_first_search.h"
#include "lz/map.h"
#include "lz/graph_utility.h"
#include "lz/utility.h"

namespace lz{


namespace ConnectedComponentsKeywords{

LZ_PARAMETER_KEYWORD(tag, vertexIndexMap)
LZ_PARAMETER_KEYWORD(tag, marker)

}

template<typename G, typename ComponentMap, typename ParamPack = EmptyParamPack>
typename MapTraits<ComponentMap>::ValueType
connectedComponents(const G &g, ComponentMap compMap, const ParamPack &params = EmptyParamPack())
{
	namespace Keys = ConnectedComponentsKeywords;

	auto indexMap = params[Keys::vertexIndexMap | vertexPropertyMap(g, vertexIndexTag)];
	auto n = verticesNumber(g);

	using Vertex = typename GraphTraits<G>::VertexDescriptor;
	auto marker = params[Keys::marker || [&](){
	    return IndexMarker<decltype(indexMap)>(indexMap, n);
	}];

	typename MapTraits<ComponentMap>::ValueType compNumber = 0;
	depthFirstSearch(g, (
	        DepthFirstSearchKeywords::marker = marker,

	        DepthFirstSearchKeywords::startVertex = [&](Vertex u) { ++ compNumber; },
	        DepthFirstSearchKeywords::discoverVertex = [&](Vertex u) { compMap[u] = compNumber - 1; }
	));
	return compNumber;
}







} // namespace lz



















#endif /* LZ_CONNECTED_COMPONENTS_H_ */
