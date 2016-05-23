/*
 * avl_tree.h
 *
 *  Created on: 2016年5月16日
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
		using VertexDescriptor = typename BinerySearchTree::VertexDescriptor;
		static constexpr VertexDescriptor nullVertex() { return BinerySearchTree::nullVertex(); }

//		VertexDescriptor& root();
//		VertexDescriptor& leftChild(VertexDescriptor u);
//		VertexDescriptor& rightChild(VertexDescriptor u);
//      VertexDescriptor& parent(VertexDescriptor u);
//      KeyType& key(VertexDesciptor u);
//		void destoryVertex(VertexDesciptor u);

	};

	class TestBinerySearchTree
	{
		struct Node
		{
			int key;
			Node* left;
			Node* right;
			Node* parent;

//			Node(int key):key(key), left(nullptr), right(nullptr), parent(nullptr){}
		};
		Node* _root;
	public:
		using KeyType = int;
		using KeyCompare = std::less<int>;
		using VertexDescriptor = Node*;
		static constexpr VertexDescriptor nullVertex() { return nullptr; }

		TestBinerySearchTree():_root(nullptr){}
		VertexDescriptor createVertex(const KeyType &key = KeyType(),
									  Node* left = nullptr,
									  Node* right = nullptr,
									  Node* parent = nullptr

									  )
		{
			return new Node{key, left, right, parent};
		}

		void destoryVertex(VertexDescriptor u)
		{
			delete u;
		}

		VertexDescriptor& root() { return _root; }
		VertexDescriptor& leftChild(VertexDescriptor u) { return u->left; }
		VertexDescriptor& rightChild(VertexDescriptor u) { return u->right; }
		VertexDescriptor& parent(VertexDescriptor u) { return u->parent; }
		KeyType& key(VertexDescriptor u) { return u->key; }

		const VertexDescriptor& root() const { return _root; }
		const VertexDescriptor& leftChild(VertexDescriptor u) const { return u->left; }
		const VertexDescriptor& rightChild(VertexDescriptor u) const { return u->right; }
		const VertexDescriptor& parent(VertexDescriptor u) const { return u->parent; }
		const KeyType& key(VertexDescriptor u) const { return u->key; }

	};



	template<typename Bst>
	struct BinerySearchTreeImplement
	{
		using KeyType = typename BinerySearchTreeTraits<Bst>::KeyType;
		using KeyCompare = typename BinerySearchTreeTraits<Bst>::KeyCompare;
		using VertexDescriptor = typename BinerySearchTreeTraits<Bst>::VertexDescriptor;

		static constexpr VertexDescriptor nullVertex = BinerySearchTreeTraits<Bst>::nullVertex();

		static void insert(Bst &bst, VertexDescriptor q)
		{
			VertexDescriptor root = bst.root();
			if(root == nullVertex)
			{
				bst.root() = q;
				return ;
			}
			for(;;)
			{
				if(root == nullVertex) break;

				if(KeyCompare()(bst.key(q), bst.key(root)))
				{
					if(bst.leftChild(root) == nullVertex)
					{
						bst.leftChild(root) = q;
						bst.parent(q) = root;
						break;
					}
					else root = bst.leftChild(root);
				}
				else
				{
					if(bst.rightChild(root) == nullVertex)
					{
						bst.rightChild(root) = q;
						bst.parent(q) = root;
						break;
					}
					else root = bst.rightChild(root);
				}
			}
		}

		static VertexDescriptor& getParentChild(Bst &bst, VertexDescriptor u)
		{
			VertexDescriptor p = bst.parent(u);
			if(bst.leftChild(p) == u)
			{
				return bst.leftChild(p);
			}
			else return bst.rightChild(p);
		}



		static VertexDescriptor search(const Bst &bst, const KeyType &key)
		{
			VertexDescriptor root = bst.root();
			for(;;)
			{
				if(root == nullVertex) return root;

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
			return nullVertex;
		}

		static void remove(Bst &bst, VertexDescriptor u)
		{
			VertexDescriptor copy_null = nullVertex;

			VertexDescriptor &p = bst.parent(u);
			VertexDescriptor &l = bst.leftChild(u);
			VertexDescriptor &r = bst.rightChild(u);


			// u在父亲中的位置
			VertexDescriptor& u_p_pos = (p == nullVertex ? copy_null : getParentChild(bst, u));

			if(l == nullVertex && r == nullVertex)
			{
				if(p != nullVertex) u_p_pos = nullVertex;
				else bst.root() = nullVertex;
			}
			else if(l != nullVertex && r == nullVertex)
			{
				if(p != nullVertex) u_p_pos = l;
				else bst.root() = l;
				bst.parent(l) = p;
			}
			else if(l == nullVertex && r != nullVertex)
			{
				if(p != nullVertex) u_p_pos = r;
				else bst.root() = r;
				bst.parent(r) = p;
			}
			else
			{

				VertexDescriptor r_most = l;
				for(;bst.rightChild(r_most) != nullVertex; r_most = bst.rightChild(r_most) );

				if(r_most == l)
				{
					bst.parent(r_most) = p;
					bst.rightChild(r_most) = r;
					if(p != nullVertex) u_p_pos = r_most;
				}
				else
				{
					VertexDescriptor r_most_p = bst.parent(r_most);
					bst.rightChild(r_most_p) = bst.leftChild(r_most);

					if(bst.leftChild(r_most) != nullVertex)
						bst.parent(bst.leftChild(r_most)) = r_most_p;

					bst.rightChild(r_most) = r;
					bst.leftChild(r_most) = l;
					bst.parent(r_most) = p;

					if(p != nullVertex) u_p_pos = r_most;
					bst.parent(l) = r_most;
					bst.parent(r) = r_most;
				}



				if(u == bst.root())
					bst.root() = r_most;

			}

			bst.destoryVertex(u);

		}


		/*
		 * 			x								  y
		 * 		  /   \		  left rotate   	     / \
		 * 		 A     y	  ------->    			x   C
		 * 		 	  / \	  right rotate  	   / \
		 * 		 	 B	 C	  <-----			  A	  B
		 */
		static void leftRotate(Bst &bst, VertexDescriptor x)
		{
			VertexDescriptor y = bst.leftChild(x);
			VertexDescriptor p = bst.parent(x);
			VertexDescriptor B = bst.leftChild(y);
			VertexDescriptor tmpVertex = nullVertex;
			VertexDescriptor& x_p_pos = (p == nullVertex ? tmpVertex : getParentChild(x));

			bst.rightChild(x) = B;
			bst.parent(x) = y;

			bst.leftChild(y) = x;
			bst.parent(y) = p;

			if(B != nullVertex) bst.parent(B) = x;
			if(p != nullVertex) x_p_pos = y;

		}
		static void rightRotate(Bst &bst, VertexDescriptor y)
		{
			VertexDescriptor x = bst.rightChild(y);
			VertexDescriptor p = bst.parent(y);
			VertexDescriptor B = bst.rightChild(x);
			VertexDescriptor tmpVertex = nullVertex;
			VertexDescriptor& y_p_pos = (p == nullVertex ? tmpVertex : getParentChild(y));

			bst.leftChild(y) = B;
			bst.parent(y) = x;

			bst.rightChild(x) = y;
			bst.parent(x) = p;

			if(B != nullVertex) bst.parent(B) = y;
			if(p != nullVertex) y_p_pos = x;

		}



		static void remove(Bst &bst, const KeyType &key)
		{
			remove(bst, search(bst, key));
		}


		static VertexDescriptor nextVertex(const Bst &bst, VertexDescriptor root)
		{
			if(bst.rightChild(root) == nullVertex)
			{
				if(bst.parent(root) == nullVertex) return nullVertex;

				if(bst.leftChild(bst.parent()) == root)
				{
					return bst.parent(root);
				}
				else return nullVertex;
			}
			root = bst.rightChild(root);

			for(;bst.leftChild(root) != nullVertex;)
			{
				root = bst.leftChild(root);
			}
			return root;
		}

		static void bfs(const Bst &bst)
		{
	//		std::queue<std::pair<Node*, int> > q;
			if(bst.root() == nullVertex) return ;
			std::queue<VertexDescriptor > q;

			q.push(bst.root());
			while(q.size())
			{
				VertexDescriptor u = q.front();
				q.pop();
				std::cout << bst.key(u) << std::endl;
				if(bst.leftChild(u) != nullVertex)
				{
					q.push(bst.leftChild(u));
				}

				if(bst.rightChild(u) != nullVertex)
				{
					q.push(bst.rightChild(u));
				}
			}


		}



	};















//	template<typename Node>
//	struct BstNodeTraits
//	{
//		using KeyType = typename Node::KeyType;
//		using KeyCompare = typename Node::KeyCompare;
//
//		// Node*& parent();
//		// Node*& leftChild() ;
//		// Node*& rightChild();
//		// KeyType& key();
//		// Node*& otherChild(const Node* cnt);
//		// Node(const KeyType& key);
//		// nullptr
//	};
//
//	template<typename Node>
//	void bstBfs(Node *root)
//	{
////		std::queue<std::pair<Node*, int> > q;
//		std::queue<Node* > q;
//		q.push(root);
//		while(q.size())
//		{
//			Node* u = q.front();
//			q.pop();
//			std::cout << u->key() << std::endl;
//			if(u->leftChild() != nullptr)
//			{
//				q.push(u->leftChild());
//			}
//
//			if(u->rightChild() != nullptr)
//			{
//				q.push(u->rightChild());
//			}
//		}
//
//
//	}
//
//	template<typename Node>
//	void bstInsert(Node* root, Node *q)
//	{
//		using KeyType = typename BstNodeTraits<Node>::KeyType;
//		using Less = typename BstNodeTraits<Node>::KeyCompare;
//
//		for(;;)
//		{
//			if(root == nullptr) break;
//
//			if(Less()(q->key(), root->key()))
//			{
//				if(root->leftChild() == nullptr)
//				{
//					root->leftChild() = q;
//					q->parent() = root;
//					break;
//				}
//				else root = root->leftChild();
//			}
//			else
//			{
//				if(root->rightChild() == nullptr)
//				{
//					root->rightChild() = q;
//					q->parent() = root;
//					break;
//				}
//				else root = root->rightChild();
//			}
//		}
//	}
//
//	template<typename Node>
//	Node* bstSearch(Node *root, const typename BstNodeTraits<Node>::KeyType &key)
//	{
//		using KeyType = typename BstNodeTraits<Node>::KeyType;
//		using Less = typename BstNodeTraits<Node>::KeyCompare;
//
//		for(;;)
//		{
//			if(root == nullptr) return root;
//			if(Less()(key, root->key()))
//			{
//				root = root->leftChild();
//			}
//			else if(Less()(root->key(), key))
//			{
//				root = root->rightChild();
//			}
//			else return root;
//		}
//		return nullptr;
//	}
//
//	template<typename Node>
//	Node* bstNextNode(Node *root)
//	{
//		if(root->rightChild() == nullptr)
//		{
//			if(root->parent() == nullptr) return nullptr;
//
//			if(root->parent() ->leftChild() == root)
//			{
//				return root->parent();
//			}
//			else return nullptr;
//		}
//		root = root->rightChild();
//
//		for(;root->leftChild() != nullptr;)
//		{
//			root = root->leftChild();
//		}
//		return root;
//	}
//	template<typename Node>
//	Node* bstSearchAll(Node* root, const typename BstNodeTraits<Node>::KeyType &key)
//	{
//
//	}

















}// namesapce lz

#endif /* LZ_AVL_TREE_H_ */
