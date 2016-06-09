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

//struct AvlTreeNode: public AvlTreeNodeFacade< BstNodeFacade<AvlTreeNode, int> >
//{
//
//	using Base = AvlTreeNodeFacade< BstNodeFacade<AvlTreeNode, int> >;
//
////	using Base::leftChild;
////	using Base::rightChild;
////	using Base::parent;
////	using Base::key;
////	using Base::height;
////
//
//
//	AvlTreeNode(int a = 0):Base(a)
//	{
//
//	}
//};
//

struct AvlTreeNode: lz::AvlTreeNodeFacade<AvlTreeNode, int>, lz::SizedBstNodeBase
{
	AvlTreeNode(int a = 0):
		lz::AvlTreeNodeFacade<AvlTreeNode, int>(a){}
};

struct MyAvl: lz::AvlTree<AvlTreeNode>, lz::SizedBstBase<AvlTreeNode>
{
};

int main()
{

	MyAvl avl;
	using I = AvlTreeImplement<MyAvl, SizedBstVisitor<MyAvl>>;
	I::insert(avl, new AvlTreeNode(10));
	I::insert(avl, new AvlTreeNode(20));
	I::insert(avl, new AvlTreeNode(220));
	I::insert(avl, new AvlTreeNode(333));
	I::insert(avl, new AvlTreeNode(300));

	I::insert(avl, new AvlTreeNode(5));
	I::insert(avl, new AvlTreeNode(240));

	I::erase(avl, I::find(avl, 300) );
	I::bfs(avl);


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
		cout << avl.key( sizedBstAt(avl, i) ) << endl;
	}




	//	using Bst = BinerySearchTree<BstNode>;
	//
	//	Bst bst;
	//	BstImplement<Bst>::insert(bst, new BstNode(111));
	//	BstImplement<Bst>::insert(bst, new BstNode(211));
	//	BstImplement<Bst>::insert(bst, new BstNode(311));
	//
	//	BstImplement<Bst>::bfs(bst);





	return 0;
}




















