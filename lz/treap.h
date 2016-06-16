/*
 * treap.h
 *
 *  Created on: 2016年6月15日
 *      Author: LZ
 */

#ifndef LZ_TREAP_H_
#define LZ_TREAP_H_

#include <lz/binary_search_tree.h>


namespace lz{


//for debug
using namespace std;




template<typename DerivedNode, typename KeyType>
struct TreapNodeFacade: public BstNodeFacade<DerivedNode, KeyType>
{
	using PriorityType = int;

	PriorityType priority;


	TreapNodeFacade(const KeyType &key = KeyType(), const PriorityType &_p = PriorityType() ):
					BstNodeFacade<DerivedNode, KeyType>(key),
					priority(_p)
					{}


};



template<typename _Node, typename _KeyCompare = std::less<typename _Node::KeyType> >
struct Treap:public BinarySearchTree<_Node, _KeyCompare>
{
private:
	using Base = BinarySearchTree<_Node, _KeyCompare>;

public:
	using PriorityType = typename _Node::PriorityType;
	using typename Base::NodeDescriptor;

	PriorityType& priority(NodeDescriptor u) { return u->priority; }
	const PriorityType& priority(NodeDescriptor u)const  { return u->priority; }


//	for debug
	void out(NodeDescriptor u) const
	{
		Base::out(u);
		cout << "| priority:" << priority(u);
	}

};


template<typename Treap, typename Visitor = BstVisitor>
struct TreapImplement: BstImplement<Treap, Visitor>
{
private:
	using NodeDescriptor = typename Treap::NodeDescriptor;
	using PriorityType = typename Treap::PriorityType;
	using Base = BstImplement<Treap, Visitor>;
	constexpr static NodeDescriptor nullNode = Treap::nullNode();

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
					Base::rightRotate(tr, p);
				}
				else
				{
					Base::leftRotate(tr, p);
				}
			}
			else u = p;
		}
	}
public:
	static void insert(Treap &tr, NodeDescriptor u)
	{
		Base::insert(tr, u);
		adjust(tr, u);
	}
	static void erase(Treap &tr, NodeDescriptor u)
	{
		NodeDescriptor res = Base::erase(tr, u);
		adjust(tr, res);


	}


};



}// namesapce lz

#endif /* LZ_TREAP_H_ */
