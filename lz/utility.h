#ifndef UTILITY_H
#define UTILITY_H

#include <functional>
#include <vector>
#include <deque>
#include <queue>
#include "lz/map.h"
// some little and mess functions

namespace lz {



/**

Buffer concept:
    A Buffer is something in which items can be put and removed.
    It does not require any particular ordering of how the items are stored or in what order they will appear when removed.

    using SizeType
    using KeyType
    SizeType size();
    bool empty();
    void pop();
    const KeyType& top();
    void push(const KeyType& key);

boost::queue
IndexableBuffer:
    model a Buffer
    bool contains(const KeyType &);
    void update(const KeyType&);
    void decrease(const KeyType&);
    void increase(const KeyType&);

 */



template<typename T, typename Container = std::deque<T>>
class Queue: public std::queue<T>
{

    using Base = std::queue<T>;

public:
    // using std::queue<T> constructors
    using Base::Base;
    using KeyType = typename Base::value_type;
    using SizeType =typename  Base::size_type;
    const KeyType& top() const
    {
        return this->front();
    }
};







/**

Marker concept:
    Mark some element. When a Marker constructed, all elements in this marker are not marked.


    using KeyType
    bool isMark(Element i) const;
    void mark(Element i);

 */
template<typename IndexMap>
class IndexMarker
{
    IndexMap indexMap;
    std::vector<bool> v;
public:
//    using Element = typename MapTraits<IndexMap>::KeyType;
    using KeyType = typename MapTraits<IndexMap>::KeyType;
    IndexMarker(const IndexMap& indexMap, std::size_t n):indexMap(indexMap), v(n, 0) {}
    bool isMark(const KeyType &i) const
    {
        return v[indexMap[i]];
    }
    void mark(const KeyType& i)
    {
        v[indexMap[i]] = true;
    }
};

template<typename IndexMap>
IndexMarker<IndexMap> makeIndexMarker(const IndexMap& indexMap, std::size_t n)
{
    return IndexMarker<IndexMap>(indexMap, n);
}

template<typename IndexMap>
auto calculateIndexMarker(const IndexMap& indexMap, std::size_t n)
    ->decltype(std::bind(makeIndexMarker<IndexMap>, indexMap, n))
{
    return std::bind(makeIndexMarker<IndexMap>, indexMap, n);
}



//template<typename IndexableHeap>
//class IndexableHeapAsMarkerForBFS
//{
//    IndexableHeap &heap;
//public:
//    using Element = typename IndexableHeap::KeyType;
//    IndexableHeapAsMarkerForBFS(IndexableHeap &heap):heap(heap){}
//    bool isMark(const Element &i) const
//    {
//        return heap.contains(i);
//    }
//    void mark(Element i)
//    {
//        // do nothing, because in BFS, heap.push will do this
//    }
//};





auto emptyFunction = [](auto...) {};






enum class DefaultColorType: unsigned char
{
    White,
    Gray,
    Black,
	Green,
	Red,
};

template<typename ColorType = DefaultColorType>
struct ColorTraits
{
	using Type = ColorType;
	static ColorType white() { return ColorType::White; }
	static ColorType gray() { return ColorType::Gray; }
	static ColorType black() { return ColorType::Black; }
	static ColorType green() { return ColorType::Green; }
	static ColorType red() { return ColorType::Red; }
};


template<typename Derived>
struct FacadeBase
{
protected:
	Derived& derived()
	{ return *static_cast<Derived*>(this); }

	Derived const& derived() const
	{ return *static_cast<Derived const*>(this); }
};

// requires operator==
template<typename Derived>
struct EqualityComparableFacade
{
	friend bool operator!=(const Derived &a, const Derived &b)
	{
		return !(a == b);
	}
};

// requires operator<
template<typename Derived>
struct LessThanComparableFacade
{
	friend bool operator>(const Derived &a, const Derived &b)
	{
		return b < a;
	}
	friend bool operator<=(const Derived &a, const Derived &b)
	{
		return !(b < a);
	}
	friend bool operator>=(const Derived &a, const Derived &b)
	{
		return !(a < b);
	}
};








template <class F1, class F2>
struct LambdaOverloadSet : F1, F2
{
	LambdaOverloadSet(F1 f1, F2 f2)
		: F1(f1), F2(f2)
	{

	}

	using F1::operator();
	using F2::operator();
};

template <class F1, class F2>
LambdaOverloadSet<F1, F2> lambdaOverload(F1 f1, F2 f2)
{
	return LambdaOverloadSet<F1, F2>(f1, f2);
}









/** 
 * @prama T should be integer
 * @prama x  should be no less than 0
 * @return number of bits in the minimal two's-complement representation of this integer
 */
template<typename T>
int integerBitLength(T x)
{
    if(x == 0) return 1;
    int r = 0;
    for(;x > 0; x >>= 1)
    {
        r ++;
    }
    return r;
}

/** 
 * @prama T should be integer
 * @return a^b % c
 */

template<typename T>
T modPow(T a, T b, const T &c)
{
    T r = 1;
    while(b > 0)
    {
        if(b & 1) r = r * b % c;
        a = a * a % c;
        b >>= 1;
    }
    return r;
}





} // namespace lz

#endif // UTILITY_H
