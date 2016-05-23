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

struct Node
{
	Node *L, *R, *P;
	int _key;

	using KeyType = int;
	using KeyCompare = std::less<int>;

	Node*& parent()
	{
		return P;
	}

	Node*& leftChild()
	{
		return L;
	}
	Node*& rightChild()
	{
		return R;
	}
	int& key()
	{
		return _key;
	}
	Node(int key):_key(key), L(nullptr), R(nullptr), P(nullptr){}
};

int main()
{

	TestBinerySearchTree tbst;

	BinerySearchTreeImplement<TestBinerySearchTree>::insert(tbst, tbst.createVertex(11));
	BinerySearchTreeImplement<TestBinerySearchTree>::insert(tbst, tbst.createVertex(2));
	BinerySearchTreeImplement<TestBinerySearchTree>::insert(tbst, tbst.createVertex(33));
	BinerySearchTreeImplement<TestBinerySearchTree>::insert(tbst, tbst.createVertex(1));
//	BinerySearchTreeImplement<TestBinerySearchTree>::insert(tbst, tbst.createVertex(2));
	BinerySearchTreeImplement<TestBinerySearchTree>::insert(tbst, tbst.createVertex(44));


	BinerySearchTreeImplement<TestBinerySearchTree>::insert(tbst, tbst.createVertex(7));
	BinerySearchTreeImplement<TestBinerySearchTree>::insert(tbst, tbst.createVertex(8));




	cout << string(100, '-') << endl;
	BinerySearchTreeImplement<TestBinerySearchTree>::bfs(tbst);

	BinerySearchTreeImplement<TestBinerySearchTree>::remove(tbst, 11);
	BinerySearchTreeImplement<TestBinerySearchTree>::remove(tbst, 8);
	BinerySearchTreeImplement<TestBinerySearchTree>::remove(tbst, 7);
	BinerySearchTreeImplement<TestBinerySearchTree>::remove(tbst, 2);
	BinerySearchTreeImplement<TestBinerySearchTree>::remove(tbst, 1);
	BinerySearchTreeImplement<TestBinerySearchTree>::remove(tbst, 33);
	BinerySearchTreeImplement<TestBinerySearchTree>::remove(tbst, 44);

	cout << string(100, '-') << endl;
	BinerySearchTreeImplement<TestBinerySearchTree>::bfs(tbst);

//	cout << tbst.root()->left->key << " " << tbst.root()->right->key << endl;
//	tbst.root() = new

	return 0;

//	Node* root = new Node(11);
//
//	bstInsert(root, new Node(22));
//	bstInsert(root, new Node(33));
//	bstInsert(root, new Node(44));
//	bstInsert(root, new Node(1));
//	bstInsert(root, new Node(22));
//
//	cout <<"bfs------" << endl;
//	bstBfs(root);
//
//	cout <<"search------" << endl;
//	cout << bstSearch(root, 22)->key() << endl;
//
//	cout <<"nextNode------" << endl;
//	Node* test = root;
//	while(test != nullptr)
//	{
//		cout << test->key() << endl;
//		test = bstNextNode(test);
//	}
//
//





	return 0;
}




















