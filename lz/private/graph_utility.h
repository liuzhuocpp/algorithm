/*
 * graph_utility.h
 *
 *  Created on: 2015Äê11ÔÂ23ÈÕ
 *      Author: LZ
 */

#ifndef LZ_PRIVATE_GRAPH_UTILITY_H_
#define LZ_PRIVATE_GRAPH_UTILITY_H_

#include <type_traits>

namespace lz
{
	namespace Private {

	template<typename Params>
	using ParamVertexIndexMap = typename std::result_of< decltype(&Params::vertexIndexMap)(Params)>::type;

	template<typename G>
	using DefaultVertexIndexMap = decltype(G().vertexPropertyMap(VertexIndexTag()));


//	using VertexIndexMap = typename std::conditional<std::is_same<ParamVertexIndexMap, ParamNotFound>::value,
//													 DefaultVertexIndexMap,
//													 ParamVertexIndexMap>::type;


	template<typename ParamType, typename DefaultType>
	using ChooseType =  typename std::conditional<std::is_same<ParamType, ParamNotFound>::value,
																DefaultType,
																ParamType>::type;










	} // namespace Private
} // namespace lz







#endif /* LZ_PRIVATE_GRAPH_UTILITY_H_ */
