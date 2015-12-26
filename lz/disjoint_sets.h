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
	template<typename  Element>
	void operator()(Element u, Element p) {}
};


template<typename ParentMap, typename ChangeParent = DefaultChangeParent>
class DisjointSets
{
	ParentMap p;
public:
	using Element = typename MapTraits<ParentMap>::ValueType;
	DisjointSets(ParentMap p):p(p) {}
	void makeSet(Element u)
	{
		p[u] = u;
	}
	Element findSet(Element u)
	{
		if(p[u] != u)
		{
			Element new_parent = findSet(p[u]);
			ChangeParent()(u, new_parent);
			p[u] = new_parent;
		}
		return p[u];
	}
	void link(Element x, Element y)
	{
		p[x] = y;
	}
	void unionSet(Element x, Element y)
	{
		link(findSet(x), findSet(y));
	}

};




}







#endif /* LZ_DISJOINT_SETS_H_ */
