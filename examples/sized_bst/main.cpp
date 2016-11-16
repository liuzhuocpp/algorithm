
/*
 * main.cpp
 *
 *  Created on: 2016年11月16日
 *      Author: LZ
 */
#include <iostream>
#include <bits/stdc++.h>

#include <lz/binary_search_tree.h>
#include <lz/sized_bst.h>
using namespace std;
using namespace lz;
struct SizedBstNode: public lz::BstNodeFacade<SizedBstNode, int>, SizedBstNodeBase
{
	SizedBstNode(const int &key = KeyType()): lz::BstNodeFacade<SizedBstNode, int >(key){}
};

struct SizedBst : public lz::BinarySearchTree<SizedBstNode>, SizedBstBase<SizedBstNode>
{
	void out(SizedBst::NodeDescriptor u) const
	{
		cout << " key:" << key(u) << " size:" << size(u) << endl;
	}
};

int main()
{

	using I = SizedBstImplement<BstImplement<SizedBst> >;


	SizedBst bst;
	I::insert(bst, new SizedBstNode(100));
	I::insert(bst, new SizedBstNode(300));
	I::insert(bst, new SizedBstNode(200));
	I::insert(bst, new SizedBstNode(312));

//	I::erase(bst, bst.root());

	I::bfs(bst);

	return 0;
}








