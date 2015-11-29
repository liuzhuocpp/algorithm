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
//using namespace std;
using namespace lz;


const int N = 50000 + 9;

int p[N], rank[N];
using V = int;
struct P:public DisjointSetsParams
{
	void initializeParent(V u)
	{
		rank[u] = 0;
	}
	void changeParent(V u, V new_p)
	{
		V old = p[u];
		if(u == old) return;

		if(rank[u] == 0) rank[u] = rank[old];
		else if(rank[u] == 1)
		{
			if(rank[old] == 0) rank[u] = 1;
			else if(rank[old] == 1) rank[u] = -1;
			else if(rank[old] == -1) rank[u] = 0;
		}
		else if(rank[u] == -1)
		{
			if(rank[old] == 0) rank[u] = -1;
			else if(rank[old] == 1) rank[u] = 0;
			else rank[u] = 1;
		}

	}
};

int main()
{
	P params;
	DisjointSets<IteratorMap<int*> , P> dis_set(makeIteratorMap(p), params);
	int n, k;
	std::scanf("%d%d", &n, &k);
	for(int i = 1; i <= n; ++ i)
	{
		dis_set.makeSet(i);
	}
	int ans = 0;
	while(k --)
	{
		int c, x, y;
		scanf("%d%d%d", &c, &x, &y);
		if(x > n || y > n || (c == 2 && x == y))
		{
			ans ++; continue;
		}


		int rx = dis_set.findSet(x),
			ry = dis_set.findSet(y);
		if(c == 1)
		{
			if(rx != ry)
			{
				dis_set.link(rx, ry);
				rank[rx] = 0;
			}
			else
			{
				if(!(rank[x] == rank[y])) ans ++;
			}
		}
		else
		{
			if(rx == ry)
			{
				if(x != rx && y != rx)
				{
					if(
					   (rank[x] == 1 && rank[y] == 0) ||
					   (rank[x] == 0 && rank[y] == -1) ||
					   (rank[x] == -1 && rank[y] == 1) );
					else ans ++;
				}
				else if(x == rx)
				{
					if(rank[y] == -1);
					else ans ++;
				}
				else
				{
					if(rank[x] == 1);
					else ans ++;
				}
			}
			else
			{
				dis_set.link(rx, ry);
				rank[rx] = 1;
			}


		}


	}
	std::printf("%d\n", ans);









	return 0;
}


























