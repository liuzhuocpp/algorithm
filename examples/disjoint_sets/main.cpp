/*
 * disjoint_sets.cpp
 *
 *  Created on: 2015Äê11ÔÂ29ÈÕ
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

int main()
{
	vector<int> ls(10);
	auto pa = makeIteratorMap(ls.begin());
	DisjointSets<decltype(pa)> ds(pa);


	return 0;
}
