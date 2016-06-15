/*
 * main.cpp
 *
 *  Created on: 2016年6月15日
 *      Author: LZ
 */

//#include <iostream>
//#include <cstdio>
#include <bits/stdc++.h>

#include <lz/binary_search_tree.h>


using namespace std;
using namespace lz;

struct BstNode: lz::BstNodeFacade<BstNode, int >
{
	BstNode(const int &key = KeyType()): lz::BstNodeFacade<BstNode, int >(key){}
};
struct Bst:public lz::BinarySearchTree<BstNode>
{

};

int main()
{
	Bst bst;
	using I = BstImplement<Bst>;
	I::insert(bst, new BstNode(100));
	I::insert(bst, new BstNode(300));
	I::insert(bst, new BstNode(200));
	I::insert(bst, new BstNode(312));

	I::bfs(bst);




	return 0;
}




















