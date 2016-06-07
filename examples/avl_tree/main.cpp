/*
 * main.cpp
 *
 *  Created on: 2016��5��16��
 *      Author: LZ
 */



//#include <iostream>
//#include <cstdio>
#include <bits/stdc++.h>

#include <lz/avl_tree.h>
using namespace std;
using namespace lz;

struct AvlTreeNode: public AvlTreeNodeFacade< BstNodeFacade<AvlTreeNode, int> >
{

	using Base = AvlTreeNodeFacade< BstNodeFacade<AvlTreeNode, int> >;

//	using Base::leftChild;
//	using Base::rightChild;
//	using Base::parent;
//	using Base::key;
//	using Base::height;
//


	AvlTreeNode(int a = 0):Base(a)
	{

	}
};




int main()
{

//	lz::AvlTree<AvlTreeNode> avl;

	using AvlTree = lz::AvlTree<lz::BinerySearchTree<AvlTreeNode>>;
	AvlTree avl;

	using I = BinerySearchTreeImplement< AvlTree > ;


	AvlTreeImplement<I >::insert(avl, new AvlTreeNode(10));
	AvlTreeImplement<I >::insert(avl, new AvlTreeNode(20));
	AvlTreeImplement<I >::insert(avl, new AvlTreeNode(220));
	AvlTreeImplement<I >::insert(avl, new AvlTreeNode(333));
	AvlTreeImplement<I>::insert(avl, new AvlTreeNode(300));


	AvlTreeImplement<I>::insert(avl, new AvlTreeNode(200	));
	AvlTreeImplement<I >::insert(avl, new AvlTreeNode(100	));
	AvlTreeImplement<I >::insert(avl, new AvlTreeNode(90	));
	AvlTreeImplement<I >::insert(avl, new AvlTreeNode(80	));
	AvlTreeImplement<I >::insert(avl, new AvlTreeNode(85	));



	AvlTreeImplement<I >::bfs(avl);






	return 0;
}




















