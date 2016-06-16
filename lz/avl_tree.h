/*
 * avl_tree.h
 *
 *  Created on: 2016��5��16��
 *      Author: LZ
 */

#ifndef LZ_AVL_TREE_H_
#define LZ_AVL_TREE_H_

#include <queue>

#include <lz/binary_search_tree.h>
namespace lz{

//for DEBUG
using namespace std;

	template<typename DerivedNode, typename _KeyType>
	struct AvlTreeNodeFacade: public BstNodeFacade<DerivedNode, _KeyType>
	{
		using HeightType = int;
		HeightType height = 1;
		AvlTreeNodeFacade(const _KeyType &key = _KeyType()):
			BstNodeFacade<DerivedNode, _KeyType>(key){}
	};


	template<typename _Node, typename _KeyCompare = std::less<typename _Node::KeyType> >
	struct AvlTree: public BinarySearchTree<_Node, _KeyCompare>
	{
		using Base = BinarySearchTree<_Node, _KeyCompare>;
	public:
		using Base::key;
		using typename Base::NodeDescriptor;

		using HeightType = typename _Node::HeightType;

		HeightType& height(NodeDescriptor u) { return u->height; }
		const HeightType& height(NodeDescriptor u) const { return u->height; }

		void out(NodeDescriptor u) const
		{
			cout << "key:" << key(u) << " height:" << height(u) << endl;
		}
	};



	template<typename AvlTree, typename Visitors = BstVisitor>
	struct AvlTreeImplement : BstImplement<AvlTree, Visitors>
	{
	private:
		using Base = BstImplement<AvlTree, Visitors>;

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

		static void leftRotate(AvlTree &avl, NodeDescriptor x)
		{
			NodeDescriptor y = avl.rightChild(x);
			Base::leftRotate(avl, x);
			updateHeight(avl, x);
			updateHeight(avl, y);
		}
		static void rightRotate(AvlTree &avl, NodeDescriptor x)
		{
			NodeDescriptor y = avl.leftChild(x);
			Base::rightRotate(avl, x);
			updateHeight(avl, x);
			updateHeight(avl, y);
		}

	public:
		static void insert(AvlTree &avl, NodeDescriptor u)
		{
			Base::insert(avl, u);
			rebalance(avl, u);
		}

		static void erase(AvlTree &avl, NodeDescriptor u)
		{
			auto res = Base::erase(avl, u);

			rebalance(avl, res.first);
		}


	};








































}// namesapce lz

#endif /* LZ_AVL_TREE_H_ */
