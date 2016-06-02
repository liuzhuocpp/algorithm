/*
 * avl_tree.h
 *
 *  Created on: 2016��5��16��
 *      Author: LZ
 */

#ifndef LZ_AVL_TREE_H_
#define LZ_AVL_TREE_H_

#include <queue>

namespace lz{

//for DEBUG
using namespace std;

	template<typename BinerySearchTree>
	struct BinerySearchTreeTraits
	{

		using KeyType = typename BinerySearchTree::KeyType;
		using KeyCompare = typename BinerySearchTree::KeyCompare;
		using NodeDescriptor = typename BinerySearchTree::NodeDescriptor;
		static constexpr NodeDescriptor nullNode() { return BinerySearchTree::nullVertex(); }

//		VertexDescriptor& root();
//		VertexDescriptor& leftChild(VertexDescriptor u);
//		VertexDescriptor& rightChild(VertexDescriptor u);
//      VertexDescriptor& parent(VertexDescriptor u);
//      KeyType& key(VertexDesciptor u);

	};










	template<typename Bst>
	struct BinerySearchTreeImplement
	{
		using KeyType = typename BinerySearchTreeTraits<Bst>::KeyType;
		using KeyCompare = typename BinerySearchTreeTraits<Bst>::KeyCompare;
		using NodeDescriptor = typename BinerySearchTreeTraits<Bst>::NodeDescriptor;

		static constexpr NodeDescriptor nullNode = BinerySearchTreeTraits<Bst>::nullNode();



		static NodeDescriptor& findInsertPos(Bst &bst, const KeyType &key, NodeDescriptor &parent)
		{
			if(bst.root() == nullNode) return bst.root();

			NodeDescriptor u = bst.root();
			for(;;)
			{

				if(KeyCompare()(key, bst.key(u)))
				{
					if(bst.leftChild(u) == nullNode)
					{
						parent = u;
						return bst.leftChild(u);
					}
					else u = bst.leftChild(u);
				}
				else
				{
					if(bst.rightChild(u) == nullNode)
					{
						parent = u;
						return bst.rightChild(u);
					}
					else u = bst.rightChild(u);
				}
			}

			cout << "ERROR in findInsertPos" << endl;


		}

		static void insert(Bst &bst, NodeDescriptor u)
		{
			NodeDescriptor parent = nullptr;
			findInsertPos(bst, bst.key(u), parent) = u;
			bst.parent(u) = parent;
		}

		static NodeDescriptor& getParentChild(Bst &bst, NodeDescriptor u)
		{
			NodeDescriptor p = bst.parent(u);
			if(bst.leftChild(p) == u)
			{
				return bst.leftChild(p);
			}
			else return bst.rightChild(p);
		}



		static NodeDescriptor find(const Bst &bst, const KeyType &key)
		{
			NodeDescriptor root = bst.root();
			for(;;)
			{
				if(root == nullNode) return root;

				if(KeyCompare()(key, bst.key(root)))
				{
					root = bst.leftChild(root);
				}
				else if(KeyCompare()(bst.key(root), key))
				{
					root = bst.rightChild(root);
				}
				else return root;
			}
			return nullNode;
		}


//返回受影响的深度最大的节点
		static NodeDescriptor erase(Bst &bst, NodeDescriptor u)
		{
			NodeDescriptor copy_null = nullNode;

			NodeDescriptor &p = bst.parent(u);
			NodeDescriptor &l = bst.leftChild(u);
			NodeDescriptor &r = bst.rightChild(u);


			// u�ڸ����е�λ��
			NodeDescriptor& u_p_pos = (p == nullNode ? copy_null : getParentChild(bst, u));

			if(l == nullNode && r == nullNode)
			{
				if(p != nullNode) u_p_pos = nullNode;
				else bst.root() = nullNode;
				return p;
			}
			else if(l != nullNode && r == nullNode)
			{
				if(p != nullNode) u_p_pos = l;
				else bst.root() = l;
				bst.parent(l) = p;
				return p;
			}
			else if(l == nullNode && r != nullNode)
			{
				if(p != nullNode) u_p_pos = r;
				else bst.root() = r;
				bst.parent(r) = p;
				return p;
			}
			else
			{
				NodeDescriptor res = nullNode;

				NodeDescriptor r_most = l;
				for(;bst.rightChild(r_most) != nullNode; r_most = bst.rightChild(r_most) );

				if(r_most == l)
				{
					bst.parent(r_most) = p;
					bst.rightChild(r_most) = r;

					if(p != nullNode) u_p_pos = r_most;
					bst.parent(r) = r_most;
					res = l;
				}
				else
				{
					NodeDescriptor r_most_p = bst.parent(r_most);
					bst.rightChild(r_most_p) = bst.leftChild(r_most);

					if(bst.leftChild(r_most) != nullNode)
						bst.parent(bst.leftChild(r_most)) = r_most_p;

					bst.rightChild(r_most) = r;
					bst.leftChild(r_most) = l;
					bst.parent(r_most) = p;

					if(p != nullNode) u_p_pos = r_most;
					bst.parent(l) = r_most;
					bst.parent(r) = r_most;

					res = r_most_p;
				}
				if(u == bst.root())
					bst.root() = r_most;

				return res;
			}

//			bst.destoryVertex(u);

		}


		/*
		 * 			x								  y
		 * 		  /   \		  left rotate   	     / \
		 * 		 A     y	  ------->    			x   C
		 * 		 	  / \	  right rotate  	   / \
		 * 		 	 B	 C	  <-----			  A	  B
		 */
		static void leftRotate(Bst &bst, NodeDescriptor x)
		{
			NodeDescriptor y = bst.rightChild(x);
			NodeDescriptor p = bst.parent(x);
			NodeDescriptor B = bst.leftChild(y);
			NodeDescriptor tmpVertex = nullNode;
			NodeDescriptor& x_p_pos = (p == nullNode ? tmpVertex : getParentChild(bst, x));

			bst.rightChild(x) = B;
			bst.parent(x) = y;

			bst.leftChild(y) = x;
			bst.parent(y) = p;

			if(B != nullNode) bst.parent(B) = x;
			if(p != nullNode) x_p_pos = y;

			if(bst.root() == x) bst.root() = y;

		}
		static void rightRotate(Bst &bst, NodeDescriptor y)
		{
			NodeDescriptor x = bst.leftChild(y);
			NodeDescriptor p = bst.parent(y);
			NodeDescriptor B = bst.rightChild(x);
			NodeDescriptor tmpVertex = nullNode;
			NodeDescriptor& y_p_pos = (p == nullNode ? tmpVertex : getParentChild(bst, y));

			bst.leftChild(y) = B;
			bst.parent(y) = x;

			bst.rightChild(x) = y;
			bst.parent(x) = p;

			if(B != nullNode) bst.parent(B) = y;
			if(p != nullNode) y_p_pos = x;

			if(bst.root() == y) bst.root() = x;

		}



		static void erase(Bst &bst, const KeyType &key)
		{
			erase(bst, find(bst, key));
		}


		static NodeDescriptor nextVertex(const Bst &bst, NodeDescriptor root)
		{
			if(bst.rightChild(root) == nullNode)
			{
				if(bst.parent(root) == nullNode) return nullNode;

				if(bst.leftChild(bst.parent()) == root)
				{
					return bst.parent(root);
				}
				else return nullNode;
			}
			root = bst.rightChild(root);

			for(;bst.leftChild(root) != nullNode;)
			{
				root = bst.leftChild(root);
			}
			return root;
		}

		static void bfs(const Bst &bst)
		{
	//		std::queue<std::pair<Node*, int> > q;
			if(bst.root() == nullNode) return ;
			using D = std::pair<NodeDescriptor, int>;
			std::queue<D  > q;

			q.push(make_pair(bst.root(), 0));
			int cntDep = -1;
			while(q.size())
			{
				D u = q.front();
				q.pop();
				if(cntDep != u.second)
				{
					cntDep = u.second;
					cout <<  std::string(100, '=') << "now dep is: " << cntDep <<endl;
				}
				bst.out(u.first);

				if(bst.leftChild(u.first) != nullNode)
				{
					q.push(std::make_pair(bst.leftChild(u.first), u.second + 1));
				}

				if(bst.rightChild(u.first) != nullNode)
				{
					q.push(std::make_pair(bst.rightChild(u.first), u.second + 1));
				}
			}
		}



	};













	template<typename DerivedNode, typename _KeyType>
	struct BinerySearchTreeNodeFacade
	{
		using KeyType = _KeyType;
		DerivedNode* leftChild = nullptr;
		DerivedNode* rightChild = nullptr;
		DerivedNode* parent = nullptr;
		KeyType key;

		BinerySearchTreeNodeFacade(const KeyType &key = KeyType()):
			key(key){}
//		BinerySearchTreeNodeFacade(DerivedNode* leftChild, DerivedNode *rightChild, DerivedNode* parent, const KeyType &key):
//				leftChild(leftChild), rightChild(rightChild), parent(parent),key(key){}

	};

	template<typename Node, typename _KeyCompare = std::less<typename Node::KeyType> >
	struct BinerySearchTree
	{
		Node* _root;
	public:
		using KeyType = typename Node::KeyType;
		using KeyCompare = _KeyCompare;
		using NodeDescriptor = Node*;
		static constexpr NodeDescriptor nullVertex() { return nullptr; }

		BinerySearchTree():_root(nullptr){}

		NodeDescriptor& root() { return _root; }
		NodeDescriptor& leftChild(NodeDescriptor u) { return u->leftChild; }
		NodeDescriptor& rightChild(NodeDescriptor u) { return u->rightChild; }
		NodeDescriptor& parent(NodeDescriptor u) { return u->parent; }
		KeyType& key(NodeDescriptor u) { return u->key; }

		const NodeDescriptor& root() const { return _root; }
		const NodeDescriptor& leftChild(NodeDescriptor u) const { return u->leftChild; }
		const NodeDescriptor& rightChild(NodeDescriptor u) const { return u->rightChild; }
		const NodeDescriptor& parent(NodeDescriptor u) const { return u->parent; }
		const KeyType& key(NodeDescriptor u) const { return u->key; }
	};




	template<typename DerivedNode, typename _KeyType>
	struct AvlTreeNodeFacade: public BinerySearchTreeNodeFacade<DerivedNode, _KeyType>
	{
	private:
		using Base = BinerySearchTreeNodeFacade<DerivedNode, _KeyType>;
	public:
		using HeightType = int;
		HeightType height = 0;
		using KeyType = typename Base::KeyType;

		AvlTreeNodeFacade(const KeyType &key = KeyType()):
			Base(key){}
	};



	template<typename Node, typename _KeyCompare = std::less<typename Node::KeyType> >
	struct AvlTree: public BinerySearchTree<Node, _KeyCompare>
	{
		using HeightType = typename Node::HeightType;

		using Base = BinerySearchTree<Node, _KeyCompare>;

//		using typename Base::KeyCompare;
//		using typename Base::KeyType;
		using typename Base::NodeDescriptor;
//		using Base::leftChild;
//		using Base::rightChild;
		using Base::key;

		HeightType& height(NodeDescriptor u) { return u->height; }
		const HeightType& height(NodeDescriptor u) const { return u->height; }




		void out(NodeDescriptor u) const
		{
			cout << "key:" << key(u) << " height:" << height(u) << endl;
		}
	};



	template<typename AvlTree>
	struct AvlTreeImplement
	{
		using KeyType = typename BinerySearchTreeTraits<AvlTree>::KeyType;
		using KeyCompare = typename BinerySearchTreeTraits<AvlTree>::KeyCompare;
		using NodeDescriptor = typename BinerySearchTreeTraits<AvlTree>::NodeDescriptor;
		using HeightType = typename AvlTree::HeightType;

		static constexpr NodeDescriptor nullNode = BinerySearchTreeTraits<AvlTree>::nullNode();


		static HeightType diff(const AvlTree &avl, NodeDescriptor u)
		{
			HeightType lh = 0, rh = 0;
			if(avl.leftChild(u) != nullNode) lh = avl.height(avl.leftChild(u)) ;
			if(avl.rightChild(u) != nullNode) rh = avl.height(avl.rightChild(u)) ;
			return lh - rh;
		}

		static void updateHeight(AvlTree &avl, NodeDescriptor u)
		{
			HeightType lh = 0, rh = 0;
			if(avl.leftChild(u) != nullNode) lh = avl.height(avl.leftChild(u)) ;
			if(avl.rightChild(u) != nullNode) rh = avl.height(avl.rightChild(u)) ;
			avl.height(u) = std::max(lh, rh) + 1;
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
						BinerySearchTreeImplement<AvlTree>::leftRotate(avl, l);
						updateHeight(avl, l);
						updateHeight(avl, lr);
						l = lr;
					}

					BinerySearchTreeImplement<AvlTree>::rightRotate(avl, u);
					updateHeight(avl, u);
					updateHeight(avl, l);
				}
				else if(diff(avl, u) == -2)
				{
					NodeDescriptor r = avl.rightChild(u);
					if(diff(avl, r) == 1)
					{
						NodeDescriptor rl = avl.leftChild(r);
						BinerySearchTreeImplement<AvlTree>::rightRotate(avl, r);
						updateHeight(avl, r);
						updateHeight(avl, rl);
						r = rl;
					}

					BinerySearchTreeImplement<AvlTree>::leftRotate(avl, u);
					updateHeight(avl, u);
					updateHeight(avl, r);

				}
			}
		}
		static void insert(AvlTree &avl, NodeDescriptor u)
		{
			BinerySearchTreeImplement<AvlTree>::insert(avl, u);

//			cout << "before' <" << endl;

//			BinerySearchTreeImplement<AvlTree>::bfs(avl);

			rebalance(avl, u);
		}

		static void erase(AvlTree &avl, NodeDescriptor u)
		{
			u = BinerySearchTreeImplement<AvlTree>::erase(avl, u);
			rebalance(avl, u);
		}


	};










	struct TestBinerySearchTreeNode: public BinerySearchTreeNodeFacade<TestBinerySearchTreeNode, int>
	{
//		using Base = BinerySearchTreeNodeFacade<TestBinerySearchTreeNode, int>;
	};


	struct TestBinerySearchTree: public BinerySearchTree<TestBinerySearchTreeNode, int>
	{
//		using NodeDescriptor = typename BinerySearchTree<TestBinerySearchTreeNode, int>::NodeDescriptor;
	};






























}// namesapce lz

#endif /* LZ_AVL_TREE_H_ */
