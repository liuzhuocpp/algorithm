/*
 * topological_sort.h
 *
 *  Created on: 2017年1月13日
 *      Author: LZ
 */

#ifndef LZ_TOPOLOGICAL_SORT_H_
#define LZ_TOPOLOGICAL_SORT_H_


#include <lz/parameter.h>
#include <lz/std_utility.h>
#include <lz/depth_first_search.h>
#include <lz/iterator_range.h>

namespace lz {

    namespace TopologicalSortKeywords {


    LZ_PARAMETER_KEYWORD(tag, startVertex)
//    LZ_PARAMETER_KEYWORD(tag, startVertex)


    } // namespace DagShortestPathsKeywords



template<typename G, typename DiscoverVertex>
void topologicalSort(const G &g, DiscoverVertex discoverVertex,
    typename GraphTraits<G>::VertexDescriptor startVertex = GraphTraits<G>::nullVertex())
{

    depthFirstSearch(g, (
            DepthFirstSearchKeywords::enterVertex = startVertex,
            DepthFirstSearchKeywords::finishVertex = [&](typename GraphTraits<G>::VertexDescriptor u)
                {
                    discoverVertex(u);
                }));

}

} // namespace lz

#endif /* LZ_TOPOLOGICAL_SORT_H_ */
