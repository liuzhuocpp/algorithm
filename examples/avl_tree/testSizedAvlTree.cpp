/*
 * main.cpp
 *
 *  Created on: 2016��5��16��
 *      Author: LZ
 */



//#include <iostream>
//#include <cstdio>
#include <bits/stdc++.h>

#include <lz/binary_search_tree.h>
#include <lz/avl_tree.h>
#include <lz/sized_bst.h>
using namespace std;
using namespace lz;



struct AvlTreeNode: lz::BstNodeFacade<AvlTreeNode, int>, lz::AvlTreeNodeBase, lz::SizedBstNodeBase
{
	AvlTreeNode(int a = 0):
		BstNodeFacade<AvlTreeNode, int>(a){}
};

struct MyAvl: lz::BinarySearchTree<AvlTreeNode>, lz::AvlTreeBase<AvlTreeNode>, lz::SizedBstBase<AvlTreeNode>
{
};

int main()
{

	MyAvl avl;
	using I = AvlTreeImplement< SizedBstImplement<BstImplement<MyAvl>  > >;
	I::insert(avl, new AvlTreeNode(10));
	I::insert(avl, new AvlTreeNode(20));
//	I::bfs(avl);

	I::insert(avl, new AvlTreeNode(220));


	I::insert(avl, new AvlTreeNode(333));
	I::insert(avl, new AvlTreeNode(300));

	I::insert(avl, new AvlTreeNode(5));
	I::insert(avl, new AvlTreeNode(240));

//	I::erase(avl, I::find(avl, 300) );
	I::bfs(avl);

	cout << string(140, '-') << endl;
	I::erase(avl, I::find(avl, 300) );
	I::bfs(avl);

//
//
	cout << "Total:" << endl;
	cout << avl.size(avl.root()) << endl;
	cout << string(100, '+') << endl;
	cout << string(100, '+') << endl;
	cout << avl.size(avl.root()) << endl;
	cout << avl.size(avl.rightChild(avl.root())) << endl;

	cout << string(100, '!') << endl;
	cout << string(100, '!') << endl;

	for(int i = 0; i < avl.size(avl.root()); ++ i)
	{
		cout << avl.key( I::at(avl, i) ) << endl;
	}

	return 0;
}




















