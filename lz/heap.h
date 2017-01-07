#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <iterator>

#include "lz/map.h"
#include "lz/utility.h"
namespace lz {



//using namespace std;

namespace HeapPrivate {


template<typename IndexType>
inline IndexType p(IndexType x)
{
    return ((x + 1) >> 1) - 1;
}

template<typename IndexType>
inline IndexType ls(IndexType x)
{
    return ((x + 1) << 1) - 1;
}

template<typename IndexType>
inline IndexType rs(IndexType x)
{
    return ls(x) + 1;
}

template<typename RandomIterator, typename Less, typename BeforeKeyChange>
void swapWithParent(RandomIterator begin, RandomIterator end, RandomIterator it,
        const Less &less, const BeforeKeyChange &beforeKeyChange)
{
    using IndexType = typename std::iterator_traits<RandomIterator>::difference_type;
    RandomIterator a = begin;
    IndexType u = it - begin;
    beforeKeyChange(a + u, a[p(u)]);
    beforeKeyChange(a + p(u), a[u]);
    std::swap(a[u], a[p(u)]);
}

template<typename RandomIterator, typename Less, typename BeforeKeyChange>
void up(RandomIterator begin, RandomIterator end, RandomIterator it,
        const Less &less, const BeforeKeyChange &beforeKeyChange)
{
    using IndexType = typename std::iterator_traits<RandomIterator>::difference_type;
    RandomIterator a = begin;
    IndexType n = end - begin, u = it - begin;
    while (u != 0)
    {
        if (less(a[u], a[p(u)]))
        {
            break;
        }
        swapWithParent(begin, end, a + u, less, beforeKeyChange);
        u = p(u);
    }
}

template<typename RandomIterator, typename Less, typename BeforeKeyChange>
void down(RandomIterator begin, RandomIterator end, RandomIterator it,
        const Less &less , const BeforeKeyChange &beforeKeyChange)
{
    using IndexType = typename std::iterator_traits<RandomIterator>::difference_type;
    RandomIterator a = begin;
    IndexType n = end - a, u = it - begin;
    while (ls(u) < n)
    {
        IndexType son = ls(u);
        if (less(a[son], a[rs(u)])) son = rs(u);
        if (less(a[u], a[son]))
        {
            swapWithParent(begin, end, a + son, less, beforeKeyChange);
            u = son;
        }
        else break;
    }
}





} // namespace HeapPrivate



template<typename RandomIterator, typename Less, typename BeforeKeyChange>
void makeHeap(RandomIterator begin, RandomIterator end, const Less &less = Less(),
        const BeforeKeyChange &beforeKeyChange = BeforeKeyChange())
{
    for (RandomIterator it = begin; it != end; ++it)
    {
        HeapPrivate::up(begin, end, it, less, beforeKeyChange);
    }
}

template<typename RandomIterator,
         typename Less = std::less<typename std::iterator_traits<RandomIterator>::difference_type>,
         typename BeforeKeyChange = decltype(emptyFunction)>
void pushHeap(RandomIterator begin, RandomIterator end, const Less &less = Less(),
        const BeforeKeyChange &beforeKeyChange = BeforeKeyChange())
{
    HeapPrivate::up(begin, end, end - 1, less, beforeKeyChange);
}

template<typename RandomIterator,
         typename Less = std::less<typename std::iterator_traits<RandomIterator>::difference_type>,
         typename BeforeKeyChange = decltype(emptyFunction)>
void popHeap(RandomIterator begin, RandomIterator end, const Less &less = Less(),
        const BeforeKeyChange &beforeKeyChange = BeforeKeyChange())
{
    if(end - begin <= 1) return ;
    beforeKeyChange(begin, *(end - 1));
    *begin = *(end - 1);
    HeapPrivate::down(begin, end - 1, begin, less, beforeKeyChange);
}


template<typename RandomIterator,
         typename Less = std::less<typename std::iterator_traits<RandomIterator>::difference_type>,
         typename BeforeKeyChange = decltype(emptyFunction)>
void increaseHeap(RandomIterator begin, RandomIterator end, RandomIterator it,
        const Less &less = Less(), const BeforeKeyChange &beforeKeyChange = BeforeKeyChange())
{
    HeapPrivate::up(begin, end, it, less, beforeKeyChange);
}

template<typename RandomIterator,
         typename Less = std::less<typename std::iterator_traits<RandomIterator>::difference_type>,
         typename BeforeKeyChange = decltype(emptyFunction)>
void decreaseHeap(RandomIterator begin, RandomIterator end, RandomIterator it,
        const Less &less = Less(), const BeforeKeyChange &beforeKeyChange = BeforeKeyChange())
{
    HeapPrivate::down(begin, end, it, less, beforeKeyChange);
}

template<typename RandomIterator,
         typename Less = std::less<typename std::iterator_traits<RandomIterator>::difference_type>,
         typename BeforeKeyChange = decltype(emptyFunction)>
void updateHeap(RandomIterator begin, RandomIterator end, RandomIterator it,
        const Less &less = Less(), const BeforeKeyChange &beforeKeyChange = BeforeKeyChange())
{
    using IndexType = typename std::iterator_traits<RandomIterator>::difference_type;
    IndexType u = it - begin;
    if (less(*(begin + p(u)), *it))
    {
        HeapPrivate::up(begin, end, it, less, beforeKeyChange);
    }
    else
    {
        HeapPrivate::down(begin, end, it, less, beforeKeyChange);
    }
}

template<typename RandomIterator,
         typename Less = std::less<typename std::iterator_traits<RandomIterator>::difference_type>,
         typename BeforeKeyChange = decltype(emptyFunction)>
void eraseHeap(RandomIterator begin, RandomIterator end, RandomIterator it,
        const Less &less = Less(), const BeforeKeyChange &beforeKeyChange = BeforeKeyChange())
{
    using IndexType = typename std::iterator_traits<RandomIterator>::difference_type;
    IndexType u = it - begin;
    while (u != 0)
    {
        HeapPrivate::swapWithParent(begin, end, begin + u, less, beforeKeyChange);
        u = p(u);
    }
    popHeap(begin, end, less, beforeKeyChange);
}




template<typename T, typename Less = std::less<T>, typename Container = std::vector<T> >
class Heap
{
    using RandomIterator = typename Container::iterator;
protected:
    Container c;
    Less less;
public:
    using KeyType = T;
    using KeyCompare = Less;

    Heap(const Less &less, const Container& c) :
            less(less), c(c)
    {
    }
    Heap(const Less &less = Less(), Container&& c = Container()) :
            less(less), c(std::move(c))
    {
    }

    void push(const KeyType &key)
    {
        c.push_back(key);
        pushHeap(c.begin(), c.end(), less);
//        HeapImplement<RandomIterator, Less>::push(c.begin(), c.end());
    }
    void pop()
    {
//        HeapImplement<RandomIterator, Less>::pop(c.begin(), c.end());
        popHeap(c.begin(), c.end(), less);
        c.pop_back();
    }
    const KeyType& top() const
    {
        return *c.begin();
    }
    size_t size() const
    {
        return c.size();
    }
    bool isEmpty() const
    {
        return c.empty();
    }

};


/**

Heap concept:
    using KeyType
    void empty(const KeyType& key);
    void size(const KeyType& key);
    void push(const KeyType& key);
    void pop(const KeyType& key);
    const KeyType& top();


IndexableHeap concept:
    Heap concept
    void decrease(const KeyType& key);
    void increase(const KeyType& key);
    void update(const KeyType& key);
    void contains(const KeyType& key);

 */




/**

IndexableHeap concept:
    Heap concept
    void decrease(const KeyType& key);
    void increase(const KeyType& key);
    void update(const KeyType& key);
    void contains(const KeyType& key);
 */


template<typename T,
         typename IndexMap,
         typename MapTraits<IndexMap>::ValueType nullIndex = (typename MapTraits<IndexMap>::ValueType)-1,
         typename Less = std::less<T>,
         typename Container = std::vector<T>>
class IndexableHeap
{
private:
    IndexMap indexMap;
    Less less;
    Container c;

    using RandomIterator = typename Container::iterator;
    struct BeforeKeyChange
    {
        Container& c;
        IndexMap& indexMap;
        void operator()(RandomIterator cnt, const T &key) const
        {
            indexMap[*cnt] = nullIndex;
            indexMap[key] = cnt - c.begin();
        }
    };
public:
    using KeyType = T;
    using KeyCompare = Less;
    using SizeType = typename Container::size_type;
    IndexableHeap(IndexMap indexMap, const Less& less , const Container &c) :
            indexMap(indexMap), less(less), c(c)
    {
    }

    IndexableHeap(IndexMap indexMap, const Less& less= Less(), Container &&c = Container()) :
            indexMap(indexMap), less(less), c(std::move(c))
    {
    }

    void push(const KeyType &key)
    {
        indexMap[key] = c.size();
        c.push_back(key);
        pushHeap(c.begin(), c.end(), less, BeforeKeyChange { c, indexMap });
    }

    void pop()
    {
        indexMap[c[0]] = nullIndex;
        popHeap(c.begin(), c.end(), less, BeforeKeyChange { c, indexMap });
        c.pop_back();
    }

    const KeyType& top() const
    {
        return *c.begin();
    }

    SizeType size() const
    {
        return c.size();
    }

    bool empty() const
    {
        return c.empty();
    }

    bool contains(const KeyType& indexKey) const
    {
        return indexMap[indexKey] != nullIndex;
    }

    void decrease(const KeyType& indexKey)
    {
        decreaseHeap(c.begin(), c.end(), c.begin() + indexMap[indexKey], less,
                BeforeKeyChange {c, indexMap });
    }

    void increase(const KeyType& indexKey)
    {
        increaseHeap(c.begin(), c.end(), c.begin() + indexMap[indexKey], less,
                BeforeKeyChange { c, indexMap });
    }

    void update(const KeyType& indexKey)
    {
        updateHeap(c.begin(), c.end(), c.begin() + indexMap[indexKey], less,
                        BeforeKeyChange { c, indexMap });
    }


    void decrease(const KeyType& indexKey, const KeyType& newKey)
    {
        set(indexKey, newKey);
        decreaseHeap(c.begin(), c.end(), c.begin() + indexMap[newKey], less,
                        BeforeKeyChange {c, indexMap });
    }

    void increase(const KeyType& indexKey, const KeyType& newKey)
    {
        set(indexKey, newKey);
        increaseHeap(c.begin(), c.end(), c.begin() + indexMap[newKey], less,
                BeforeKeyChange {c, indexMap });
    }
    void update(const KeyType& indexKey, const KeyType& newKey)
    {
        set(indexKey, newKey);
        updateHeap(c.begin(), c.end(), c.begin() + indexMap[newKey], less,
                BeforeKeyChange {c, indexMap });
    }
    void out() const
    {
        std::cout << "[";
        for(int i = 0; i < c.size(); ++ i)
        {
            std::cout << c[i] << " (" <<  indexMap[c[i]] << "), ";
        }
        std::cout << "]\n";

    }

private:
    void set(const KeyType& indexKey, const KeyType& newKey)
    {
        SizeType index = indexMap[indexKey];
        indexMap[indexKey] = nullIndex;
        c[index] = newKey;
        indexMap[newKey] = index;
    }

};


//template<typename RandomIterator, typename Less = std::less<
//        typename std::iterator_traits<RandomIterator>::value_type>,
//        typename BeforeKeyChange = HeapPrivate::BeforeKeyChange<RandomIterator> >
//class HeapImplement
//{
//    using IndexType = typename std::iterator_traits<RandomIterator>::difference_type; // [0, n)
//    inline static IndexType p(IndexType x)
//    {
//        return ((x + 1) >> 1) - 1;
//    }
//    inline static IndexType ls(IndexType x)
//    {
//        return ((x + 1) << 1) - 1;
//    }
//    inline static IndexType rs(IndexType x)
//    {
//        return ls(x) + 1;
//    }
//
//public:
//    static void make(RandomIterator begin, RandomIterator end, const Less &less = Less(),
//            const BeforeKeyChange &beforeKeyChange = BeforeKeyChange())
//    {
//        for (RandomIterator it = begin; it != end; ++it)
//        {
//            up(begin, end, it, less, beforeKeyChange);
//        }
//    }
//    static void push(RandomIterator begin, RandomIterator end, const Less &less = Less(),
//            const BeforeKeyChange &beforeKeyChange = BeforeKeyChange())
//    {
//        up(begin, end, end - 1, less, beforeKeyChange);
//    }
//    static void pop(RandomIterator begin, RandomIterator end, const Less &less = Less(),
//            const BeforeKeyChange &beforeKeyChange = BeforeKeyChange())
//    {
//        if(end - begin <= 1) return ;
//        beforeKeyChange(begin, *(end - 1));
//        *begin = *(end - 1);
//        down(begin, end - 1, begin, less, beforeKeyChange);
//    }
//    static void increase(RandomIterator begin, RandomIterator end, RandomIterator it,
//            const Less &less = Less(), const BeforeKeyChange &beforeKeyChange = BeforeKeyChange())
//    {
//        up(begin, end, it, less, beforeKeyChange);
//    }
//
//    static void decrease(RandomIterator begin, RandomIterator end, RandomIterator it,
//            const Less &less = Less(), const BeforeKeyChange &beforeKeyChange = BeforeKeyChange())
//    {
//        down(begin, end, it, less, beforeKeyChange);
//    }
//    static void update(RandomIterator begin, RandomIterator end, RandomIterator it,
//            const Less &less = Less(), const BeforeKeyChange &beforeKeyChange = BeforeKeyChange())
//    {
//        IndexType u = it - begin;
//        if (less(*(begin + p(u)), *it))
//        {
//            up(begin, end, it, less, beforeKeyChange);
//        }
//        else
//        {
//            down(begin, end, it, less, beforeKeyChange);
//        }
//    }
//
//    static void erase(RandomIterator begin, RandomIterator end, RandomIterator it,
//            const Less &less = Less(), const BeforeKeyChange &beforeKeyChange = BeforeKeyChange())
//    {
//        IndexType u = it - begin;
//        while (u != 0)
//        {
//            swapWithParent(begin, end, begin + u, less, beforeKeyChange);
//            u = p(u);
//        }
//        pop(begin, end, less, beforeKeyChange);
//    }
//
//protected:
//    static void swapWithParent(RandomIterator begin, RandomIterator end, RandomIterator it,
//            const Less &less = Less(), const BeforeKeyChange &beforeKeyChange = BeforeKeyChange())
//    {
//        RandomIterator a = begin;
//        IndexType u = it - begin;
//        beforeKeyChange(a + u, a[p(u)]);
//        beforeKeyChange(a + p(u), a[u]);
//        std::swap(a[u], a[p(u)]);
//    }
//    static void up(RandomIterator begin, RandomIterator end, RandomIterator it, const Less &less =
//            Less(), const BeforeKeyChange &beforeKeyChange = BeforeKeyChange())
//    {
//        RandomIterator a = begin;
//        IndexType n = end - begin, u = it - begin;
//        while (u != 0)
//        {
//            if (less(a[u], a[p(u)]))
//            {
//                break;
//            }
//            swapWithParent(begin, end, a + u, less, beforeKeyChange);
//            u = p(u);
//        }
//    }
//    static void down(RandomIterator begin, RandomIterator end, RandomIterator it, const Less &less =
//            Less(), const BeforeKeyChange &beforeKeyChange = BeforeKeyChange())
//    {
//        RandomIterator a = begin;
//        IndexType n = end - a, u = it - begin;
//        while (ls(u) < n)
//        {
//            IndexType son = ls(u);
//            if (less(a[son], a[rs(u)])) son = rs(u);
//            if (less(a[u], a[son]))
//            {
//                swapWithParent(begin, end, a + son, less, beforeKeyChange);
//                u = son;
//            }
//            else break;
//        }
//    }
//};


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

}// namespace lz
#endif // HEAP_H
