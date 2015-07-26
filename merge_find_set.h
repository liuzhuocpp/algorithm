/*
 * merge_find_set.h
 *
 *  Created on: 2015Äê7ÔÂ26ÈÕ
 *      Author: LZ
 */

#ifndef MERGE_FIND_SET_H_
#define MERGE_FIND_SET_H_

#include <vector>
namespace lz {

using std::vector;


class MergeFindSet
{s
	vector<int> p;
public:
	MergeFindSet(int n = 0):p(n)
	{

	}
	bool merge(int a, int b)
	{
		int ida = find(a), idb = find(b);
		if(ida == idb) return 0;
		p[ida] = idb;
		return 1;
	}
	int find(int x)
	{
		if(p[x] != x) p[x] = find(p[x]);
		return p[x];
	}

};



} // namespace lz






#endif /* MERGE_FIND_SET_H_ */
