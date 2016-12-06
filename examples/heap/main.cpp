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
	void operator()(vector<int>::iterator cnt, vector<int>::iterator _new)
	{
		id[*_new] = cnt - a.begin();
	}
};
int main()
{
	cout << "FF" << endl;

	using HeapImpl = HeapImplement<vector<int>::iterator, BeforeKeyChange>;

	for(int i = 0; i < n; ++ i) a[i] = i;
	random_shuffle(a.begin(), a.end());
	for(int i = 0; i < n; ++ i) id[a[i]] = i;

	for(int i = 0; i < n; ++ i)
	{
		cout << a[i] << " : " << i << " " << id[a[i]] << endl;
	}
//	return 0;
	HeapImpl::make(a.begin(), a.end());
	cout << string(100, '-') << endl;
	for(int i = 0; i < n; ++ i)
	{
		cout << a[i] << " : " << i << " " << id[a[i]] << endl;
	}

	return 0;


//	for(int )

//	for(int i = 0; i < 100; ++ i)
//	{
//		HeapImpl::push(a.begin(), a.begin() + i + 1);
//	}
//	for(int i = 0; i < 100; ++ i)
//	{
//		a.push_back(i*10);
//	}
//	cout << a.size
//	random_shuffle(a.begin(), a.end());

//	HeapImpl::make(a.begin(), a.end());

	for(int i = a.size(); i > 0; --i)
	{
		cout << a[0] << endl;
		HeapImpl::pop(a.begin(), a.begin() + i);
	}







	return 0;
}
