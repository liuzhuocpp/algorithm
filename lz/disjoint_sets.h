/*
 * disjoint_sets.h
 *
 *  Created on: 2015Äê11ÔÂ29ÈÕ
 *      Author: LZ
 */

#ifndef LZ_DISJOINT_SETS_H_
#define LZ_DISJOINT_SETS_H_

#include "lz/map.h"

namespace lz {



struct DisjointSetsParams
{
	template<typename V>
	void initializeParent(V u) {}

	template<typename V>
	void changeParent(V u, V new_parent) {}

};

template<typename ParentMap, typename Params = DisjointSetsParams>
class DisjointSets
{
	ParentMap p;
	Params params;
public:
	using ValueType = typename MapTraits<ParentMap>::ValueType;
	DisjointSets(ParentMap p, Params &params):p(p), params(params) {}
	void makeSet(ValueType u)
	{
		params.initializeParent(u);
		p[u] = u;
	}
	template<typename ElementIterator>
	void makeSet(ElementIterator first, ElementIterator last)
	{
		for(;first != last; first ++)
		{
			makeSet(*first);
		}
	}
	ValueType findSet(ValueType u)
	{
		if(p[u] != u)
		{
			ValueType new_parent = findSet(p[u]);
			params.changeParent(u, new_parent);
			p[u] = new_parent;
		}
		return p[u];
	}
	void link(ValueType x, ValueType y)
	{
		params.changeParent(x, y);
		p[x] = y;
	}
	void unionSet(ValueType x, ValueType y)
	{
		link(findSet(x), findSet(y));
	}

};




}







#endif /* LZ_DISJOINT_SETS_H_ */
