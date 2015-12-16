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
#include <memory>
#include <type_traits>
namespace lz {

//using std::cout;
//using std::endl;

template<typename Map>
struct MapTraits
{
	using KeyType = typename Map::KeyType;
	using ValueType = typename Map::ValueType;

//	using DecayValueType = std::decay_t
};

template<typename Key, typename Value>
struct MapFacade
{
	using KeyType = Key;
	using ValueType = Value;
};


//template<typename UniqueArray>
//struct UniqueArrayMap: public MapFacade<std::ptrdiff_t, typename UniqueArray::element_type&>
//{
//	UniqueArrayMap(UniqueArray &&u):u(move(u))
//	{
//
//	}
//	typename UniqueArray::element_type& operator[](ptrdiff_t d) const
//	{
//		return u[d];
//	}
//private:
//	UniqueArray u;
//};
//
//
//template<typename UniqueArray>
//UniqueArrayMap<typename std::remove_reference<UniqueArray>::type> makeUniqueArrayMap(UniqueArray && u)
//{
//	return UniqueArrayMap<typename std::remove_reference<UniqueArray>::type>(move(u));
//}















template<typename _ValueType>
struct SharedArrayMap: public MapFacade<std::ptrdiff_t, _ValueType>
{
private:
	std::shared_ptr<_ValueType> sp;
public:
	SharedArrayMap(std::shared_ptr<_ValueType> sp):sp(sp){}
	SharedArrayMap() :sp(nullptr) {}

	SharedArrayMap(size_t n)
	:sp(new _ValueType[n], std::default_delete<_ValueType[]>())  { }

//	_ValueType&  operator[](ptrdiff_t d) const

	auto operator[](ptrdiff_t d) const ->decltype(sp.get()[d])
	{
		return	sp.get()[d];
	}
};


//template<typename _ValueType>
//SharedArrayMap<_ValueType> makeUniqueArrayMap(size_t n)
//{
//
//}






template<typename Key>
struct IdentityMap:public MapFacade<Key, Key>
{
	const Key& operator[](const Key &key) const
	{
		return key;
	}
};

//template<typename UnaryFunction, typename Key, typename Value>
//class FunctionMap:public MapFacade<Key, Value>
//{
//	const UnaryFunction* f = nullptr;
//	using Base = MapFacade<Key, Value>;
//public:
//	const UnaryFunction* function(){ return f;}
//
//	explicit FunctionMap() = default;
//	explicit FunctionMap(const UnaryFunction &f):f(&f) {}
//	typename Base::ValueType operator[](typename Base::KeyType key) const
//	{
//		return (*f)(key);
//	}
//};
template<typename I
//,
//		 typename Key = typename std::iterator_traits<I>::difference_type,
//		 typename Value = decltype(I()[Key()])
		 >
class IteratorMap:public MapFacade<
//Key
typename std::iterator_traits<I>::difference_type
,
typename std::iterator_traits<I>::value_type
//Value

>
{
	I i;
//	using Base = MapFacade<Key, Value>;
public:
	IteratorMap() = default;
	IteratorMap(I i):i(i){}

	using Iterator = I;
	Iterator iterator() const { return i;}

//	template<typename Key>
	auto operator[](typename std::iterator_traits<I>::difference_type key) const ->decltype(i[key])
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
//	explicit ComposeMap(FM &&fm, SM &&sm):fm(std::move(fm)), sm(std::move(sm)) { }
//	explicit ComposeMap(FM &&fm, const SM &sm):fm(std::move(fm)), sm(sm) {}
//	explicit ComposeMap(const FM &fm, SM &&sm):fm(fm), sm(std::move(sm)) {}
	explicit ComposeMap(const FM &fm,  const SM &sm):fm(fm), sm(sm) {}


	FirstMap firstMap() const { return fm; }
	SecondMap secondMap() const { return sm; }

	template<typename Key>
	auto operator[](Key && key) const ->decltype(sm[fm[key]])
	{
		return sm[fm[key]];
	}
};

template<typename FM, typename SM>
//ComposeMap<typename std::remove_reference<FM>::type,
//  	  	   typename std::remove_reference<SM>::type>
ComposeMap<FM, SM>
makeComposeMap(const FM &fm, const SM &sm)

{
	return ComposeMap<FM, SM>(fm, sm);
//	return ComposeMap<typename std::remove_reference<FM>::type,
//					  typename std::remove_reference<SM>::type>(fm, sm) ;
}







}// namespace lz




#endif /* LZ_MAP_H_ */

