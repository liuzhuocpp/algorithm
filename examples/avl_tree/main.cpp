/*
 * main.cpp
 *
 *  Created on: 2016Äê5ÔÂ16ÈÕ
 *      Author: LZ
 */



//#include <iostream>
//#include <cstdio>
#include <bits/stdc++.h>

#include <lz/avl_tree.h>
using namespace std;
using namespace lz;

struct AvlTreeNode: public AvlTreeNodeFacade<AvlTreeNode, int>
{

	using Base = AvlTreeNodeFacade<AvlTreeNode, int>;

//	using Base::leftChild;
//	using Base::rightChild;
//	using Base::parent;
//	using Base::key;
//	using Base::height;
//
//	void f()
//	{
//		leftChild = nullptr;
//	}


	AvlTreeNode(int a = 0):Base(a)
	{

	}
};




int main()
{

	lz::AvlTree<AvlTreeNode> avl;

	AvlTreeNode *p = new AvlTreeNode(20);


//	cout << "TEST: " << (p->parent == nullptr) << endl;

	cout << "HHA" << endl;
	AvlTreeImplement<lz::AvlTree<AvlTreeNode> >::insert(avl, new AvlTreeNode(10));
	AvlTreeImplement<lz::AvlTree<AvlTreeNode> >::insert(avl, new AvlTreeNode(20));
	AvlTreeImplement<lz::AvlTree<AvlTreeNode> >::insert(avl, new AvlTreeNode(220));
	AvlTreeImplement<lz::AvlTree<AvlTreeNode> >::insert(avl, new AvlTreeNode(333));
	AvlTreeImplement<lz::AvlTree<AvlTreeNode> >::insert(avl, new AvlTreeNode(300));


	AvlTreeImplement<lz::AvlTree<AvlTreeNode> >::insert(avl, new AvlTreeNode(200	));
	AvlTreeImplement<lz::AvlTree<AvlTreeNode> >::insert(avl, new AvlTreeNode(100	));
	AvlTreeImplement<lz::AvlTree<AvlTreeNode> >::insert(avl, new AvlTreeNode(90	));
	AvlTreeImplement<lz::AvlTree<AvlTreeNode> >::insert(avl, new AvlTreeNode(80	));
	AvlTreeImplement<lz::AvlTree<AvlTreeNode> >::insert(avl, new AvlTreeNode(85	));




//	BinerySearchTreeImplement<lz::AvlTree<AvlTreeNode> >::insert(avl, new AvlTreeNode(10));
//	BinerySearchTreeImplement<lz::AvlTree<AvlTreeNode> >::insert(avl, new AvlTreeNode(20));
//	BinerySearchTreeImplement<lz::AvlTree<AvlTreeNode> >::insert(avl, new AvlTreeNode(220));

	cout << "HHA------------------" << endl;


	BinerySearchTreeImplement<lz::AvlTree<AvlTreeNode>>::bfs(avl);









	return 0;
}




















