#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <algorithm>
#include <iostream>

namespace lz
{

using std::vector;
using std::swap;
using std::cout;
using std::endl;

#define p(x) (((x + 1) >> 1) - 1)
#define ls(x) (((x + 1) << 1) - 1)
#define rs(x) (ls(x) + 1)
#define sz(x) (int(x.size()))
#define last(x) (sz(x) - 1)
template<typename Value, typename Less = std::less<Value> >
class Heap
{
public:
	Heap(){}
	Heap(const vector<Value> &v)
	{
		for(int i = 0; i < sz(v); ++ i) push(v[i]);
	}
	inline int size() const { return a.size(); }
	inline Value top() const { return a[0]; }
	inline Value value(int i) const { return a[input[i]]; }
	inline int inputIdToHeapId(int i) { return input[i]; }
	inline int heapIdToInputId(int i) { return heap[i]; }
	
	void clear()
	{
		a.clear();
		input.clear();
		heap.clear();
	}
	void pop()
	{
		swap(a.front(), a.back());
		swap(input[heap[0]], input[heap[last(a)]]);
		swap(heap[0], heap[last(a)]);
		input[heap[last(a)]] = -1;
		a.pop_back();
		down(0);
	}
	void push(const Value &val)
	{
		a.push_back(val);
		input.push_back(last(a));

		if(last(heap) < last(a)) heap.push_back(last(input));
		else heap[last(a)] = last(input);


		

		up(last(a));
	}
	void modify(int i, const Value &new_val)
	{
		if(less(new_val, a[input[i]])) 
		{
			a[input[i]] = new_val;
			down(input[i]);
		}
		else 
		{
			a[input[i]] = new_val;
			up(input[i]);
		}
	}




private:
	void up(int u)
	{
		for(;u != 0;)
		{
			if(less(a[p(u)], a[u])) 
			{
				swap(a[p(u)], a[u]);
				swap(input[heap[p(u)]], input[heap[u]]);
				swap(heap[p(u)], heap[u]);
				u = p(u);
			}
			else return ;
		}
	}
	void down(int u)
	{
		for(;;)
		{
			int s = ls(u);
			if(s >= sz(a)) return ;
			if(rs(u) < sz(a) && less(a[s], a[rs(u)])) s = rs(u);
			if(less(a[u], a[s])) 
			{
				swap(a[u], a[s]);
				swap(input[heap[u]], input[heap[s]]);
				swap(heap[u], heap[s]);
				u = s;
			}
			else return ;
		}
	}
	
	vector<int> input, heap;
	vector<Value> a;
	Less less;

};

#undef p
#undef ls
#undef rs
#undef sz
#undef last



} // namespace lz
#endif // HEAP_H