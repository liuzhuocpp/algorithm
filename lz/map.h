/*
 * map.h
 *
 *  Created on: 2015Äê8ÔÂ5ÈÕ
 *      Author: LZ
 */

/*
 * Map concept is similar to boost property map library,
 * but Map concept is sample. It only contain operator[] for
 * a Map object.
 */
#ifndef LZ_MAP_H_
#define LZ_MAP_H_

//#include <iostream>

namespace lz {

//using std::cout;
//using std::endl;

template<typename Map>
struct MapTraits
{
	using KeyType = typename Map::KeyType;
	using ValueType = typename Map::ValueType;
};

template<typename Key, typename Value, typename ReferenceType = Value&>
struct MapFacade
{
	using KeyType = Key;
	using ValueType = Value;
};


template<typename Key>
struct IdentityMap:public MapFacade<Key, Key>
{
	Key operator[](Key key) const
	{
		return key;
	}
};

template<typename UnaryFunction, typename Key, typename Value>
class FunctionMap:public MapFacade<Key, Value>
{
	const UnaryFunction* f = nullptr;
	using Base = MapFacade<Key, Value>;
public:
	const UnaryFunction* function(){ return f;}

	explicit FunctionMap() = default;
	explicit FunctionMap(const UnaryFunction &f):f(&f) {}
	typename Base::ValueType operator[](typename Base::KeyType key) const
	{
		return (*f)(key);
	}
};
template<typename I,
		 typename Key = typename std::iterator_traits<I>::difference_type,
		 typename Value = decltype(I()[Key()]) >
class IteratorMap:public MapFacade<Key, Value>
{
	I i;
	using Base = MapFacade<Key, Value>;
public:
	IteratorMap() = default;
	IteratorMap(I i):i(i){}

	using Iterator = I;
	Iterator iterator() const { return i;}

	typename Base::ValueType operator[](typename Base::KeyType key)
	{
		return i[key];
	}
};
template<typename I>
IteratorMap<I> makeIteratorMap(I i)
{
	return IteratorMap<I>(i);
}


template<typename FM, typename SM>
class ComposeMap:public MapFacade<typename MapTraits<FM>::KeyType, typename MapTraits<SM>::ValueType >
{
	FM fm;
	SM sm;
	using Base = MapFacade<typename MapTraits<FM>::KeyType, typename MapTraits<SM>::ValueType>;
public:
	using FirstMap = FM;
	using SecondMap = SM;

	explicit ComposeMap() = default;
	explicit ComposeMap(FM fm, SM sm):fm(fm), sm(sm){}

	FirstMap firstMap() const { return fm; }
	SecondMap secondMap() const { return sm; }

	typename Base::ValueType operator[](typename Base::KeyType key) const
	{
		return sm[fm[key]];
	}
};

template<typename FM, typename SM>
ComposeMap<FM, SM> makeComposeMap(FM fm, SM sm)
{
	return ComposeMap<FM, SM>(fm, sm);
}







}// namespace lz




#endif /* LZ_MAP_H_ */

