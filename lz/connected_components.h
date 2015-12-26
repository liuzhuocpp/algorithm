/*
 * connected_components.h
 *
 *  Created on: 2015Äê12ÔÂ16ÈÕ
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
	LZ_PARAMETER_KEYWORD(tag, colorMap)
	LZ_PARAMETER_KEYWORD(tag, white)
	LZ_PARAMETER_KEYWORD(tag, black)

	}




template<typename G, typename ComponentMap, typename ParamPack = EmptyParamPack>
typename MapTraits<ComponentMap>::ValueType

connectedComponents(const G &g, ComponentMap compMap, const ParamPack &p = EmptyParamPack())
{
	namespace k = ConnectedComponentsKeywords;
	auto i_map = p[k::vertexIndexMap | g.vertexPropertyMap(VertexIndexTag())];

	auto n = g.vertexNumber();
	using ColorValue = typename ColorTraits<>::Type;

	auto _colorMap = p[k::colorMap || std::bind(makeVertexIndexComposeMap
						<ColorValue, decltype(i_map), decltype(n) >,  i_map, n) ];

	using namespace DepthFirstSearchKeywords;
	typename MapTraits<ComponentMap>::ValueType compNumber = 0;
	using Vertex = typename GraphTraits<G>::VertexDescriptor;

	lz::depthFirstSearch(g,(
			colorMap = _colorMap
		   ,white = p[k::white | ColorTraits<>::white]
		   ,black = p[k::black | ColorTraits<>::black]
		   ,startVertex = [&](Vertex u) { ++ compNumber; }
		   ,discoverVertex = [&](Vertex u) { compMap[u] = compNumber - 1; }


	));

	return compNumber;

	return 0;

}







} // namespace lz



















#endif /* LZ_CONNECTED_COMPONENTS_H_ */
