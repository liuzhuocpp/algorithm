
//#include <iostream>
//#include <cstdio>
#include <bits/stdc++.h>

#include <lz/treap.h>


using namespace std;
using namespace lz;

struct TreapNode: lz::TreapNodeFacade<TreapNode, int >
{
	TreapNode(const int &key = 0, const PriorityType & _p = PriorityType()):
		lz::TreapNodeFacade<TreapNode, int >(key, _p){}
};
struct Tr:public lz::Treap<TreapNode>
{

};

int main()
{
	Tr bst;
	using I = TreapImplement<Tr>;

	I::insert(bst, new TreapNode(100, 33));
	I::insert(bst, new TreapNode(300, 11));
	I::insert(bst, new TreapNode(200, 5324));
	I::insert(bst, new TreapNode(312, 123));

	I::bfs(bst);




	return 0;
}




















