#ifndef TRIE_H
#define TRIE_H

#include <vector>
#include <queue>

namespace lz {

using std::queue;

template<int N>
class Trie
{
public:
	struct Node
	{
		Node *son[N];
		int tail;
		Node():tail(0) { for(int i = 0; i < N; ++ i) son[i] = nullptr; }
	};
	Node* root;

	Trie():root(new Node()) {}

	template<typename RandomIterator>
	void insert(RandomIterator first, RandomIterator end)
	{
		RandomIterator s = first;
		int n = end - first;

		Node *top = root;
		for(int i = 0; i < n; ++ i)
		{
			if(top->son[s[i]] == nullptr) 
				top->son[s[i]] = new Node();
			top = top->son[s[i]];
		}
		top->tail ++;
	}
	template<typename RandomIterator>
	int query(RandomIterator first, RandomIterator end)
	{
		RandomIterator s = first;
		int n = end - first;

		Node *top = root;
		for(int i = 0; i < n; ++ i)
		{			
			top = top->son[s[i]];
		}
		return top->tail;
	}
	~Trie()
	{
		queue<Node *> q;
		q.push(root);
		while(!q.empty())
		{
			Node *c = q.top();
			q.pop();
			for(int i = 0; i < N; ++ i)
			{
				if(c->son[i] == nullptr) continue;
				q.push(c->son[i]);
			}
			delete c;
		}
	}
};








} // namespace lz



#endif // TRIE_H