/*
 * treap.h
 *
 *  Created on: 2016年6月15日
 *      Author: LZ
 */

#ifndef LZ_TREAP_H_
#define LZ_TREAP_H_

//#include <lz/binary_search_tree.h>


namespace lz{


//for debug
using namespace std;





struct TreapNodeBase
{
	using PriorityType = int;
	PriorityType priority;
};



template<typename TreapNode>
struct TreapBase
{
public:
	using PriorityType = typename TreapNode::PriorityType;

	PriorityType& priority(TreapNode* u) { return u->priority; }
	const PriorityType& priority(TreapNode* u)const  { return u->priority; }

////	for debug
//	void out(TreapNode* u) const
//	{
//		cout << "| priority:" << priority(u);
//	}
//
};


template<typename BstImplement>
struct TreapImplement: BstImplement
{
private:
	using Treap = typename BstImplement::BinarySearchTree;

	using NodeDescriptor = typename Treap::NodeDescriptor;
	using PriorityType = typename Treap::PriorityType;

	constexpr static NodeDescriptor nullNode = Treap::nullNode();

	using BstImplement::rightRotate;
	using BstImplement::leftRotate;
	static void adjust(Treap &tr, NodeDescriptor u)
	{
		for(;;)
		{
			NodeDescriptor p = tr.parent(u);
			if(p == nullNode) break;

			if(tr.priority(p) > tr.priority(u))
			{
				if(tr.leftChild(p) == u)
				{
					BstImplement::rightRotate(tr, p);
				}
				else
				{
					BstImplement::leftRotate(tr, p);
				}
			}
			else u = p;
		}
	}
public:
	static void insert(Treap &tr, NodeDescriptor u)
	{
		BstImplement::insert(tr, u);
		adjust(tr, u);
	}
	static std::pair<NodeDescriptor, NodeDescriptor> erase(Treap &tr, NodeDescriptor u)
	{
		std::pair<NodeDescriptor, NodeDescriptor> res = BstImplement::erase(tr, u);
		adjust(tr, res.first);

		return res;

	}


};



}// namesapce lz

#endif /* LZ_TREAP_H_ */
