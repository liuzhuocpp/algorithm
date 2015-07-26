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


struct NoSetParent
{
	void operator()(int a, int p) {}
};

template<typename SetParent = NoSetParent>
class MergeFindSet
{
	vector<int> p;
public:
	explicit MergeFindSet(int n = 0): p(n)
	{
		for(int i = 0; i < n; ++ i) p[i] = i;
	}
	void makeSet(int n)
	{
		p.resize(n);
		for(int i = 0; i < n; ++ i) p[i] = i;
	}

	int parent(int a) const { return p[a]; }
	void setParent(int a, int pa) { SetParent()(a, pa);  p[a] = pa; }
	bool merge(int a, int b)
	{
		int ida = find(a), idb = find(b);
		if(ida == idb) return 0;
		SetParent()(ida, idb);
		p[ida] = idb;
		return 1;
	}
	int find(int x)
	{
		if(p[x] != x)
		{
			int t = find(p[x]);
			SetParent()(x, t);
			p[x] = t;
		}
		return p[x];
	}


};



} // namespace lz






#endif /* MERGE_FIND_SET_H_ */
