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
 * DisjointSets concept:
 * parent(u): return the reference of the parent of u
 * VertexDescriptor: the type of the vertex descriptor of DisjointSets
 */
template<typename ParentMap>
struct DisjointSets
{
	using VertexDescriptor = typename MapTraits<ParentMap>::ValueType;
	VertexDescriptor& parent(VertexDescriptor u) { return parentMap[u]; }
	const VertexDescriptor& parent(VertexDescriptor u) const { return parentMap[u]; }

	DisjointSets(ParentMap parentMap): parentMap(parentMap){}
private:
	ParentMap parentMap;
};

/*
 * DisjointSets must require DisjointSets concept
 * DisjointSetsImplement concept:
 * makeSet(u)
 * findSet(u)
 * link(x, y) //x, y must be the root of different set
 */
template<typename DisjointSets>
class DisjointSetsImplement
{
	using VertexDescriptor = typename DisjointSets::VertexDescriptor;
public:
	static void makeSet(DisjointSets &sets, VertexDescriptor u)
	{
		sets.parent(u) = u;
	}

	template<typename BeforeParentChange>
	static VertexDescriptor findSet(DisjointSets &sets, VertexDescriptor u, BeforeParentChange beforeParentChange)
	{
		VertexDescriptor root = u;
		while(root != sets.parent(root))
		{
			root = sets.parent(root);
		}
		while(u != root)
		{
			VertexDescriptor parent = sets.parent(u);
			beforeParentChange(u, root);
			sets.parent(u) = root;
			u = parent;
		}
		return root;
	}

	static VertexDescriptor findSet(DisjointSets &sets, VertexDescriptor u)
	{
		return findSet(sets, u, [](VertexDescriptor current, VertexDescriptor newParent){});
	}


	// x, y must be a representative(root) of a set
	static void link(DisjointSets &sets, VertexDescriptor x, VertexDescriptor y)
	{
		sets.parent(x ) = y;
	}
//	static void unionSet(DisjointSets &sets, VertexDescriptor x, VertexDescriptor y)
//	{
//		link(findSet(x), findSet(y));
//	}
};









}







#endif /* LZ_DISJOINT_SETS_H_ */
