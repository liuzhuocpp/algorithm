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

#include <functional>
#include <iostream>
#include <utility>
namespace lz {

using std::function;
using std::cout;
using std::endl;

template<typename Map>
struct MapTraits
{
	using KeyType = typename Map::KeyType;
	using ValueType = typename Map::ValueType;
	using Reference = typename Map::Reference;
};

template<typename Key, typename Value, typename ReferenceType = Value&>
struct MapFacade
{
	using KeyType = Key;
	using ValueType = Value;
	using Reference = ReferenceType;
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
	explicit FunctionMap() = default;
	explicit FunctionMap(const UnaryFunction &f):f(&f) {}
	typename Base::ValueType
	operator[](typename Base::KeyType key) const
	{
		return (*f)(key);
	}
};
template<typename I,
		 typename Key = typename std::iterator_traits<I>::difference_type,
		 typename Value = typename  std::iterator_traits<I>::value_type>
class IteratorMap:public MapFacade<Key, Value>
{
	I i;
	using Base = MapFacade<Key, Value>;
public:
	IteratorMap() = default;
	IteratorMap(I i):i(i){}
	typename Base::ValueType
	operator[](typename Base::KeyType key) const
	{
		return i[key];
	}
};
template<typename FM, typename SM>
class ComposeMap:public MapFacade<typename MapTraits<FM>::KeyType, typename MapTraits<SM>::ValueType >
{
	FM fm;
	SM sm;
	using Base = MapFacade<typename MapTraits<FM>::KeyType, typename MapTraits<SM>::ValueType>;
public:
	explicit ComposeMap() = default;
	explicit ComposeMap(FM fm, SM sm):fm(fm), sm(sm){}
	typename Base::ValueType
	operator[](typename Base::KeyType key) const
	{
		return sm[fm[key]];
	}
};

template<typename FM, typename SM>
ComposeMap<FM, SM> makeComposeMap(FM fm, SM sm)
{
	return ComposeMap<FM, SM>(fm, sm);
}


//template<typename UnaryFunction>
//FunctionMap<UnaryFunction> makeFunctionMap(const UnaryFunction &f)
//{
//	return FunctionMap<UnaryFunction>(f);
//}


//template<typename T>
//class ClassMap
//{
//	T *t;
//public:
//	ClassMap() = default;
//
//    ClassMap(T *t):t(t){}
//
//    template<typename P>
//    auto operator[](P p) const ->decltype(t->*p)
//    {
//        return t->*p;
//    }
//};
//
//template<typename T>
//ClassMap<T> makeClassMap(T *t)
//{
//	return ClassMap<T>(t);
//}








}// namespace lz




#endif /* LZ_MAP_H_ */
