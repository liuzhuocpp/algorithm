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

struct DefaultChangeParent
{
	template<typename  V>
	void operator()(V u, V p) {}
};


template<typename ParentMap, typename ChangeParent = DefaultChangeParent>
class DisjointSets
{
	ParentMap p;
public:
	using ValueType = std::decay_t<typename MapTraits<ParentMap>::ValueType>;
	DisjointSets(ParentMap p):p(p) {}
	void makeSet(ValueType u)
	{
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
			ChangeParent()(u, new_parent);
			p[u] = new_parent;
		}
		return p[u];
	}
	void link(ValueType x, ValueType y)
	{
		ChangeParent()(x, y);
		p[x] = y;
	}
	void unionSet(ValueType x, ValueType y)
	{
		link(findSet(x), findSet(y));
	}

};




}







#endif /* LZ_DISJOINT_SETS_H_ */
