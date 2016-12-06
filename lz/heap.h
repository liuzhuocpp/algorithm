#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <algorithm>
#include <iostream>

#include "lz/map.h"
namespace lz {

//template<typename _RandomIterator, typename _KeyCompare, typename IndexMap>
//class Heap
//{
//public:
//	using RandomIterator = _RandomIterator;
//	using KeyCompare = _KeyCompare;
//	RandomIterator& begin()
//	{
//		return _begin;
//	}
//	RandomIterator& end()
//	{
//		return _end;
//	}
//	std::size_t& index(const std::iterator_traits<RandomIterator>::value_type & key)
//	{
//		return indexMap[key];
//	}
//	Heap(RandomIterator _b, RandomIterator _e):_begin(_b), _end(_e){}
//private:
//	RandomIterator _begin, _end;
//	IndexMap indexMap;
//};

namespace HeapPrivate {

	template<typename RandomIterator>
	struct BeforeKeyChange
	{
		void operator()(RandomIterator cntIterator, RandomIterator newIterator)
		{

		}
	};
}
template<typename RandomIterator,
		 typename BeforeKeyChange = HeapPrivate::BeforeKeyChange<RandomIterator>,
		 typename Less = std::less<typename std::iterator_traits<RandomIterator>::value_type> >
class HeapImplement
{
	using IndexType = typename std::iterator_traits<RandomIterator>::difference_type; // [0, n)
	using KeyType = typename std::iterator_traits<RandomIterator>::value_type;
	inline static IndexType p(IndexType x) { return ((x + 1) >> 1) - 1; }
	inline static IndexType ls(IndexType x) { return ((x + 1) << 1) - 1; }
	inline static IndexType rs(IndexType x) { return ls(x) + 1; }

public:
	static void make(RandomIterator begin, RandomIterator end)
	{
		for(RandomIterator it = begin; it != end; ++ it)
		{
			up(begin, end, it);
		}
	}
	static void push(RandomIterator begin, RandomIterator end)
	{
		up(begin, end, end - 1);
	}
	static void pop(RandomIterator begin, RandomIterator end)
	{
		BeforeKeyChange()(begin, end - 1);
		*begin = *(end - 1);
		down(begin, end - 1, begin);
	}
	static const KeyType& top(RandomIterator begin, RandomIterator end)
	{
		return *begin;
	}
	static void increase(RandomIterator begin, RandomIterator end, RandomIterator it)
	{
		up(begin, end, it);
	}

	static void decrease(RandomIterator begin, RandomIterator end, RandomIterator it)
	{
		down(begin, end, it);
	}
	static void update(RandomIterator begin, RandomIterator end, RandomIterator it)
	{
		IndexType u = it - begin;
		if(Less() (*(begin + p(u)), *it) )
		{
			up(begin, end, it);
		}
		else
		{
			down(begin, end, it);
		}
	}

	static void erase(RandomIterator begin, RandomIterator end, RandomIterator it)
	{
		IndexType u = it - begin;
		while(u != 0)
		{
			swapWithParent(begin, end, begin + u);
			u = p(u);
		}
		pop(begin, end);
	}

protected:
	static void swapWithParent(RandomIterator begin, RandomIterator end, RandomIterator it)
	{
		RandomIterator a = begin;
		IndexType u = it - begin;
		BeforeKeyChange()(a + u, a + p(u));
		BeforeKeyChange()(a + p(u), a + u);
		std::swap(a[u], a[p(u)] );
	}
	static void up(RandomIterator begin, RandomIterator end, RandomIterator it)
	{
		RandomIterator a = begin;
		IndexType n = end - begin, u = it - begin;
		while(u != 0)
		{
			if(Less()(a[u], a[p(u)]))
			{
				break;
			}
			swapWithParent(begin, end, a + u);
			u = p(u);
		}
	}
	static void down(RandomIterator begin, RandomIterator end, RandomIterator it)
	{
		RandomIterator a = begin;
		IndexType n = end - a, u = it - begin;
		while(ls(u) < n)
		{
			IndexType son = ls(u);
			if(Less()(a[son], a[rs(u)])) son = rs(u);
			if(Less()(a[u], a[son]))
			{
				swapWithParent(begin, end, a + son);
				u = son;
			}
			else break;
		}
	}
};


//template<typename T, typename Container = std::vector<T>, typename Compare = std::less<T> >
//class Heap
//{
//public:
//	using RandomIterator = _RandomIterator;
//	using KeyCompare = _KeyCompare;
//	using IndexType = typename std::iterator_traits<RandomIterator>::difference_type;
//	RandomIterator begin()
//	{
//		return _begin;
//	}
//	RandomIterator end()
//	{
//		return _end;
//	}
//	void beforeKeyChange(IndexType nowIndex, IndexType newIndex){}
//	Heap(RandomIterator _b, RandomIterator _e):_begin(_b), _end(_e){}
//private:
//	RandomIterator _begin, _end;
//};
//
//template<typename Heap>
//class HeapImplement
//{
//	using RandomIterator = typename Heap::RandomIterator;
//	using IndexType = typename std::iterator_traits<RandomIterator>::difference_type; // [0, n)
//	using KeyType = typename std::iterator_traits<RandomIterator>::value_type;
//	using Less = typename Heap::KeyCompare;
//	inline static IndexType p(IndexType x) { return ((x + 1) >> 1) - 1; }
//	inline static IndexType ls(IndexType x) { return ((x + 1) << 1) - 1; }
//	inline static IndexType rs(IndexType x) { return ls(x) + 1; }
//
//public:
//
//	static void make(Heap &heap)
//	{
//		RandomIterator it;
//		for(it = heap.begin(); it != heap.end(); ++ it)
//		{
//			up(heap, it);
//		}
//	}
//	static void push(Heap &heap)
//	{
//		RandomIterator end = heap.end();
//		up(heap, end);
//	}
//	static void pop(Heap &heap)
//	{
//		*heap.begin() = *--heap.end();
//		down(heap, heap.begin());
//	}
//	static const KeyType& top(Heap &heap)
//	{
//		return *heap.begin();
//	}
//
//protected:
//	static void swapWithParent(RandomIterator begin, RandomIterator end, IndexType u)
//	{
//		auto a = heap.begin();
//
////		std::swap(heap.indexMap(a[u]), heap.indexMap(a[p(u)]));
//		std::swap(a[u], a[p(u)] );
//	}
//	static void up(RandomIterator begin, RandomIterator end, IndexType u)
//	{
//		RandomIterator a = begin;
//		IndexType n = end - begin;
//		while(u != 0)
//		{
//			if(Less()(a[u], a[p(u)]))
//			{
//				break;
//			}
//			swapWithParent(begin, end, a + u);
//			u = p(u);
//		}
//	}
//	static void down(RandomIterator begin, RandomIterator end, IndexType u)
//	{
//		RandomIterator a = heap.begin();
//		IndexType n = heap.end() - a;
//		while(ls(u) < n)
//		{
//			IndexType son = ls(u);
//			if(Less()(a[son], a[rs(u)])) son = rs(u);
//			if(Less()(a[u], a[son]))
//			{
//				swapWithParent(begin, end, a + son);
//				u = son;
//			}
//			else break;
//		}
//	}
//};






//template<typename _RandomIterator, typename _KeyCompare, typename IndexMap>
//class IndexedHeap: public Heap<_RandomIterator, _KeyCompare>
//{
//	using IndexType = typename std::iterator_traits<RandomIterator>::difference_type; // [0, n)
//	using KeyValue =  typename std::iterator_traits<RandomIterator>::value_type;
//	IndexMap indexMap;
//public:
//	Index& index(const KeyValue &key)
//	{
//		return indexMap[key];
//	}
//};
//template<typename HeapImplement>
//class IndexedHeapImplement:public HeapImplement
//{
//	using Heap = typename HeapImplement::Heap;
//	static void swapWithParent(Heap &heap, IndexType u)
//	{
//		std::swap(heap.index(a[u]), heap.index(a[p(u)]));
//		HeapImplement::swapWithParent(heap, u);
//	}
//
//	static void up(Heap &heap, IndexType u)
//	{
//
//	}
//
//};





//using std::vector;
//using std::swap;
//using std::cout;
//using std::endl;
//
//template<typename Value, typename Less = std::less<Value> >
//class Heap
//{
//	inline static int p(int x) { return ((x + 1) >> 1) - 1; }
//	inline static int ls(int x) { return ((x + 1) << 1) - 1; }
//	inline static int rs(int x) { return ls(x) + 1; }
//	template<typename T> inline static int sz(const T &x) { return int(x.size()); }
//	template<typename T> inline static int last(const T &x) { return sz(x) - 1; }
//public:
//	Heap(){}
//	Heap(const vector<Value> &v)
//	{
//		for(int i = 0; i < sz(v); ++ i) push(v[i]);
//	}
//	inline int size() const { return a.size(); }
//	inline Value top() const { return a[0]; }
//	inline Value value(int i) const { return a[input[i]]; }
//	inline int inputIdToHeapId(int i) { return input[i]; }
//	inline int heapIdToInputId(int i) { return heap[i]; }
//
//	void clear()
//	{
//		a.clear();
//		input.clear();
//		heap.clear();
//	}
//	void pop()
//	{
//		swap(a.front(), a.back());
//		swap(input[heap[0]], input[heap[last(a)]]);
//		swap(heap[0], heap[last(a)]);
//		input[heap[last(a)]] = -1;
//		heap.pop_back();
//		a.pop_back();
//		down(0);
//	}
//	void push(const Value &val)
//	{
//		a.push_back(val);
//		input.push_back(last(a));
//		heap.push_back(last(input));
//		up(last(a));
//	}
//	void modify(int i, const Value &new_val)
//	{
//		if(less(new_val, a[input[i]]))
//		{
//			a[input[i]] = new_val;
//			down(input[i]);
//		}
//		else
//		{
//			a[input[i]] = new_val;
//			up(input[i]);
//		}
//	}
//private:
//	void up(int u)
//	{
//		for(;u != 0;)
//		{
//			if(less(a[p(u)], a[u]))
//			{
//				swap(a[p(u)], a[u]);
//				swap(input[heap[p(u)]], input[heap[u]]);
//				swap(heap[p(u)], heap[u]);
//				u = p(u);
//			}
//			else return ;
//		}
//	}
//	void down(int u)
//	{
//		for(;;)
//		{
//			int s = ls(u);
//			if(s >= sz(a)) return ;
//			if(rs(u) < sz(a) && less(a[s], a[rs(u)])) s = rs(u);
//			if(less(a[u], a[s]))
//			{
//				swap(a[u], a[s]);
//				swap(input[heap[u]], input[heap[s]]);
//				swap(heap[u], heap[s]);
//				u = s;
//			}
//			else return ;
//		}
//	}
//	vector<int> input, heap;
//	vector<Value> a;
//	Less less;
//
//};




} // namespace lz
#endif // HEAP_H
