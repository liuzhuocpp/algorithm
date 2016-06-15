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

//	struct BstVisitor
//	{
//		template<typename B, typename N>
//		static void finishInsert(B &b, N u){}
//
//		template<typename B, typename N>
//		static void finishErase(B &b, N u){}
//
//		template<typename B, typename N>
//		static void finishLeftRotate(B &b, N u){}
//
//		template<typename B, typename N>
//		static void finishRightRotate(B &b, N u){}
//	};
//
//
//	template<typename Bst, typename Vistors = BstVisitor>
//	struct BstImplement
//	{
//	private:
//		using KeyType = typename Bst::KeyType;
//		using KeyCompare = typename Bst::KeyCompare;
//		using NodeDescriptor = typename Bst::NodeDescriptor;
//
//		static constexpr NodeDescriptor nullNode = Bst::nullNode();
//
//	public:
//
//		static NodeDescriptor& findInsertPos(Bst &bst, const KeyType &key, NodeDescriptor &parent)
//		{
//			if(bst.root() == nullNode) return bst.root();
//
//			NodeDescriptor u = bst.root();
//			for(;;)
//			{
//
//				if(KeyCompare()(key, bst.key(u)))
//				{
//					if(bst.leftChild(u) == nullNode)
//					{
//						parent = u;
//						return bst.leftChild(u);
//					}
//					else u = bst.leftChild(u);
//				}
//				else
//				{
//					if(bst.rightChild(u) == nullNode)
//					{
//						parent = u;
//						return bst.rightChild(u);
//					}
//					else u = bst.rightChild(u);
//				}
//			}
//
//			cout << "ERROR in findInsertPos" << endl;
//
//
//		}
//
//		static void insert(Bst &bst, NodeDescriptor u)
//		{
//			NodeDescriptor parent = nullNode;
//			findInsertPos(bst, bst.key(u), parent) = u;
//			bst.parent(u) = parent;
//
//			Vistors::finishInsert(bst, u);
//		}
//
//		static NodeDescriptor& getParentChild(Bst &bst, NodeDescriptor u)
//		{
//			NodeDescriptor p = bst.parent(u);
//			if(bst.leftChild(p) == u)
//			{
//				return bst.leftChild(p);
//			}
//			else return bst.rightChild(p);
//		}
//
//
//
//		static NodeDescriptor find(const Bst &bst, const KeyType &key)
//		{
//			NodeDescriptor root = bst.root();
//			for(;;)
//			{
//				if(root == nullNode) return root;
//
//				if(KeyCompare()(key, bst.key(root)))
//				{
//					root = bst.leftChild(root);
//				}
//				else if(KeyCompare()(bst.key(root), key))
//				{
//					root = bst.rightChild(root);
//				}
//				else return root;
//			}
//			return nullNode;
//		}
//
//
////返回受影响的深度最大的节点
//		static NodeDescriptor erase(Bst &bst, NodeDescriptor u)
//		{
//			NodeDescriptor copy_null = nullNode;
//
//			NodeDescriptor &p = bst.parent(u);
//			NodeDescriptor &l = bst.leftChild(u);
//			NodeDescriptor &r = bst.rightChild(u);
//
//
//
//			NodeDescriptor& u_p_pos = (p == nullNode ? copy_null : getParentChild(bst, u));
//
//			NodeDescriptor res = nullNode;
//
//			if(l == nullNode && r == nullNode)
//			{
//				if(p != nullNode) u_p_pos = nullNode;
//				else bst.root() = nullNode;
//				res = p;
//			}
//			else if(l != nullNode && r == nullNode)
//			{
//				if(p != nullNode) u_p_pos = l;
//				else bst.root() = l;
//				bst.parent(l) = p;
//				res = p;
//			}
//			else if(l == nullNode && r != nullNode)
//			{
//				if(p != nullNode) u_p_pos = r;
//				else bst.root() = r;
//				bst.parent(r) = p;
//				res = p;
//			}
//			else
//			{
//
//
//				NodeDescriptor r_most = l;
//				for(;bst.rightChild(r_most) != nullNode; r_most = bst.rightChild(r_most) );
//
//				if(r_most == l)
//				{
//					bst.parent(r_most) = p;
//					bst.rightChild(r_most) = r;
//
//					if(p != nullNode) u_p_pos = r_most;
//					bst.parent(r) = r_most;
//					res = l;
//				}
//				else
//				{
//					NodeDescriptor r_most_p = bst.parent(r_most);
//					bst.rightChild(r_most_p) = bst.leftChild(r_most);
//
//					if(bst.leftChild(r_most) != nullNode)
//						bst.parent(bst.leftChild(r_most)) = r_most_p;
//
//					bst.rightChild(r_most) = r;
//					bst.leftChild(r_most) = l;
//					bst.parent(r_most) = p;
//
//					if(p != nullNode) u_p_pos = r_most;
//					bst.parent(l) = r_most;
//					bst.parent(r) = r_most;
//
//					res = r_most_p;
//				}
//				if(u == bst.root())
//					bst.root() = r_most;
//
//
//			}
//
//
//
//			Vistors::finishErase(bst, res);
//
//			return res;
////			bst.destoryVertex(u);
//
//		}
//
//
//		/*
//		 * 			x								  y
//		 * 		  /   \		  left rotate   	     / \
//		 * 		 A     y	  ------->    			x   C
//		 * 		 	  / \	  right rotate  	   / \
//		 * 		 	 B	 C	  <-----			  A	  B
//		 */
//		static void leftRotate(Bst &bst, NodeDescriptor x)
//		{
//			NodeDescriptor y = bst.rightChild(x);
//			NodeDescriptor p = bst.parent(x);
//			NodeDescriptor B = bst.leftChild(y);
//			NodeDescriptor tmpVertex = nullNode;
//			NodeDescriptor& x_p_pos = (p == nullNode ? tmpVertex : getParentChild(bst, x));
//
//			bst.rightChild(x) = B;
//			bst.parent(x) = y;
//
//			bst.leftChild(y) = x;
//			bst.parent(y) = p;
//
//			if(B != nullNode) bst.parent(B) = x;
//			if(p != nullNode) x_p_pos = y;
//
//			if(bst.root() == x) bst.root() = y;
//
//// 新的根为y
//			Vistors::finishLeftRotate(bst, y);
//
//		}
//		static void rightRotate(Bst &bst, NodeDescriptor y)
//		{
//			NodeDescriptor x = bst.leftChild(y);
//			NodeDescriptor p = bst.parent(y);
//			NodeDescriptor B = bst.rightChild(x);
//			NodeDescriptor tmpVertex = nullNode;
//			NodeDescriptor& y_p_pos = (p == nullNode ? tmpVertex : getParentChild(bst, y));
//
//			bst.leftChild(y) = B;
//			bst.parent(y) = x;
//
//			bst.rightChild(x) = y;
//			bst.parent(x) = p;
//
//			if(B != nullNode) bst.parent(B) = y;
//			if(p != nullNode) y_p_pos = x;
//
//			if(bst.root() == y) bst.root() = x;
//
//			Vistors::finishRightRotate(bst, x);
//
//		}
//
//
//
////		static void erase(Bst &bst, const KeyType &key)
////		{
////			erase(bst, find(bst, key));
////		}
//
//
//		static NodeDescriptor nextVertex(const Bst &bst, NodeDescriptor root)
//		{
//			if(bst.rightChild(root) == nullNode)
//			{
//				if(bst.parent(root) == nullNode) return nullNode;
//
//				if(bst.leftChild(bst.parent()) == root)
//				{
//					return bst.parent(root);
//				}
//				else return nullNode;
//			}
//			root = bst.rightChild(root);
//
//			for(;bst.leftChild(root) != nullNode;)
//			{
//				root = bst.leftChild(root);
//			}
//			return root;
//		}
//
//		static void bfs(const Bst &bst)
//		{
//	//		std::queue<std::pair<Node*, int> > q;
//			if(bst.root() == nullNode) return ;
//			using D = std::pair<NodeDescriptor, int>;
//			std::queue<D  > q;
//
//			q.push(make_pair(bst.root(), 0));
//			int cntDep = -1;
//			while(q.size())
//			{
//				D u = q.front();
//				q.pop();
//				if(cntDep != u.second)
//				{
//					cntDep = u.second;
//					cout <<  std::string(100, '=') << "now dep is: " << cntDep <<endl;
//				}
//				bst.out(u.first);
//
//				if(bst.leftChild(u.first) != nullNode)
//				{
//					q.push(std::make_pair(bst.leftChild(u.first), u.second + 1));
//				}
//
//				if(bst.rightChild(u.first) != nullNode)
//				{
//					q.push(std::make_pair(bst.rightChild(u.first), u.second + 1));
//				}
//			}
//		}
//	};
//
//
//	//默认的BstNodeFacade
//	template<typename DerivedNode, typename _KeyType>
//	struct BstNodeFacade
//	{
//		using KeyType = _KeyType;
//
//		DerivedNode* leftChild = nullptr;
//		DerivedNode* rightChild = nullptr;
//		DerivedNode* parent = nullptr;
//		KeyType key;
//		BstNodeFacade(const KeyType &key = KeyType()): key(key){}
//	};
//
//
//
//
//	template<typename _Node, typename _KeyCompare = std::less<typename _Node::KeyType> >
//	struct BinarySearchTree
//	{
//private:
//		_Node* _root = nullptr;
//public:
//
////		for debug
//		void out(_Node* u) const
//		{
//			cout << u->key << endl;
//		}
//
//
////below implement bst concept
//		using KeyType = typename _Node::KeyType;
//		using KeyCompare = _KeyCompare;
//		using NodeDescriptor = _Node*;
//		static constexpr NodeDescriptor nullNode() { return nullptr; }
//
//
//
//		NodeDescriptor& root() { return _root; }
//		NodeDescriptor& leftChild(NodeDescriptor u) { return u->leftChild; }
//		NodeDescriptor& rightChild(NodeDescriptor u) { return u->rightChild; }
//		NodeDescriptor& parent(NodeDescriptor u) { return u->parent; }
//		KeyType& key(NodeDescriptor u) { return u->key; }
//
//		const NodeDescriptor& root() const { return _root; }
//		const NodeDescriptor& leftChild(NodeDescriptor u) const { return u->leftChild; }
//		const NodeDescriptor& rightChild(NodeDescriptor u) const { return u->rightChild; }
//		const NodeDescriptor& parent(NodeDescriptor u) const { return u->parent; }
//		const KeyType& key(NodeDescriptor u) const { return u->key; }
//	};
//
//
//
//



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

		static NodeDescriptor erase(AvlTree &avl, NodeDescriptor u)
		{
			NodeDescriptor res = Base::erase(avl, u);

			rebalance(avl, res);
			return res;
		}


	};








//	struct SizedBstNodeBase
//	{
//	public:
//		using SizeType = int;
//		SizeType size = 0;
//	};
//
//
//	template<typename _Node>
//	struct SizedBstBase
//	{
//	private:
////		using NodeDescriptor = _Node*;  // 使用此句会产生bug
//		using _NodeDescriptor = _Node*;
//	public:
//		using SizeType = typename _Node::SizeType;
//		SizeType& size(_NodeDescriptor u) { return u->size; }
//		const SizeType& size(_NodeDescriptor u) const { return u->size; }
//	};
//
//
//
//
//	template<typename SizedBst>
//	struct SizedBstVisitor
//	{
//
//	private:
//		using SizeType = typename SizedBst::SizeType;
//		using NodeDescriptor = typename SizedBst::NodeDescriptor;
//		static constexpr NodeDescriptor nullNode = SizedBst::nullNode();
//
//		static SizeType size(const SizedBst &bst, NodeDescriptor u)
//		{
//			if(u == nullNode) return 0;
//			else return bst.size(u);
//		}
//
//		static void updateSize(SizedBst &bst, NodeDescriptor u)
//		{
//			bst.size(u) = size(bst, bst.leftChild(u)) + size(bst, bst.rightChild(u)) + 1;
//		}
//		static void updateSizeToRoot(SizedBst &bst, NodeDescriptor u)
//		{
//			for(;u != nullNode; u = bst.parent(u))
//			{
//				updateSize(bst, u);
//			}
//		}
//
//	public:
//		static void finishInsert(SizedBst &bst, NodeDescriptor u)
//		{
//			updateSizeToRoot(bst, u);
//
//		}
//		static void finishErase(SizedBst &bst, NodeDescriptor u)
//		{
//			updateSizeToRoot(bst, u);
//		}
//		static void finishLeftRotate(SizedBst &bst, NodeDescriptor x)
//		{
//			NodeDescriptor y = bst.leftChild(x);
//			updateSize(bst, y);
//			updateSize(bst, x);
//
//		}
//		static void finishRightRotate(SizedBst &bst, NodeDescriptor x)
//		{
//			NodeDescriptor y = bst.rightChild(x);
//			updateSize(bst, y);
//			updateSize(bst, x);
//
//		}
////按照下标索引
//		static NodeDescriptor at(const SizedBst &bst, SizeType i)
//		{
//			++ i;
//			NodeDescriptor u = bst.root();
//			for(;;)
//			{
//
//				SizeType ls = size(bst, bst.leftChild(u));
//				if(ls + 1 == i ) break;
//				if(ls + 1 < i)
//				{
//					i -= ls + 1;
//					u = bst.rightChild(u);
//				}
//				else u = bst.leftChild(u);
//			}
//			return u;
//		}
//	};
//
//	template<typename SizedBst>
//	typename SizedBst::NodeDescriptor sizedBstAt(const SizedBst &bst, typename SizedBst::SizeType i)
//	{
//		return SizedBstVisitor<SizedBst>::at(bst, i);
//	}







//	template<typename DerivedNode, typename KeyType>
//	struct TreapNodeFacade: BstNodeFacade<DerivedNode, KeyType>
//	{
//		using PriorityType = int;
//		PriorityType priority;
//	};
//
//	template<typename _Node, typename _KeyCompare>
//	struct Treap:BinarySearchTree<_Node, _KeyCompare>
//	{
//	private:
//		using Base = BinarySearchTree<_Node, _KeyCompare>;
//
//	public:
//		using PriorityType = typename _Node::PriorityType;
//		using typename Base::NodeDescriptor;
//
//		PriorityType& priority(NodeDescriptor u) { return u->priority; }
//		const PriorityType& priority(NodeDescriptor u)const  { return u->priority; }
//	};
//
//
//	template<typename Treap, typename Visitor = BstVisitor>
//	struct TreapImplement: BstImplement<Treap, Visitor>
//	{
//	private:
//		using NodeDescriptor = typename Treap::NodeDescriptor;
//		using PriorityType = typename Treap::PriorityType;
//		using Base = BstImplement<Treap, Visitor>;
//		constexpr static NodeDescriptor nullNode = Treap::nullNode();
//
//		static void adjust(Treap &tr, NodeDescriptor u)
//		{
//
//			for(;;)
//			{
//				NodeDescriptor p = tr.parent(u);
//				if(p == nullNode) break;
//
//				if(tr.priority(p) > tr.priority(u))
//				{
//					if(tr.leftChild(p) == u)
//					{
//						Base::rightRotate(tr, p);
//					}
//					else
//					{
//						Base::leftChild(tr, p);
//					}
//				}
//				else u = p;
//			}
//		}
//	public:
//		static void insert(Treap &tr, NodeDescriptor u)
//		{
//			Base::insert(tr, u);
//			adjust(tr, u);
//		}
//		static NodeDescriptor erase(Treap &tr, NodeDescriptor u)
//		{
//			NodeDescriptor res = Base::erase(tr, u);
//			adjust(tr, u);
//
//			return res;
//		}
//
//
//	};

































}// namesapce lz

#endif /* LZ_AVL_TREE_H_ */
