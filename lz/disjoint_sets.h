/*
 * disjoint_sets.h
 *
 *  Created on: 2015��11��29��
 *      Author: LZ
 */
#ifndef LZ_DISJOINT_SETS_H_
#define LZ_DISJOINT_SETS_H_


#include "lz/map.h"
#include <algorithm>
#include <functional>
namespace lz {





/*
 * ParentMap: key type must be same to value type
 *
 * DisjointSetsImplement concept:
 * makeSet(u)
 * findSet(u)
 * link(x, y) //x, y must be the root of different set
 */
template<typename ParentMap>
class DisjointSetsImplement
{
	using VertexDescriptor = typename MapTraits<ParentMap>::KeyType;
public:
	static void makeSet(const ParentMap &parentMap, VertexDescriptor u)
	{
	    parentMap[u] = u;
	}

	template<typename BeforeParentChange>
	static VertexDescriptor findSet(const ParentMap &parentMap, VertexDescriptor u, BeforeParentChange beforeParentChange)
	{
		VertexDescriptor root = u;
		while(root != parentMap[root])
		{
			root = parentMap[root];
		}
		while(u != root)
		{
			VertexDescriptor oldParent = parentMap[u];
			beforeParentChange(u, root);
			parentMap[u] = root;
			u = oldParent;
		}
		return root;
	}

	static VertexDescriptor findSet(const ParentMap &parentMap, VertexDescriptor u)
	{
		return findSet(parentMap, u, [](VertexDescriptor current, VertexDescriptor newParent){});
	}


	// x, y must be a representative(root) of a set
	static void link(const ParentMap &parentMap, VertexDescriptor x, VertexDescriptor y)
	{
		parentMap[x] = y;
	}
//	static void unionSet(DisjointSets &sets, VertexDescriptor x, VertexDescriptor y)
//	{
//		link(findSet(x), findSet(y));
//	}
};

/*

DisjointSets concept:
    makeSet(u)
    findSet(u)
    link(x, y) //x, y must be the root of different set

 */

template<typename ParentMap>
struct DisjointSets
{
    using VertexDescriptor = typename MapTraits<ParentMap>::KeyType;
    DisjointSets() = default;
    DisjointSets(const ParentMap &parentMap):parentMap(parentMap){}

    template<typename ForwordIterator>
    DisjointSets(const ParentMap &parentMap, ForwordIterator begin, ForwordIterator end):
        parentMap(parentMap)
    {
        std::for_each(begin, end, [this](auto u){ makeSet(u); });
    }
    void makeSet(VertexDescriptor u)
    {
        Impl::makeSet(parentMap, u);
    }

    VertexDescriptor findSet(VertexDescriptor u)
    {
        return Impl::findSet(parentMap, u);
    }

    // x, y must be a representative(root) of a set
    void link(VertexDescriptor x, VertexDescriptor y)
    {
        Impl::link(parentMap, x, y);
    }
private:
    ParentMap parentMap;
    using Impl = DisjointSetsImplement<ParentMap>;
};


template<typename ParentMap, typename ForwordIterator>
DisjointSets<ParentMap> makeDisjointSets(const ParentMap& parentMap, ForwordIterator begin, ForwordIterator end )
{
    return DisjointSets<ParentMap>(parentMap, begin, end);
}

//template<typename ParentMap, typename ForwordIterator>
//auto calculateDisjointSets(const ParentMap& parentMap, ForwordIterator begin, ForwordIterator end )
//{
//
//    return std::bind(parentMap, begin, end);
//}







}







#endif /* LZ_DISJOINT_SETS_H_ */
