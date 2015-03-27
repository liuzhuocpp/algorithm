#ifndef TRIE_H
#define TRIE_H

#include <vector>
#include <queue>
#include <iostream>

namespace lz {

using std::queue;
using std::vector;
using std::cout;
using std::endl;

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
			Node *c = q.front();
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









template<int N>
struct AcAutomaton
{

	struct Node
	{
		Node *son[N];
		Node *next;
		int tail;
		Node():tail(0), next(nullptr)
		{
			for(int i = 0; i < N; ++ i) 
				son[i] = nullptr;
		}
	};
	Node *root;	
	vector<Node*> v;
	AcAutomaton():root(new Node()) { v.push_back(root); }
	
	template<typename RandomIterator>
	void insert(RandomIterator first, RandomIterator end)
	{
		RandomIterator s = first;
		int n = end - first;

		Node *top = root;
		for(int i = 0; i < n; ++ i)
		{
			if(top->son[s[i]] == nullptr)
			{
				top->son[s[i]] = new Node();
				v.push_back(top->son[s[i]]);
			}
			top = top->son[s[i]];
		}
		top->tail ++;
	}
	void build() // bfs
	{
		queue<Node*> q;
		root->next = root;
		q.push(root);
		while(!q.empty())
		{
			Node *u = q.front();
			q.pop();
			for(int i = 0; i < N; ++ i)
			{
				if(u->son[i] == nullptr)
				{
					if(u == root) u->son[i] = root;
					else u->son[i] = u->next->son[i];					
				}
				else 
				{
					if(u == root) u->son[i]->next = root;
					else u->son[i]->next = u->next->son[i];
					q.push(u->son[i]);
				}				
			}
		}
	}



	template<typename RandomIterator>	
	int query(RandomIterator first, RandomIterator end)
	{
		RandomIterator s = first;
		int n = end - first;

		Node *u = root;
		int ans = 0;
		for(int i = 0; i < n; ++ i)
		{
			// cout << "JJJ" << int(s[i]) << " " << u << endl;
			u = u->son[s[i]];
			Node *tp = u;
			for(;tp != root; tp = tp->next) 
			ans += tp->tail, tp->tail = 0;
		}
		return ans;
	}
	~AcAutomaton()
	{
		for(int i = 0; i < int(v.size()); ++ i) delete v[i];
	}
	

};






















} // namespace lz



#endif // TRIE_H