/*
 * disjoint_sets.h
 *
 *  Created on: 2015��11��29��
 *      Author: LZ
 */
#ifndef LZ_DISJOINT_SETS_H_
#define LZ_DISJOINT_SETS_H_


#include "lz/map.h"

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
    DisjointSets(){}
    DisjointSets(const ParentMap &parentMap):parentMap(parentMap){}
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











}







#endif /* LZ_DISJOINT_SETS_H_ */
