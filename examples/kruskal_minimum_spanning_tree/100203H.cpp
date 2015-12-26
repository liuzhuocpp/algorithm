/*
 * 100203H.cpp
 *
 *  Created on: 2015Äê12ÔÂ23ÈÕ
 *      Author: LZ
 *
 *
 *
 * http://codeforces.com/problemset/gymProblem/100203/H
 */

#include <bits/stdc++.h>
#include "lz/kruskal_minimum_spanning_tree.h"
#include "lz/adjacency_list.h"

#pragma GCC diagnostic error "-std=c++14"
using namespace std;
using namespace lz;

using EP = Property<EdgeWeightTag, double>;

using Graph = AdjacencyList<UndirectedGraphTag, NoProperty, EP>;
const int N = 750 + 9;

vector<pair<int, int> > p;
double dis(int i, int j)
{
	double a = p[i].first - p[j].first;
	double b = p[i].second - p[j].second;
	return sqrt(a * a + b * b);
}
int main()
{

	Graph g;


	int n;
	cin >> n;
	p.clear();
	for(int i = 0; i < n; ++ i)
	{
		int x, y; cin >> x >> y;
		p.push_back(make_pair(x, y));
		g.addVertex();
	}

	for(int i = 0; i < n; ++ i)
	{
		for(int j = i; j < n; ++ j)
		{
			double x = dis(i, j);
			g.addEdge(i, j, EP(x )  );
		}
	}
	cin >> n;
	while(n --)
	{
		int x, y; cin >> x >> y;
		x -- ,y --;
		g.addEdge(x, y, EP(0.0)  );
	}



	kruskalMinimumSpanningTree(g,
		[&](auto e)
		{
			auto a = g.source(e);
			auto b = g.target(e);
			a ++ ; b++;
			if(g.edgeProperties(e)[EdgeWeightTag()] > 0)
			{

				cout << a << " " << b << "\n";
			}
			else
			{
//				cout << "GG" << endl;
			}
		}
	);







	//	while(m --)
//	{
//		int x, y, _g, _s;
//		cin >> x >> y >> _g >> _s;
//		--x;
//		--y;
//		LL t = G * _g + S * _s;
////		cout << "T " << t << endl;
//		g.addEdge(x, y, EP(make_tuple(t, _g, _s)));
//	}
//
//	auto ewMap = g.edgePropertyMap(EdgeWeightTag());
//	auto gold = g.edgePropertyMap(GoldTag());
//	auto silver = g.edgePropertyMap(SilverTag());
//	LL sum1 = 0, sum2 = 0;
//	int cot = 0;
//	kruskalMinimumSpanningTree(g,
//		[&](auto e)
//		{
//			sum1 = max(sum1, gold[e]);
//			sum2 = max(sum2, silver[e]);
//			cot ++;
//		}
//	);
//
//	if(cot == n - 1)
//	{
//
//		cout << (sum1 * G + sum2 * S) << "\n";
//	}
//	else
//	{
//		cout << "-1\n";
//	}
//











	return 0;
}
