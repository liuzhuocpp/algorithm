/*
 * disjoint_sets.cpp
 *
 *  Created on: 2015��11��29��
 *      Author: LZ
 */

#include <iostream>
#include <cstdio>
#include <cstring>
#include <tuple>
#include <vector>

#include "lz/disjoint_sets.h"


/*
 * 0: u same to parent
 * 1: u eat parent
 * 2: parent eat u
 */
// poj 1182
using namespace std;
using namespace lz;

int n = 10;
int main()
{
	vector<int> ls(n);
	auto pa = makeIteratorMap(ls.begin());
	DisjointSets<decltype(pa)> ds(pa);
	for(int i = 0; i < n; ++ i)
	ds.makeSet(i);

	ds.link(0, 1);

	ds.link(3, 6);
	ds.link(4, 6);
	ds.link(5, 6);
	ds.link(6, 6);

	for(int i = 0; i < n; ++ i)
	{
	    cout << i << ": " << ds.findSet(i) << endl;
	}



	return 0;
}
