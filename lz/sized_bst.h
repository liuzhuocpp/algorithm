/*
 * sized_bst.h
 *
 *  Created on: 2016年11月16日
 *      Author: LZ
 */

#ifndef LZ_SIZED_BST_H_
#define LZ_SIZED_BST_H_

//#include <lz/binary_search_tree.h>

namespace lz {

//对每个节点，添加表示以这个节点为根的子树的节点数目
struct SizedBstNodeBase
{
public:
	using SizeType = int;
	SizeType size = 0;
};


template<typename _Node>
struct SizedBstBase
{
private:
//		using NodeDescriptor = _Node*;  // 使用此句会产生bug
//	using _NodeDescriptor = _Node*;
public:
	using SizeType = typename _Node::SizeType;
	SizeType& size(_Node* u) { return u->size; }
	const SizeType& size(_Node* u) const { return u->size; }
};

template<typename BstImplement>
class SizedBstImplement: public BstImplement
{
	using SizedBst = typename BstImplement::BinarySearchTree;
	using SizeType = typename SizedBst::SizeType;
	using NodeDescriptor = typename SizedBst::NodeDescriptor;
	static constexpr NodeDescriptor nullNode = SizedBst::nullNode();

	static SizeType size(const SizedBst &bst, NodeDescriptor u)
	{
		if(u == nullNode) return 0;
		else return bst.size(u);
	}

	static void updateSize(SizedBst &bst, NodeDescriptor u)
	{
		bst.size(u) = size(bst, bst.leftChild(u)) + size(bst, bst.rightChild(u)) + 1;
	}
	static void updateSizeToRoot(SizedBst &bst, NodeDescriptor u)
	{
		for(;u != nullNode; u = bst.parent(u))
		{
			updateSize(bst, u);
		}
	}

public:
	static void insert(SizedBst &bst, NodeDescriptor u)
	{
		BstImplement::insert(bst, u);
		updateSizeToRoot(bst, u);

	}
	static std::pair<NodeDescriptor, NodeDescriptor> erase(SizedBst &bst, NodeDescriptor u)
	{
		std::pair<NodeDescriptor, NodeDescriptor> res = BstImplement::erase(bst, u);
		updateSizeToRoot(bst, res.first);
		return res;
	}
	static void leftRotate(SizedBst &bst, NodeDescriptor x)
	{
//		cout << "sized bst left rotate" << endl;
		NodeDescriptor y = bst.rightChild(x);
		BstImplement::leftRotate(bst, x);
//		cout << "sized bst left rotate end" << endl;

//		cout << "sized bst left rotate end1" << endl;
//		if(y != nullNode)
		updateSize(bst, x);
//		cout << "sized bst left rotate end2" << endl;
		updateSize(bst, y);
//		cout << "sized bst left rotate end3" << endl;

	}
	static void rightRotate(SizedBst &bst, NodeDescriptor x)
	{
		NodeDescriptor y = bst.leftChild(x);
		BstImplement::rightRotate(bst, x);

		updateSize(bst, x);
		updateSize(bst, y);

	}
//按照下标索引
	static NodeDescriptor at(const SizedBst &bst, SizeType i)
	{
		++ i;
		NodeDescriptor u = bst.root();
		for(;;)
		{

			SizeType ls = size(bst, bst.leftChild(u));
			if(ls + 1 == i ) break;
			if(ls + 1 < i)
			{
				i -= ls + 1;
				u = bst.rightChild(u);
			}
			else u = bst.leftChild(u);
		}
		return u;
	}

};




//template<typename SizedBst>
//struct SizedBstVisitor
//{
//
//private:
//	using SizeType = typename SizedBst::SizeType;
//	using NodeDescriptor = typename SizedBst::NodeDescriptor;
//	static constexpr NodeDescriptor nullNode = SizedBst::nullNode();
//
//	static SizeType size(const SizedBst &bst, NodeDescriptor u)
//	{
//		if(u == nullNode) return 0;
//		else return bst.size(u);
//	}
//
//	static void updateSize(SizedBst &bst, NodeDescriptor u)
//	{
//		bst.size(u) = size(bst, bst.leftChild(u)) + size(bst, bst.rightChild(u)) + 1;
//	}
//	static void updateSizeToRoot(SizedBst &bst, NodeDescriptor u)
//	{
//		for(;u != nullNode; u = bst.parent(u))
//		{
//			updateSize(bst, u);
//		}
//	}
//
//public:
//	static void finishInsert(SizedBst &bst, NodeDescriptor u)
//	{
//		updateSizeToRoot(bst, u);
//
//	}
//	static void finishErase(SizedBst &bst, NodeDescriptor u, NodeDescriptor v)
//	{
//		updateSizeToRoot(bst, u);
//	}
//	static void finishLeftRotate(SizedBst &bst, NodeDescriptor x)
//	{
//		NodeDescriptor y = bst.leftChild(x);
//		updateSize(bst, y);
//		updateSize(bst, x);
//
//	}
//	static void finishRightRotate(SizedBst &bst, NodeDescriptor x)
//	{
//		NodeDescriptor y = bst.rightChild(x);
//		updateSize(bst, y);
//		updateSize(bst, x);
//
//	}
////按照下标索引
//	static NodeDescriptor at(const SizedBst &bst, SizeType i)
//	{
//		++ i;
//		NodeDescriptor u = bst.root();
//		for(;;)
//		{
//
//			SizeType ls = size(bst, bst.leftChild(u));
//			if(ls + 1 == i ) break;
//			if(ls + 1 < i)
//			{
//				i -= ls + 1;
//				u = bst.rightChild(u);
//			}
//			else u = bst.leftChild(u);
//		}
//		return u;
//	}
//};
//
//template<typename SizedBst>
//typename SizedBst::NodeDescriptor sizedBstAt(const SizedBst &bst, typename SizedBst::SizeType i)
//{
//	return SizedBstVisitor<SizedBst>::at(bst, i);
//}
//
//








}// namesapce lz

#endif /* LZ_SIZED_BST_H_ */
