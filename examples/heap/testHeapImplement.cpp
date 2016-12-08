/*
 * main.cpp
 *
 *  Created on: 2016年12月3日
 *      Author: LZ
 */



#include "lz/heap.h"
#include <bits/stdc++.h>

using namespace lz;
using namespace std;

int n = 100;
vector<int> a(n, 0);
vector<int> id(n, 0);
struct BeforeKeyChange
{
	void operator()(vector<int>::iterator cnt, const int & newKey) const
	{
		id[newKey] = cnt - a.begin();
	}
}beforeKeyChange;
int main()
{
	cout << "FF" << endl;
	std::less<int> _less;

	using HeapImpl = HeapImplement<vector<int>::iterator, std::less<int>, BeforeKeyChange>;

	for(int i = 0; i < n; ++ i) a[i] = i;
	random_shuffle(a.begin(), a.end());
	for(int i = 0; i < n; ++ i) id[a[i]] = i;


	for(int i = 0; i < n; ++ i)
	{
		cout << a[i] << " : " << i << " " << id[a[i]] << endl;
	}
	HeapImpl::make(a.begin(), a.end(), _less, beforeKeyChange);
	cout << string(100, '-') << endl;
	for(int i = 0; i < n; ++ i)
	{
		cout << a[i] << " : " << i << " " << id[a[i]] << endl;
	}


	return 0;
}
