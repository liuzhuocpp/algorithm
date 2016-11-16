/*
 * avl_tree.h
 *
 *  Created on: 2016��5��16��
 *      Author: LZ
 */

#ifndef LZ_AVL_TREE_H_
#define LZ_AVL_TREE_H_

#include <queue>

//#include <lz/binary_search_tree.h>
namespace lz{

//for DEBUG
using namespace std;

/**
 * AvlTreeNode concept requires:
 * BstNode concept
 * containing a type named HeightType
 * containing a field named height
 */
struct AvlTreeNodeBase
{
	using HeightType = int;
	HeightType height = 1;
};



/**
 * _Node must require AvlTreeNode concept
 *
 * AvlTree concept requires:
 * BinerySearchTree concept
 * containing a type named HeightType
 * containg one or some function named height(NodeDescriptor) to get or set the height value of a node
 */
template<typename _Node >
struct AvlTreeBase
{
public:
	using HeightType = typename _Node::HeightType;
	HeightType& height(_Node* u) { return u->height; }
	const HeightType& height(_Node* u) const { return u->height; }
};



template<typename BstImplement>
struct AvlTreeImplement : BstImplement
{
	using AvlTree = typename BstImplement::BinarySearchTree;
private:
//	using Base = BstImplement<AvlTree, Visitors>;

	using KeyType = typename AvlTree::KeyType;
	using KeyCompare = typename AvlTree::KeyCompare;
	using NodeDescriptor = typename AvlTree::NodeDescriptor;
	using HeightType = typename AvlTree::HeightType;

	static constexpr NodeDescriptor nullNode = AvlTree::nullNode();

	static HeightType height(const AvlTree &avl, NodeDescriptor u)
	{
		if(u != nullNode) return avl.height(u);
		else return 0;
	}

	static HeightType diff(const AvlTree &avl, NodeDescriptor u)
	{
		return height(avl, avl.leftChild(u)) - height(avl, avl.rightChild(u));
	}

	static void updateHeight(AvlTree &avl, NodeDescriptor u)
	{
		avl.height(u) = 1 + std::max(height(avl, avl.leftChild(u)), height(avl, avl.rightChild(u)));
	}

	static void rebalance(AvlTree &avl, NodeDescriptor u)
	{
		for(NodeDescriptor pu; u != nullNode; u = pu)
		{
			pu = avl.parent(u);
			updateHeight(avl, u);

			if(diff(avl, u) == 2)
			{
				NodeDescriptor l = avl.leftChild(u);
				if(diff(avl, l) == -1)
				{
					NodeDescriptor lr = avl.rightChild(l);
					leftRotate(avl, l);
				}
				rightRotate(avl, u);
				break;
			}
			else if(diff(avl, u) == -2)
			{
				NodeDescriptor r = avl.rightChild(u);
				if(diff(avl, r) == 1)
				{
					NodeDescriptor rl = avl.leftChild(r);
					rightRotate(avl, r);
				}
				leftRotate(avl, u);
				break;
			}
		}
	}
public:
	static void leftRotate(AvlTree &avl, NodeDescriptor x)
	{
//		cout << "LEFT RO" << endl;
		NodeDescriptor y = avl.rightChild(x);
//		cout << "ENTER left" << endl;
		BstImplement::leftRotate(avl, x);
//		cout << "ENTER left end" << endl;
		updateHeight(avl, x);
		updateHeight(avl, y);
	}
	static void rightRotate(AvlTree &avl, NodeDescriptor x)
	{
		NodeDescriptor y = avl.leftChild(x);
		BstImplement::rightRotate(avl, x);
		updateHeight(avl, x);
		updateHeight(avl, y);
	}


	static void insert(AvlTree &avl, NodeDescriptor u)
	{
//		cout << "ENTER" << endl;
		BstImplement::insert(avl, u);
//		cout << "end insert" << endl;


		rebalance(avl, u);
//		cout << "end re" << endl;
	}

	static std::pair<NodeDescriptor, NodeDescriptor> erase(AvlTree &avl, NodeDescriptor u)
	{
		std::pair<NodeDescriptor, NodeDescriptor> res = BstImplement::erase(avl, u);

		rebalance(avl, res.first);
		return res;
	}


};








































}// namesapce lz

#endif /* LZ_AVL_TREE_H_ */
