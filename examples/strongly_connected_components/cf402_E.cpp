/*
 * cf402_E.cpp
 *
 *  Created on: 2015年11月24日
 *      Author: LZ
 *
 *
 * URL:     http://codeforces.com/contest/402/problem/E *
 */



#pragma GCC diagnostic error "-std=c++14"

#include <iostream>
#include <cstdio>
#include <cstring>
#include <tuple>
#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H


#include <vector>
#include <tuple>
#include <algorithm>


#ifndef PORPERTY_H
#define PORPERTY_H




#include <iostream>
#include <typeinfo>
#include <vector>

#include <type_traits>
#include <tuple>
namespace lz {

using std::integral_constant;
using std::cout;
using std::endl;



struct NoProperty {
	using Tag = NoProperty;
	using ValueType = NoProperty;
	using NextProperty = NoProperty;

	NoProperty() = default;

protected:
    template<typename ...Args, size_t N>
    NoProperty(const std::tuple<Args...> &tp, std::integral_constant<size_t, N> t)
	{
	}


};


    namespace PropertyPrivate {

        template<typename P>
        struct CountProperty
        : public integral_constant<size_t, CountProperty<typename P::NextProperty>::value + 1>
        { };

        template<>
        struct CountProperty<NoProperty>
        : public integral_constant<size_t, 0>
        { };

        template<typename... T>
        struct CountVariadic;

        template<typename Head, typename...T>
        struct CountVariadic<Head, T...>
        : public integral_constant< size_t, CountVariadic<T...>::value + 1>
        { };

        template<>
        struct CountVariadic<>
        : public integral_constant<size_t, 0>
        { };


        template<typename QueryTag, typename Tag, typename Property>
        struct Get;


        template <typename Property, typename CntTag, typename Tuple, size_t CntId>
		struct AssignTuple;

        template<typename Property, typename Tag, typename QueryTag>
        struct GetType;

    } // namespace PropertyPrivate


template<typename P>
struct PropertySize:public std::integral_constant<
	size_t, PropertySize<typename P::NextProperty>::value + 1> {};

template<>
struct PropertySize<NoProperty>
: public integral_constant<size_t, 0> { };


template<typename _Tag, typename _ValueType, typename _NextProperty = NoProperty >
struct Property: public _NextProperty
{
    template<typename QueryTag, typename Tag, typename Property>
    friend struct PropertyPrivate::Get;

    template <typename Property, typename CntTag, typename Tuple, size_t CntId>
    friend struct PropertyPrivate::AssignTuple;

    using Tag = _Tag;
    using ValueType = _ValueType;
    using NextProperty = _NextProperty;

    template<typename QueryTag>
    using QueryValueType = typename PropertyPrivate::GetType<Property, Tag, QueryTag>::ValueType;

    template<typename QueryTag>
    using QueryPropertyType = typename PropertyPrivate::GetType<Property, Tag, QueryTag>::PropertyType;

    Property() = default;

//    Property(const ValueType &value, const NextProperty & np):
//    	NextProperty(np),m_value(value){}

    Property(ValueType &&value, const NextProperty & np):
		NextProperty(np), m_value(value){}

protected:
    template<typename ...Args, size_t N>
    Property(const std::tuple<Args...> &tp, std::integral_constant<size_t, N> t):
		m_value(std::get<N>(tp)), NextProperty(tp, std::integral_constant<size_t, N + 1>())
	{

	}
public:
    template<typename ...Args>
	Property( const std::tuple<Args...> &tp):
		Property(tp, std::integral_constant<size_t, 0>())
	{

	}



//    template <class... UTypes>
//    Property(const std::tuple<UTypes...>& tp)
//	{
//    	PropertyPrivate::AssignTuple<Property, Tag, std::tuple<UTypes...>, 0>::apply(*this, tp);
//	}

//    template <class... UTypes>
//    Property(std::tuple<UTypes...>&& tp)
//	{
//    	PropertyPrivate::AssignTuple<Property, Tag, std::tuple<UTypes...>, 0>::apply(*this, tp);
//	}

//    template <class... UTypes>
//    Property& operator=(const std::tuple<UTypes...>& tp)
//	{
//    	PropertyPrivate::AssignTuple<Property, Tag, std::tuple<UTypes...>, 0>::apply(*this, tp);
//	}
//
//    template <class... UTypes>
//    Property& operator=(std::tuple<UTypes...>&& tp)
//	{
//    	PropertyPrivate::AssignTuple<Property, Tag, std::tuple<UTypes...>, 0>::apply(*this, tp);
//	}


    template<typename QueryTag>
    QueryValueType<QueryTag>& operator[](QueryTag tag)
	{
    	using Base = QueryPropertyType<QueryTag>;
    	return this->Base::m_value;
	}
    template<typename QueryTag>
	const QueryValueType<QueryTag>& operator[](QueryTag tag) const
	{
		using Base = QueryPropertyType<QueryTag>;
		return this->Base::m_value;
	}

    template<typename QueryTag>
    constexpr bool contains(QueryTag tag) const
    {
    	return !std::is_same<decltype((*this)[tag]), NoProperty>::value;
    }

protected:
    ValueType m_value;

};





    namespace PropertyPrivate {

	template<typename Property, typename Tag, typename QueryTag>
	struct GetType
	{
	private:
		using NextProperty = typename Property::NextProperty  ;
		using NextTag = typename NextProperty::Tag;
	public:
		using ValueType = typename GetType<NextProperty, NextTag, QueryTag>::ValueType;
		using PropertyType = typename GetType<NextProperty, NextTag, QueryTag>::PropertyType;
	};

	template<typename Property, typename QueryTag>
	struct GetType<Property, QueryTag, QueryTag>
	{
		using ValueType = typename Property::ValueType;
		using PropertyType = Property;
	};

	template<typename Property, typename QueryTag>
	struct GetType<Property, NoProperty, QueryTag>
	{
		using ValueType = NoProperty;
		using PropertyType = NoProperty;
	};





	template <typename Property, typename Tag, typename Tuple, size_t Id>
	struct AssignTuple
	{
		static void apply(Property &p, const Tuple &t)
		{
//			typename std::tuple_element<Id, Tuple>::type x = std::get<Id>(t);

			p[Tag()] = std::get<Id>(t);

			AssignTuple<typename Property::NextProperty,
						typename Property::NextProperty::Tag,
						Tuple,
						Id + 1>::apply(p, t);
		}
		static void apply(Property &p, Tuple &&t)
		{
//			 typename std::tuple_element<Id, Tuple>::type x = std::get<Id>(t);

			 p[Tag()] = std::get<Id>(t);

			 AssignTuple<typename Property::NextProperty,
						 typename Property::NextProperty::Tag,
						 Tuple,
						 Id + 1>::apply(p, t);
		}

	};

	template <typename Property, typename Tuple, size_t CntId>
	struct AssignTuple<Property, NoProperty, Tuple, CntId>
	{
		static void apply(Property &p, const Tuple &t)
		{

		}
	};












    } // namespace PropertyPrivate





//template<typename Tag, typename ValueType, typename NextProperty, typename QueryTag>
//auto get(Property<Tag, ValueType, NextProperty> & p, QueryTag tag)
//-> decltype(PropertyPrivate::Get<Property<Tag, ValueType, NextProperty>, Tag, QueryTag>::get(p))
//{
//    return PropertyPrivate::Get<Property<Tag, ValueType, NextProperty>, Tag, QueryTag>::get(p);
//}
//
//template<typename Tag, typename ValueType, typename NextProperty, typename QueryTag>
//auto get(const Property<Tag, ValueType, NextProperty> & p, QueryTag tag)
//-> decltype(PropertyPrivate::Get<Property<Tag, ValueType, NextProperty>, Tag, QueryTag>::get(p))
//{
//    return PropertyPrivate::Get<Property<Tag, ValueType, NextProperty>, Tag, QueryTag>::get(p);
//}






template<typename P>
struct ChooseDefineProperties
{
	P properties;
	void set(const P &_p) { properties = _p; }
};
template<> struct ChooseDefineProperties<NoProperty>
{
	void set(const NoProperty &_p) {  }
};





















} // namepspace lz






#endif // PORPERTY_H


#ifndef GRAPH_UTILITY_H
#define GRAPH_UTILITY_H
/*
 * this header file define some base classes and functions for graph
 */
//#include "lz/property.h"

//#include "lz/utility.h"
/*
 * map.h
 *
 *  Created on: 2015年8月5日
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
namespace lz {

//using std::cout;
//using std::endl;

template<typename Map>
struct MapTraits
{
	using KeyType = typename Map::KeyType;
	using ValueType = typename Map::ValueType;
};

template<typename Key, typename Value>
struct MapFacade
{
	using KeyType = Key;
	using ValueType = Value;
};


template<typename UniqueArray>
struct UniqueArrayMap: public MapFacade<std::ptrdiff_t, typename UniqueArray::element_type&>
{
	UniqueArrayMap(UniqueArray &&u):u(move(u))
	{

	}
	typename UniqueArray::element_type& operator[](ptrdiff_t d) const
	{
		return u[d];
	}
private:
	UniqueArray u;
};


template<typename UniqueArray>
UniqueArrayMap<typename std::remove_reference<UniqueArray>::type> makeUniqueArrayMap(UniqueArray && u)
{
	return UniqueArrayMap<typename std::remove_reference<UniqueArray>::type>(move(u));
}















template<typename _ValueType>
struct SharedArrayMap: public MapFacade<std::ptrdiff_t, _ValueType&>
{
	SharedArrayMap(std::shared_ptr<_ValueType> sp):sp(sp){}
	SharedArrayMap() :sp(nullptr) {}

	SharedArrayMap(size_t n)
	:sp(new _ValueType[n], std::default_delete<_ValueType[]>())  { }

	_ValueType& operator[](ptrdiff_t d) const
	{
		return	sp.get()[d];
	}
private:
	std::shared_ptr<_ValueType> sp;
};


//template<typename _ValueType>
//SharedArrayMap<_ValueType> makeUniqueArrayMap(size_t n)
//{
//
//}






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
//	explicit ComposeMap(FM &&fm, SM &&sm):fm(std::move(fm)), sm(std::move(sm)) { }
//	explicit ComposeMap(FM &&fm, const SM &sm):fm(std::move(fm)), sm(sm) {}
//	explicit ComposeMap(const FM &fm, SM &&sm):fm(fm), sm(std::move(sm)) {}
	explicit ComposeMap(FM fm,  SM sm):fm(fm), sm(sm) {}


	FirstMap firstMap() const { return fm; }
	SecondMap secondMap() const { return sm; }

	typename Base::ValueType operator[](typename Base::KeyType key) const
	{
		return sm[fm[key]];
	}
};

template<typename FM, typename SM>
ComposeMap<typename std::remove_reference<FM>::type,
  	  	   typename std::remove_reference<SM>::type> makeComposeMap(FM fm, SM sm)

{
	return ComposeMap<typename std::remove_reference<FM>::type,
					  typename std::remove_reference<SM>::type>(fm, sm) ;
}







}// namespace lz




#endif /* LZ_MAP_H_ */



#include <memory>
//#include "lz/adjacency_list.h"

namespace lz {
	namespace GraphUtilityPrivate {
	}

/*
 * Indicate if a graph is directed
 */
struct DirectedGraphTag {};
struct UndirectedGraphTag {};

/*
 * Some common graph`s vertex or edge property tag
 */
struct VertexIndexTag {};
struct EdgeIndexTag {};



template<typename Graph>
typename Graph::VertexDescriptor
opposite(const Graph &g,
         typename Graph::EdgeDescriptor e,
         typename Graph::VertexDescriptor u)
{
    if(g.source(e) != u) return g.source(e);
    return g.target(e);
}

template<typename G>
struct GraphTraits
{
	using VertexDescriptor = typename G::VertexDescriptor;
	using EdgeDescriptor = typename G::EdgeDescriptor;

	using VertexIterator = typename G::VertexIterator;
	using EdgeIterator = typename G::EdgeIterator;

	using DirectedCategory = typename G::DirectedCategory;

	using OutEdgeIterator = typename G::OutEdgeIterator;

	using VertexProperties = typename G::VertexProperties;
	using EdgeProperties = typename G::EdgeProperties;

	template<typename Tag>
	using VertexPropertyMap = typename G::template VertexPropertyMap<Tag>;

	template<typename Tag>
	using EdgePropertyMap = typename G::template EdgePropertyMap<Tag>;

};







template<typename G, typename Tag>
class VertexPropertyMap
{
	using _ValueType = typename std::decay<decltype(typename GraphTraits<G>::VertexProperties()[Tag()])>::type;
	G *g = nullptr;
public:
	using KeyType = typename GraphTraits<G>::VertexDescriptor;
	using ValueType = _ValueType&;

	VertexPropertyMap() = default;
	VertexPropertyMap(G &_g):g(&_g) { }
	ValueType operator[](typename GraphTraits<G>::VertexDescriptor u) const
	{
		return g->vertexProperties(u)[Tag()];
	}

	using Type = VertexPropertyMap<G, Tag>;

	struct ConstType
	{
		using KeyType = typename GraphTraits<G>::VertexDescriptor;
		using ValueType = const _ValueType &;
		const G *g;
		ConstType():g(nullptr){}
		ConstType(const G &g):g(&g){}
		ValueType operator[](KeyType u) const
		{
			return g->vertexProperties(u)[Tag()];
		}
	};
};


template<typename G, typename Tag>
class EdgePropertyMap
{
	using _ValueType = typename std::decay<decltype(typename GraphTraits<G>::EdgeProperties()[Tag()])>::type;
	G *g = nullptr;
public:
	using KeyType = typename GraphTraits<G>::EdgeDescriptor;
	using ValueType = _ValueType&;

	EdgePropertyMap() = default;
	EdgePropertyMap(G &_g):g(&_g) { }
	ValueType operator[](KeyType u) const
	{
		return g->edgeProperties(u)[Tag()];
	}

	using Type = EdgePropertyMap<G, Tag>;

	struct ConstType
	{
	private:
		const G *g;
	public:
		using KeyType = typename GraphTraits<G>::EdgeDescriptor;
		using ValueType = const _ValueType &;

		ConstType():g(nullptr){}
		ConstType(const G &g):g(&g){}
		ValueType operator[](KeyType u) const
		{
			return g->edgeProperties(u)[Tag()];
		}
	};
};


//template<typename G, typename Tag>
//class EdgePropertyMap: public MapFacade<typename GraphTraits<G>::EdgeDescriptor,
//										decltype(typename GraphTraits<G>::EdgeProperties()[Tag()])>
//{
//	G *g = nullptr;
//public :
//	EdgePropertyMap() = default;
//	EdgePropertyMap(G &_g):g(&_g) { }
//
//	using Type = EdgePropertyMap<G, Tag>;
//	using ConstType = EdgePropertyMap<const G, Tag>;
//
//	auto operator[](typename GraphTraits<G>::EdgeDescriptor e) const
//	{
//		return g->edgeProperties(e)[Tag()];
//	}
//};



template<typename G, typename Tag>
typename VertexPropertyMap<G, Tag>::Type
makeVertexPropertyMap(G &g, Tag)
{
	return typename VertexPropertyMap<G, Tag>::Type(g);
}

template<typename G, typename Tag>
typename VertexPropertyMap<G, Tag>::ConstType
makeVertexPropertyMap(const G &g, Tag tag)

{
	return typename VertexPropertyMap<G, Tag>::ConstType(g);
}

template<typename G, typename Tag>
auto makeEdgePropertyMap( G &g, Tag tag)
->decltype(typename EdgePropertyMap<G, Tag>::Type(g))
{
	return typename EdgePropertyMap<G, Tag>::Type(g);
}

template<typename G, typename Tag>
auto makeEdgePropertyMap(const G &g, Tag tag)
->decltype(typename EdgePropertyMap<G, Tag>::ConstType(g))
{
	return typename EdgePropertyMap<G, Tag>::ConstType(g);
}







//
//auto lazyDiscoverTimeMap = [&]() {
//		auto d_array = std::unique_ptr<std::size_t[]>(new std::size_t[g.vertexNumber()]);
//
//		return makeComposeMap(_vertexIndexMap,
//							  std::move(makeUniqueArrayMap( d_array )    ) );
//	};

//template<typename ValueType, typename VertexIndexMap, typename VertexNumberType>
//auto
//makeVertexIndexComposeMap(VertexIndexMap indexMap, VertexNumberType n)
//->decltype(makeComposeMap(indexMap,
//		  std::move(makeUniqueArrayMap(std::unique_ptr<ValueType[]>(new ValueType[n])))	))
//
//{
////	auto value_array = ;
//	return makeComposeMap(indexMap,
//						  std::move(makeUniqueArrayMap(std::unique_ptr<ValueType[]>(new ValueType[n])))	);
//}


template<typename ValueType, typename VertexIndexMap, typename VertexNumberType>
auto
makeVertexIndexComposeMap(VertexIndexMap indexMap, VertexNumberType n)
->decltype(makeComposeMap(indexMap, SharedArrayMap<ValueType>(n)) )

{

	return makeComposeMap(indexMap, SharedArrayMap<ValueType>(n) );
}




//// for graph param
//template<typename G, typename VertexIndexMapName>
//using ChooseVertexIndexMap = ChooseParamReturnType<MemberFunctionReturnType<VertexIndexMapName>,
//		                                           typename GraphTraits<G>::template VertexPropertyMap<VertexIndexTag>::ConstType
//												   >;
//
//
//template<typename GeneralParamName, typename VertexIndexMap, typename ValueType>
//using ChooseVertexIndexComposeMap = ChooseParamReturnType<
//		MemberFunctionReturnType<GeneralParamName>,
//		ComposeMap<VertexIndexMap, IteratorMap<ValueType*> >
//>;
//
//
//template<typename DefaultValueType, typename ParamReturnType, typename VertexIndexMap, typename VertexNumberType>
//auto
//chooseVertexIndexComposeMap(ParamReturnType paramMap, VertexIndexMap indexMap, VertexNumberType n)
////->decltype(paramMap)
//{
//	return paramMap;
//}
//
//template<typename DefaultValueType, typename VertexIndexMap, typename VertexNumberType>
//auto
//chooseVertexIndexComposeMap( ParamNotFound, VertexIndexMap indexMap, VertexNumberType n)
////->decltype(makeComposeMap(indexMap, makeIteratorMap(new DefaultValueType[n])))
//{
//	return makeComposeMap(indexMap, makeIteratorMap(
//									new DefaultValueType[n]
//								 ));
//}
//
//template<typename Map, typename ParamRetrunType>
//void deleteVertexIndexComposeMap(Map map, ParamRetrunType ) { }
//
//template<typename Map>
//void deleteVertexIndexComposeMap(Map map, ParamNotFound )
//{
//	delete[] map.secondMap().iterator();
//}


} // namespace lz




#endif // GRAPH_UTILITY_H




/*
 * iterator.h
 *
 *  Created on: 2015年11月4日
 *      Author: LZ
 */

#ifndef LZ_ITERATOR_FACADE_H_
#define LZ_ITERATOR_FACADE_H_
#include <iterator>
#ifndef UTILITY_H
#define UTILITY_H

#include <functional>
// some little and mess functions

namespace lz {

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

//template<typename ColorType = int>
//struct ColorTraits
//{
//	using Type = ColorType;
//	static ColorType white() { return 0; }
//	static ColorType black() { return 1; }
//};


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





/*
 *  A params is param list, and every param is a member function in params.
 *  Every param have a default value.
 */

//struct ParamNotFound{};

//	namespace UtilityPrivate{
//
//		template<typename UserParam, typename DefaultParam>
//		struct ChooseParam
//		{
//			static UserParam& get(UserParam &up, DefaultParam &dp)
//			{
//				return up;
//			}
//		};
//		template<typename DefaultParam>
//		struct ChooseParam<ParamNotFound, DefaultParam>
//		{
//			static DefaultParam& get(ParamNotFound &up, DefaultParam &dp)
//			{
//				return dp;
//			}
//		};
//
//	} // UtilityPrivate

//template<typename ParamRetrunType, typename Default>
//auto chooseParamReturnValue(ParamRetrunType && p, Default && d) ->
//decltype(UtilityPrivate::ChooseParam<typename std::remove_reference<ParamRetrunType>::type,
//									 typename std::remove_reference<Default>::type >::get(p, d))
//{
//	return UtilityPrivate::ChooseParam<typename std::remove_reference<ParamRetrunType>::type,
//									   typename std::remove_reference<Default>::type>::get(p, d);
//}
//
//
//
//// Choose the param return type we hoped according to the ParamType, DefaultType
//template<typename ParamReturnType, typename DefaultType>
//using ChooseParamReturnType =  typename std::conditional<std::is_same<ParamReturnType, ParamNotFound>::value,
//														 DefaultType,
//														 ParamReturnType>::type;
//
//template<typename ParamName>
//using MemberFunctionReturnType =
//		typename decltype(std::mem_fn(ParamName()))::result_type;
//
//
//

//
//template<typename... Args>
//void emptyFunction(Args... args)
//{
//
//}




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



namespace lz{




/*
 * You should implement the below method:


forward iterator requires:
	Derived& operator++() ;
	typename Derived::reference operator*() const;
	bool operator==(Derived const& o) const;

bidirectional_iterator requires:
	Derived& operator--();

random_access_iterator requires:
	Derived& operator+=(typename Derived::difference_type n);
	typename Derived::difference_type operator-(Derived const& o) const;
	bool operator<(Derived const& o) const;


 */


template <
	class Derived,
	class Category,
	class T,
	class Distance = std::ptrdiff_t,
	class Pointer = T*,
	class Reference = T&
>
class IteratorFacade;

/*
 * Implementation for forward traversal iterators
 */

template <
	class Derived,
	class T,
	class Distance ,
	class Pointer,
	class Reference
>
class IteratorFacade<Derived, std::forward_iterator_tag, T, Distance, Pointer, Reference>
		: public std::iterator<std::forward_iterator_tag, T, Distance, Pointer, Reference>,
		  public lz::FacadeBase<Derived>,
		  public lz::EqualityComparableFacade<Derived>
{
	using Base = std::iterator<std::forward_iterator_tag, T, Distance, Pointer, Reference>;
public:
  	friend Derived operator++(Derived& a, int)
	{
		Derived	tmp(a);
		++a;
		return tmp;
	}

	typename Base::pointer operator->() const
	{
		return &*this->derived();
	}

//	bool operator!=(const Derived &o) const
//	{
//		return !(this->derived() == o);
//	}
};

/*
 * Implementation for bidirectional traversal iterators
 */

template <
	class Derived,
	class T,
	class Distance,
	class Pointer,
	class Reference
>
class IteratorFacade<Derived, std::bidirectional_iterator_tag, T, Distance, Pointer, Reference>
		: public IteratorFacade<Derived, std::forward_iterator_tag, T, Distance, Pointer, Reference>
{
	using Base = IteratorFacade<Derived, std::forward_iterator_tag, T, Distance, Pointer, Reference>;
public:
	using iterator_category = std::bidirectional_iterator_tag;

	friend Derived operator--(Derived&a, int)
	{
		Derived	tmp(a);
		--a;
		return tmp;
	}
};

// Implementation for random access traversal iterators
template <
	class Derived,
	class T,
	class Distance,
	class Pointer,
	class Reference
>
class IteratorFacade<Derived, std::random_access_iterator_tag, T, Distance, Pointer, Reference>
		: public IteratorFacade<Derived,std::bidirectional_iterator_tag, T, Distance, Pointer, Reference>
		, public lz::LessThanComparableFacade<Derived>
{
	using Base = IteratorFacade<Derived,std::bidirectional_iterator_tag, T, Distance, Pointer, Reference>;

public:
	using iterator_category = std::random_access_iterator_tag;

	friend Derived operator+(const Derived &a, typename Base::difference_type b)
	{
		Derived res = a;
		res += b;
		return res;
	}
	friend Derived operator+(typename Base::difference_type b, const Derived &a)
	{
		Derived res = a;
		res += b;
		return res;
	}
	Derived operator-(typename Base::difference_type b) const
	{
		Derived res = this->derived();
		res += -b;
		return res;
	}
	Derived& operator-=(typename Base::difference_type o)
	{
		this->derived() += -o;
		return this->derived();
	}

	typename Base::reference operator[](typename Base::difference_type o) const
	{
		return *(this->derived() + o);
	}


//	bool operator>(const Derived &o) const
//	{
//		return o < this->derived();
//	}
//	bool operator<=(const Derived &o) const
//	{
//		return !(o < this->derived());
//	}
//	bool operator>=(const Derived &o) const
//	{
//		return !(this->derived() < o);
//	}

};


template<typename Integer>
class IntegerIterator: public IteratorFacade<
							IntegerIterator<Integer>,
							std::random_access_iterator_tag,
							Integer,
							std::ptrdiff_t,
							Integer*,
							Integer>
{
	Integer m_val;
	using Base = IteratorFacade<
			IntegerIterator<Integer>,
			std::random_access_iterator_tag,
			Integer,
			std::ptrdiff_t,
			Integer*,
			Integer>;
	using Derived = IntegerIterator<Integer>;
public:
	IntegerIterator() = default;
	explicit IntegerIterator(Integer val):m_val(val) {}

	//forward iterator requires:
	Derived& operator++()
	{
		++ m_val;
		return this->derived();
	}
	typename Base::reference operator*() const
	{
		return m_val;
	}
	bool operator==(Derived const& f) const
	{
		return m_val == f.m_val;
	}

	//bidirectional_iterator requires:
	Derived& operator--()
	{
		-- m_val;
		return this->derived();
	}

	//random_access_iterator requires:
	Derived& operator+=(typename Base::difference_type n)
	{
		m_val += n;
		return this->derived();
	}
	typename Base::difference_type operator-(Derived const& f) const
	{
		return m_val - f.m_val;
	}

	bool operator<(IntegerIterator const& f) const
	{
		return m_val < f.m_val;
	}



};



} // namespace lz



#endif /* LZ_ITERATOR_FACADE_H_ */


//#include "lz/map.h"


namespace lz {


using std::vector;
using std::cout;
using std::endl;
using std::pair;
using std::make_pair;

template<typename DirectedCategory = DirectedGraphTag,
         typename VP = NoProperty,
         typename EP = NoProperty,
         typename GP = NoProperty>
class AdjacencyList;

    namespace AdjacencyListPrivate {

    using VertexDescriptor = int;
    using EdgeDescriptor = int;
    using SizeType = int;



    // vertex data
    template<typename VP>
    struct VertexData: public ChooseDefineProperties<VP>
	{
    	EdgeDescriptor head;
    	VertexData(EdgeDescriptor _head, const VP &vp):head(_head)
    	{
    		ChooseDefineProperties<VP>::set(vp);
    	}
	};

    // edge data
	template<typename EP>
	struct EdgeData:ChooseDefineProperties<EP>
	{
		VertexDescriptor source, target;
		EdgeDescriptor next;

		EdgeData(VertexDescriptor source, VertexDescriptor target, EdgeDescriptor next, const EP & ep)
		:source(source), target(target), next(next)
		{
			ChooseDefineProperties<EP>::set(ep);
		}
	};

    // graph data
    template<typename VP, typename EP, typename GP>
    struct GraphData:public ChooseDefineProperties<GP>
    {
        vector<VertexData<VP> > v;
        vector<EdgeData<EP> > e;
        EdgeDescriptor addEdge(VertexDescriptor a, VertexDescriptor b, const EP &ep = EP())
		{
			this->e.push_back(EdgeData<EP>(a, b, this->v[a].head, ep));
			return this->v[a].head = this->e.size() - 1;
		}
    };



    template<typename G>
    class OutEdgeIterator: public IteratorFacade<
		OutEdgeIterator<G>,
		std::forward_iterator_tag,
		typename GraphTraits<G>::EdgeDescriptor,
		std::ptrdiff_t,
		typename GraphTraits<G>::EdgeDescriptor*,
		typename GraphTraits<G>::EdgeDescriptor >
	{
    	template<typename D, typename VP, typename EP, typename GP> friend class AdjacencyList;

    	EdgeDescriptor i; // realED
    	const G *g;
    	OutEdgeIterator(EdgeDescriptor i, const G *g): i(i), g(g) {} //AdjacencyList call this function

	public:
    	OutEdgeIterator():i(-1), g(nullptr) {}

    	OutEdgeIterator& operator++()
		{
    		i = g->e[i].next;
    		return *this;
		}
    	typename OutEdgeIterator::reference operator*() const
    	{
    		return G::R2V(i);
    	}
    	bool operator==(OutEdgeIterator const& o) const
		{
    		return G::R2V(i) == G::R2V(o.i) && g == o.g;
		}
	};

    // realED: in memory
    // virtualED: for user
    // V2R: virtualED to realED
    // R2V: realED to virtualED
    // DistinguishDirectionGraph
    template<typename Direction, typename VP, typename EP, typename GP>
    struct DistinguishDirectionGraph: public GraphData<VP, EP, GP>
    {
    	static EdgeDescriptor V2R(EdgeDescriptor e) { return e; }
    	static EdgeDescriptor R2V(EdgeDescriptor e) { return e; }
    	using GraphData<VP, EP, GP>::addEdge;
    };

    template<typename VP,
			 typename EP,
			 typename GP>
    struct DistinguishDirectionGraph<UndirectedGraphTag, VP, EP, GP>
    	:public GraphData<VP, EP, GP>
    {
    	static EdgeDescriptor V2R(EdgeDescriptor e) { return e << 1; }
    	static EdgeDescriptor R2V(EdgeDescriptor e) { return e >> 1; }
    	using Base = GraphData<VP, EP, GP>;
    	EdgeDescriptor addEdge(VertexDescriptor a, VertexDescriptor b, const EP &ep = EP())
		{
    		Base::addEdge(a, b, ep);
			return Base::addEdge(b, a, ep) >> 1;
		}
    };

    } // AdjacencyListPrivate




template<typename D, typename VP, typename EP, typename GP>
class VertexPropertyMap< AdjacencyList<D, VP, EP, GP>, VertexIndexTag>
	:public IdentityMap<typename GraphTraits< AdjacencyList<D, VP, EP, GP> >::VertexDescriptor>
{
public:
	VertexPropertyMap() = default;
	VertexPropertyMap(const AdjacencyList<D, VP, EP, GP> &_g){}

	using Type = VertexPropertyMap< AdjacencyList<D, VP, EP, GP>, VertexIndexTag>;
	using ConstType = Type;
};

template<typename D, typename VP, typename EP, typename GP>
class EdgePropertyMap<AdjacencyList<D, VP, EP, GP>, EdgeIndexTag>
	:public IdentityMap<typename GraphTraits<AdjacencyList<D, VP, EP, GP>>::EdgeDescriptor>
{
public:
	EdgePropertyMap() = default;
	EdgePropertyMap(const AdjacencyList<D, VP, EP, GP> &_g){}
	using Type = EdgePropertyMap;
	using ConstType = Type;
};




template<typename Direction, typename VP,
		 typename EP, typename GP>
class AdjacencyList: private AdjacencyListPrivate::DistinguishDirectionGraph<
					 Direction, VP, EP, GP>
{
	template<typename G> friend class AdjacencyListPrivate::OutEdgeIterator;

	using VertexData = AdjacencyListPrivate::VertexData<VP> ;
	using EdgeData = AdjacencyListPrivate::EdgeData<EP> ;
	using G = AdjacencyList;
	using Base = AdjacencyListPrivate::DistinguishDirectionGraph<
				 Direction, VP, EP, GP>;
public:
	using DirectedCategory = Direction;
	using VertexProperties = VP;
	using EdgeProperties = EP;
	using GraphProperties = GP;

	using VertexDescriptor = AdjacencyListPrivate::VertexDescriptor ;
	using EdgeDescriptor = AdjacencyListPrivate::EdgeDescriptor ;
	using SizeType = AdjacencyListPrivate::SizeType;

	using VertexIterator = IntegerIterator<VertexDescriptor>;
	using EdgeIterator = IntegerIterator<EdgeDescriptor>;
	using OutEdgeIterator = AdjacencyListPrivate::OutEdgeIterator<G>;
	template<typename Tag>
	using VertexPropertyMap = lz::VertexPropertyMap<G, Tag>;
	template<typename Tag>
	using EdgePropertyMap = lz::EdgePropertyMap<G, Tag>;

	explicit AdjacencyList(SizeType n = 0, const VP & vp = VP())
	{
		this->v.assign(n, VertexData(-1, vp));
	}
	void clear()
	{
		this->v.clear();
		this->e.clear();
	}
	VertexDescriptor addVertex(const VP &vp = VP())
	{
		this->v.push_back(VertexData(-1, vp));
		return this->v.size() - 1;
	}
	SizeType vertexNumber() const { return this->v.size(); }
	SizeType edgeNumber() const { return this->R2V(this->e.size() - 1) + 1; }
	const GP& graphProperties() const { return this->properties; }
	GP& graphProperties() { return this->properties; }
	VertexDescriptor source(EdgeDescriptor e) const { return this->e[this->V2R(e)].source; }
	VertexDescriptor target(EdgeDescriptor e) const { return this->e[this->V2R(e)].target; }

	VP& vertexProperties(VertexDescriptor u)
	{ return this->v[u].properties; }
	const VP& vertexProperties(VertexDescriptor u) const
	{ return this->v[u].properties; }

	EP& edgeProperties(EdgeDescriptor e)
	{ return this->e[this->V2R(e)].properties; }
	const EP& edgeProperties(EdgeDescriptor e) const
	{ return this->e[this->V2R(e)].properties; }

	EdgeDescriptor addEdge(VertexDescriptor a, VertexDescriptor b, const EP &ep = EP())
	{ return this->Base::addEdge(a, b, ep); }

	template<typename Tag>
	typename VertexPropertyMap<Tag>::Type
	vertexPropertyMap(Tag tag)
	{
		return makeVertexPropertyMap<G, Tag>(*this, tag);
	}

	template<typename Tag>
	typename VertexPropertyMap<Tag>::ConstType
	vertexPropertyMap(Tag tag) const
	{
		return makeVertexPropertyMap<G, Tag>(*this, tag);

// I dont know why wrong with below
//		return makeVertexPropertyMap(*this, tag);
	}



	template<typename Tag>
	typename EdgePropertyMap<Tag>::Type
	edgePropertyMap(Tag tag)

	{ return makeEdgePropertyMap(*this, tag); }

	template<typename Tag>
	typename EdgePropertyMap<Tag>::ConstType
	edgePropertyMap(Tag tag) const
	{ return makeEdgePropertyMap(*this, tag); }


	pair<VertexIterator, VertexIterator> vertices() const
	{
		return make_pair(VertexIterator(0),
						 VertexIterator(this->vertexNumber()));
	}
	pair<EdgeIterator, EdgeIterator> edges() const
	{
		return make_pair(EdgeIterator(0), EdgeIterator(this->edgeNumber()));
	}
	pair<OutEdgeIterator, OutEdgeIterator> outEdges(VertexDescriptor u) const
	{
		return make_pair(OutEdgeIterator(this->v[u].head, this),
						 OutEdgeIterator(-1, this)) ;
	}

};







} // namespace lz


#endif // ADJACENCY_LIST_H





#ifndef STRONG_COMPONENTS_H
#define STRONG_COMPONENTS_H

#include <utility>
#include <functional>

//#include "lz/adjacency_list.h"
/*
 * parameter.h
 *
 *  Created on: 2015年11月30日
 *      Author: LZ
 */

#ifndef LZ_PARAMETER_H_
#define LZ_PARAMETER_H_

//#include "lz/property.h"
#include <utility>

namespace lz{


struct ParamNotFound {
	using Keyword = ParamNotFound;
	using Reference = ParamNotFound;
	using Next = ParamNotFound;
};
struct EmptyParamPack;


namespace Parameter{



template<typename _Tag>
struct Keyword;
template<typename _Keyword, typename _Default>
struct Default;

template<typename _Keyword, typename _Default>
struct LazyDefault;

template<typename ArgList, typename QueryKeyword, typename Keyword>
struct GetParamPackImpl
{
	using ParamPackType = typename GetParamPackImpl<typename ArgList::Next,
							QueryKeyword, typename ArgList::Next::Keyword>::ParamPackType;
};

template<typename ArgList, typename QueryKeyword>
struct GetParamPackImpl<ArgList, QueryKeyword, QueryKeyword>
{
	using ParamPackType = ArgList;
};

template<typename ArgList, typename QueryKeyword>
struct GetParamPackImpl<ArgList, QueryKeyword, EmptyParamPack>
{
	using ParamPackType = EmptyParamPack;
};

template<typename ArgList, typename QueryKeyword>
struct GetParamPack
{
	using ParamPackType = typename GetParamPackImpl<ArgList, QueryKeyword, typename ArgList::Keyword>::ParamPackType;
};



template<typename ArgList, typename QueryKeyword, typename Base = typename GetParamPack<ArgList, QueryKeyword>::ParamPackType >
struct GetReference
{
	static auto apply(const ArgList &ag)
	->decltype(std::forward<typename Base::Reference>(ag.Base::ref))
	{
//		cout << "HH " << endl;
//		cout << ag.ref << endl;
		return std::forward<typename Base::Reference>(ag.Base::ref);
	}
};

template<typename ParamPack, typename QueryKeyword>
struct GetReference<ParamPack, QueryKeyword, EmptyParamPack>
{
	static ParamNotFound apply(const ParamPack &ag)
	{
		return ParamNotFound();
	}
};

template<typename _Keyword, typename _Default>
struct Default
{
	using Keyword =_Keyword;
	using Reference =_Default;

	Reference ref;
	Default(Reference ref):ref(std::forward<Reference>(ref)) {}

	Default(const Default &o)
	:ref(std::forward<Reference>(o.ref)) {}
};

template<typename _Keyword, typename _Default>
struct LazyDefault
{
	using Keyword =_Keyword;
	using Reference =_Default;

	Reference ref;

	LazyDefault(Reference ref):ref(std::forward<Reference>(ref)) {}

	LazyDefault(const LazyDefault &o)
	:ref(std::forward<Reference>(o.ref)) {}
};


template<typename ParamPack, typename Default,
		 typename Base = typename GetParamPack<ParamPack, typename Default::Keyword>::ParamPackType>
struct ComputeDefault
{
	static auto apply(const ParamPack &ar, Default o)
	->decltype(ar[typename Default::Keyword()])
	{
		return ar[typename Default::Keyword()];
	}
};
template<typename ParamPack, typename Default>
struct ComputeDefault<ParamPack, Default, EmptyParamPack>
{
	static auto apply(const ParamPack &ar, Default o)
	->decltype(std::forward<typename Default::Reference>(o.ref))
	{
		return std::forward<typename Default::Reference>(o.ref);
	}
};



template<typename ParamPack, typename LazyDefault,
		 typename Base = typename GetParamPack<ParamPack, typename LazyDefault::Keyword>::ParamPackType>
struct ComputeLazyDefault
{
	static auto apply(const ParamPack &ar, LazyDefault o)
	->decltype(ar[typename LazyDefault::Keyword()])
	{
		return ar[typename LazyDefault::Keyword()];
	}
};
template<typename ParamPack, typename LazyDefault>
struct ComputeLazyDefault<ParamPack, LazyDefault, EmptyParamPack>
{
	static auto apply(const ParamPack &ar, LazyDefault o)
	->decltype(o.ref())
	{
		return o.ref();
	}
};





} // namespace Parameter



struct EmptyParamPack
{

	using Keyword = EmptyParamPack;
	using Reference = EmptyParamPack;
	using Next = EmptyParamPack;

	template<typename QTag>
	ParamNotFound operator[](Parameter::Keyword<QTag>) const
	{
		return ParamNotFound();
	}

	template<typename QK, typename D>
	auto operator[](Parameter::Default<QK, D> o) const
	->decltype(std::forward<decltype(o.ref)>(o.ref))
	{
		return std::forward<decltype(o.ref)>(o.ref);
	}


	template<typename QK, typename D>
	auto operator[](Parameter::LazyDefault<QK, D> o) const
	->decltype(o.ref())
	{
		return o.ref();
	}


//	template<typename QTag>
//	auto operator[](Parameter::Keyword<QTag>) const
//	->decltype(Parameter::GetReference<EmptyParamPack, Parameter::Keyword<QTag> >::apply(*this))
//	{
//		return Parameter::GetReference<EmptyParamPack, Parameter::Keyword<QTag> >::apply(*this);
//	}
//
//	template<typename QK, typename D>
//	auto operator[](Parameter::Default<QK, D> o) const
//
//	->decltype(Parameter::ComputeDefault<EmptyParamPack, Parameter::Default<QK, D>>::apply(*this, o))
//	{
//		return Parameter::ComputeDefault<EmptyParamPack, Parameter::Default<QK, D>>::apply(*this, o);
//	}
//
//
//
//	template<typename QK, typename D>
//	auto operator[](Parameter::LazyDefault<QK, D> o) const
//	->decltype(Parameter::ComputeLazyDefault<EmptyParamPack, Parameter::LazyDefault<QK, D> >::apply(*this, o))
//	{
//		return Parameter::ComputeLazyDefault<EmptyParamPack, Parameter::LazyDefault<QK, D> >::apply(*this, o);
//	}



};



namespace Parameter{






template<typename _Keyword, typename _Reference, typename _Next = EmptyParamPack>
struct ParamPack:public _Next
{


	template<typename QK>
	using QueryReference = decltype(GetReference<ParamPack, typename std::decay<QK>::type>::apply(

			std::declval<ParamPack>()
			   ) );

	_Reference ref;
	ParamPack(_Reference _ref):ref(std::forward<_Reference>(*&_ref)  )
	{
//		cout << std::is_same<int&&, decltype(ref)>::value << "|||||||" <<  endl;
//		cout << "CON" <<  _ref << endl;
//		cout << "CON" <<  ref << endl;
//		cout << "REF: " << &ref << endl;
	}
	ParamPack(_Reference ref, const _Next &next):ref(std::forward<_Reference>(ref)), _Next(next) {}

	ParamPack(const ParamPack &o)
	:ref(std::forward<_Reference>(o.ref)), _Next(*static_cast<const _Next*>(&o) )
//	:ParamPack(std::forward<_Reference>(o.ref), o.Next)
	{
//		cout << "HHEHEHEEH" << endl;

	}

	template<typename OKeyword, typename OReference>
	ParamPack<OKeyword, OReference, ParamPack> operator,(ParamPack<OKeyword, OReference> o) const
	{
		return ParamPack<OKeyword, OReference, ParamPack>
				(std::forward<OReference>(o.ref), *this);
	}

	template<typename QTag>
	auto operator[](Parameter::Keyword<QTag>) const
	->decltype(GetReference<ParamPack, Parameter::Keyword<QTag> >::apply(*this))
	{
//		cout << "SB " << ref << endl;
//		cout << "REF: " << &ref << endl;
		return GetReference<ParamPack, Parameter::Keyword<QTag> >::apply(*this);
	}

	template<typename QK, typename D>
	auto operator[](Default<QK, D> o) const

	->decltype(ComputeDefault<ParamPack, Default<QK, D>>::apply(*this, o))
	{
		return ComputeDefault<ParamPack, Default<QK, D>>::apply(*this, o);
	}



	template<typename QK, typename D>
	auto operator[](LazyDefault<QK, D> o) const
	->decltype(ComputeLazyDefault<ParamPack, LazyDefault<QK, D> >::apply(*this, o))
	{
		return ComputeLazyDefault<ParamPack, LazyDefault<QK, D> >::apply(*this, o);
	}

	using Keyword = _Keyword;
	using Reference = _Reference;
	using Next = _Next;

};




template<typename _Tag>
struct Keyword
{
	using Tag = _Tag;

	template<typename T>
	auto operator=(T &&t) const
	->ParamPack<Keyword, decltype(std::forward<T>(t))>
	{
//		cout <<" T: " << t << endl;
		return ParamPack<Keyword, decltype(std::forward<T>(t))>(std::forward<T>(t));
	}


	template<typename T>
	auto operator|(T &&t) const
	->Default<Keyword, decltype(std::forward<T>(t)) >
	{
		return Default<Keyword, decltype(std::forward<T>(t)) >(std::forward<T>(t));
	}

	template<typename T>
	auto operator||(T &&t) const
	->LazyDefault<Keyword, decltype(std::forward<T>(t))>
	{
		return LazyDefault<Keyword, decltype(std::forward<T>(t))>(std::forward<T>(t));
	}




	static const Keyword<Tag> instance;
};
template<typename _Tag>
const Keyword<_Tag> Keyword<_Tag>::instance = {};












#define LZ_PARAMETER_KEYWORD(tag_namespace,name)                    \
    namespace tag_namespace                                         \
    {                                                               \
		struct name                                                 \
		{                                                           \
		};                                                          \
    }                                                               \
    namespace                                                       \
    {                                                               \
		::lz::Parameter::Keyword<tag_namespace::name> const& name   \
		= ::lz::Parameter::Keyword<tag_namespace::name>::instance;  \
    }																\

#define LZ_PARAMETER_KEYWORD_TAG_NAMESPACE(name)   \
	LZ_PARAMETER_KEYWORD(tag, name)



}// namespace lz::Parameter



//template<typename ArgList, typename QueryKeyword>
//struct GetParamPack
//{
//	using ParamPackType = typename GetParamPackImpl<ArgList, QueryKeyword, typename ArgList::Keyword>::ParamPackType;
//};



template<typename ParamPack, typename KeywordType, typename Default = ParamNotFound>
using ChooseParamType =	typename std::conditional<

	!std::is_same<typename Parameter::GetParamPack<ParamPack, KeywordType>::ParamPackType::Reference, ParamNotFound >::value,

	typename Parameter::GetParamPack<ParamPack, KeywordType>::ParamPackType::Reference,


	Default>::type;




} // namespace lz


















#endif /* LZ_PARAMETER_H_ */

















#ifndef DEPTH_FIRST_SEARCH_H
#define DEPTH_FIRST_SEARCH_H

#include <tuple>
#include <algorithm>
#include <vector>
#include <stack>
#include <type_traits>
#include <iostream>
#include <memory>


//#include "lz/utility.h"

//#include "lz/graph_utility.h"
//#include "lz/utility.h"
//#include "lz/property.h"
//#include "lz/map.h"
//#include "lz/parameter.h"

namespace lz {

using std::tie;
using std::pair;
using std::stack;
using std::vector;
using std::cout;
using std::endl;



namespace DepthFirstSearchKeywords {

	LZ_PARAMETER_KEYWORD(tag, initializeVertex)
	LZ_PARAMETER_KEYWORD(tag, startVertex)
	LZ_PARAMETER_KEYWORD(tag, discoverVertex)
	LZ_PARAMETER_KEYWORD(tag, examineEdge)
	LZ_PARAMETER_KEYWORD(tag, treeEdge)
	LZ_PARAMETER_KEYWORD(tag, treeEdgeReturn)
	LZ_PARAMETER_KEYWORD(tag, notTreeEdge)
	LZ_PARAMETER_KEYWORD(tag, finishEdge)
	LZ_PARAMETER_KEYWORD(tag, finishVertex)


	LZ_PARAMETER_KEYWORD(tag, colorMap)
	LZ_PARAMETER_KEYWORD(tag, white)
	LZ_PARAMETER_KEYWORD(tag, black)

	LZ_PARAMETER_KEYWORD(tag, isInit)
	LZ_PARAMETER_KEYWORD(tag, vertexIndexMap)
	LZ_PARAMETER_KEYWORD(tag, outEdges)
	LZ_PARAMETER_KEYWORD(tag, enterVertex)
}



	namespace DepthFirstSearchPrivate {


	using namespace DepthFirstSearchKeywords;


	template<typename G, typename ParamPack>
	struct Impl
	{
		const G &g;
		const ParamPack &p;

		Impl(const G &g, const ParamPack &p):g(g), p(p)
		{
			init(p[isInit]);
			chooseEnterVertex(p[enterVertex]);
		}

		void dfs(typename GraphTraits<G>::VertexDescriptor u)
		{
			p[colorMap][u] = p[black]();
			p[discoverVertex](u);
			auto ei = p[outEdges](g, u);
			for(;ei.first != ei.second; ++ ei.first)
			{
				auto e = *ei.first;
				auto to = g.target(e);
				p[examineEdge](e, u);
				if(p[colorMap][to] == p[white]())
				{
					p[treeEdge](e, u);
					dfs(to);
					p[treeEdgeReturn](e, u);
				}
				else
				{
					p[notTreeEdge](e, u);
				}
				p[finishEdge](e, u);
			}
			p[finishVertex](u);
		}

		void init(std::true_type)
		{
			auto&& vi = g.vertices();
			for(;vi.first != vi.second; ++vi.first)
			{
				auto&& u = *vi.first;
				p[colorMap][u] = p[white]();
				p[initializeVertex](u);
			}
		}
		void init(std::false_type) {}


		template<typename EnterVertex>
		void chooseEnterVertex(EnterVertex u)
		{
			p[startVertex](u);
			dfs(u);
		}
		void chooseEnterVertex(ParamNotFound )
		{
			auto&& vi = g.vertices();
			for(;vi.first != vi.second; ++vi.first)
			{
				auto&& u = *vi.first;
				if(p[colorMap][u] == p[white]())
				{
					p[startVertex](u);
					dfs(u);
				}
			}
		}


	};

	}


template<typename G, typename ParamPack>
void depthFirstSearch(const G &g, const ParamPack &p)
{
	using namespace DepthFirstSearchKeywords;

	auto _white = p[white | ColorTraits<>::white];
	auto _black = p[black | ColorTraits<>::black];
	auto _isInit = p[isInit | std::true_type()];

	auto _outEdges = p[outEdges | std::mem_fn(&G::outEdges)];
	auto _vertexIndexMap = p[vertexIndexMap | g.vertexPropertyMap(VertexIndexTag())];

	using ColorValue = decltype(_white());

	auto n = g.vertexNumber();

	auto iMap = p[vertexIndexMap | g.vertexPropertyMap(VertexIndexTag())];

	auto gg = std::bind(
			makeVertexIndexComposeMap<ColorValue, decltype(iMap), decltype(n) >,
			iMap, n);


	auto _colorMap = p[colorMap
					   ||

					   std::bind(
					   			makeVertexIndexComposeMap<ColorValue, decltype(iMap), decltype(n) >,
					   			iMap, n)

					   ];
	auto _enterVertex = p[enterVertex];

	using V = typename GraphTraits<G>::VertexDescriptor;
	using E = typename GraphTraits<G>::EdgeDescriptor;

	auto actionVertex = [](V u){};
	auto actionEdge = [](E e, V u){};

	auto newParamPack = (
			initializeVertex = p[initializeVertex | actionVertex],
			startVertex = p[startVertex | actionVertex],
			discoverVertex = p[discoverVertex | actionVertex],
			examineEdge = p[examineEdge | actionEdge],
			treeEdge = p[treeEdge | actionEdge],
			treeEdgeReturn = p[treeEdgeReturn | actionEdge],
			notTreeEdge = p[notTreeEdge | actionEdge],
			finishEdge = p[finishEdge | actionEdge],
			finishVertex = p[finishVertex | actionVertex],

			white = _white,
			black = _black,
			isInit = _isInit,
			outEdges = _outEdges,
			vertexIndexMap = _vertexIndexMap,
			colorMap = _colorMap,
			enterVertex =  _enterVertex);


	DepthFirstSearchPrivate::Impl
	<G, decltype(newParamPack)>(g, newParamPack);






}





//	namespace DepthFirstSearchPrivate {
//
//	template<typename G, typename Params>
//	struct Impl
//	{
//		using V = typename GraphTraits<G>::VertexDescriptor;
//		using VertexIndexMap = ChooseVertexIndexMap<G, decltype(&Params::vertexIndexMap)>;
//		using DefaultColor = ColorTraits<>::Type;
//		using ColorMap = ChooseVertexIndexComposeMap<decltype(&Params::colorMap), VertexIndexMap, DefaultColor>;
//
//		VertexIndexMap indexMap;
//		ColorMap colorMap;
//		const G &g;
//		Params &p;
//		Impl(const G &g, Params &p):g(g), p(p)
//		{
//			indexMap = chooseParamReturnValue(p.vertexIndexMap(),
//											  g.vertexPropertyMap(VertexIndexTag()));
//			colorMap = chooseVertexIndexComposeMap<ColorTraits<>::Type>(p.colorMap(), indexMap, g.vertexNumber());
//		}
//		auto outEdges(V u)
//		{
//			return chooseParamReturnValue(p.outEdges(u), g.outEdges(u) );
//		};
//		void init(std::true_type)
//		{
//			auto&& vi = g.vertices();
//			for(;vi.first != vi.second; ++vi.first)
//			{
//				auto&& u = *vi.first;
//				colorMap[u] = p.white();
//				p.initializeVertex(u);
//			}
//		}
//		void init(std::false_type) {}
//
//		template<typename EnterVertex>
//		void chooseEnterVertex(EnterVertex u)
//		{
//			p.startVertex(u);
//			dfs(u);
//		}
//		void chooseEnterVertex(ParamNotFound )
//		{
//			auto&& vi = g.vertices();
//			for(;vi.first != vi.second; ++vi.first)
//			{
//				auto&& u = *vi.first;
//				if(colorMap[u] == p.white())
//				{
//					p.startVertex(u);
//					dfs(u);
//				}
//			}
//		}
//
//
//		void dfs(typename GraphTraits<G>::VertexDescriptor u)
//		{
//			colorMap[u] = p.black();
//			p.discoverVertex(u);
//			auto ei = outEdges(u);
//			for(;ei.first != ei.second; ++ ei.first)
//			{
//				auto e = *ei.first;
//				auto to = g.target(e);
//				p.examineEdge(e, u);
//				if(colorMap[to] == p.white())
//				{
//					p.treeEdge(e, u);
//					dfs(to);
//					p.treeEdgeReturn(e, u);
//				}
//				else
//				{
//					p.notTreeEdge(e, u);
//				}
//				p.finishEdge(e, u);
//			}
//			p.finishVertex(u);
//		}
//
//		void run()
//		{
//			init(typename Params::IsInit());
//			chooseEnterVertex(p.enterVertex());
//			deleteVertexIndexComposeMap(colorMap, p.colorMap());
//		}
//	};
//	} // namespace DepthFirstSearchPrivate
//
//class DepthFirstSearchParams
//{
//	using DefaultColor = ColorTraits<>::Type;
//public:
//	template<typename V> void initializeVertex(V u) {}
//	template<typename V> void startVertex(V u) {}
//	template<typename V> void discoverVertex(V u) {}
//	template<typename E, typename V> void examineEdge(E e, V u) {}
//	template<typename E, typename V> void treeEdge(E e, V u) {}
//	template<typename E, typename V> void treeEdgeReturn(E e, V u) {}
//	template<typename E, typename V> void notTreeEdge(E e, V u) {}
//	template<typename E, typename V> void finishEdge(E e, V u) {}
//	template<typename V> void finishVertex(V u) {}
//
//	DefaultColor white() { return ColorTraits<>::white(); }
//	DefaultColor black() { return ColorTraits<>::black(); }
//
//	using IsInit = std::true_type;
//	ParamNotFound vertexIndexMap() {}
//	template<typename V>
//	ParamNotFound outEdges(V u) {}
//	ParamNotFound colorMap() {}
//	ParamNotFound enterVertex(){}
//};
//
//template<typename G, typename Params>
//void depthFirstSearch(const G &g, Params &&p)
//{
//	using RealParams = typename std::remove_reference<Params>::type;
//
//	DepthFirstSearchPrivate::Impl<G, RealParams> impl(g, p);
//	impl.run();
//}



























} // namesace lz
#endif // DEPTH_FIRST_SEARCH_H


//#include "lz/map.h"
//#include "lz/utility.h"
//#include "lz/graph_utility.h"

namespace lz {

using std::swap;
using std::less;

namespace StronglyConnectedComponentsKeywords {

	LZ_PARAMETER_KEYWORD(tag, vertexIndexMap)
	LZ_PARAMETER_KEYWORD(tag, discoverTimeMap)
	LZ_PARAMETER_KEYWORD(tag, rootMap)
}


    namespace StronglyConnectedComponentsPrivate {




    }// namespace StronglyConnectedComponentsPrivate







    template<typename G, typename ComponentMap, typename ParamPack = lz::EmptyParamPack>

	auto stronglyConnectedComponents(const G &g, ComponentMap compMap, const ParamPack &p = lz::EmptyParamPack() )
    {
		static_assert(std::is_same<typename GraphTraits<G>::DirectedCategory, DirectedGraphTag>::value, "this graph is not directed");

		namespace k = StronglyConnectedComponentsKeywords;

		auto indexMap = p[k::vertexIndexMap | g.vertexPropertyMap(VertexIndexTag())];

		using Vertex = typename GraphTraits<G>::VertexDescriptor;
		using Edge = typename GraphTraits<G>::EdgeDescriptor;


		auto n = g.vertexNumber();

		auto discoverTimeMap =
				p[k::discoverTimeMap ||
				 std::bind(makeVertexIndexComposeMap<size_t, decltype(indexMap), decltype(n) >,
						   indexMap, n) ];

		auto rootMap =  p[k::rootMap ||
						  std::bind(makeVertexIndexComposeMap<Vertex, decltype(indexMap), decltype(n) >,
						  		    indexMap, n)];


		using ComponentType = typename std::decay<typename MapTraits<ComponentMap>::ValueType>::type;
		using TimeType = typename std::decay<typename MapTraits<decltype(discoverTimeMap)>::ValueType>::type;

		using namespace DepthFirstSearchKeywords;

		ComponentType compNumber = 0;
		TimeType dfsTime = 0;

		std::vector<Vertex> stack;
		std::vector<bool>inStack(n, 0);


		auto minRoot = [&](Vertex u, Vertex v) ->Vertex
		{
			auto ans =  discoverTimeMap[u] < discoverTimeMap[v] ? u : v;
			return ans;
		};


		depthFirstSearch(g, (

				colorMap = discoverTimeMap,
				white =  []()->TimeType { return 0; },
				black = [&]()->TimeType { return ++dfsTime; }
				,

				discoverVertex = [&](Vertex u)
				{
					rootMap[u] = u;
					stack.push_back(u);
					inStack[u] = 1;
				},

				treeEdgeReturn = [&](Edge e, Vertex u)
				{
					Vertex to = opposite(g, e, u);
					rootMap[u] = minRoot(rootMap[u], rootMap[to]);
				},

				notTreeEdge = [&](Edge e, Vertex u)
				{
					Vertex to = opposite(g, e, u);
					if(inStack[to])
					{
						rootMap[u] = minRoot(rootMap[u], to);
					}
				},

				finishVertex = [&](Vertex u)
				{
					if(u == rootMap[u])
					{
						Vertex tmp;
						do
						{
							tmp = stack.back();
							stack.pop_back();
							compMap[tmp] = compNumber;
							inStack[tmp] = 0;
						}
						while(tmp != u);
						++compNumber;
					}
				}
		));





		return compNumber;


    }





















//struct StronglyConnectedComponentsParams{
//
//	ParamNotFound vertexIndexMap(){}
//	ParamNotFound discoverTimeMap(){}
//	ParamNotFound rootMap(){}
//};
//
//
//
//    namespace StronglyConnectedComponentsPrivate {
//
//
//
//	template<typename G, typename ComponentMap, typename Params>
//	struct Impl: public DepthFirstSearchParams
//	{
//		using V = typename GraphTraits<G>::VertexDescriptor;
//		using E = typename GraphTraits<G>::EdgeDescriptor;
//
//		using VertexIndexMap = ChooseVertexIndexMap<G, decltype(&Params::vertexIndexMap)>;
//
//		using DiscoverTimeMap = ChooseVertexIndexComposeMap<decltype(&Params::discoverTimeMap), VertexIndexMap, size_t>;
//
//		using RootMap = ChooseVertexIndexComposeMap<decltype(&Params::rootMap), VertexIndexMap, V>;
//
//		using ComponentType = typename std::decay<typename MapTraits<ComponentMap>::ValueType>::type;
//		using TimeType = typename std::decay<typename MapTraits<DiscoverTimeMap>::ValueType>::type;
//
//		const G &g;
//		ComponentMap compMap;
//		Params &p;
//		DiscoverTimeMap discoverTimeMap;
//		RootMap rootMap;
//		vector<bool> inStack;
//		vector<V> stack;
//		ComponentType compNumber = 0;
//		TimeType dfsTime = 0;
//
//		Impl(const G &g,
//			 ComponentMap compMap,
//			 Params & p):g(g), compMap(compMap), p(p)
//		{
//			VertexIndexMap
//			indexMap = chooseParamReturnValue(
//						p.vertexIndexMap(),
//						g.vertexPropertyMap(VertexIndexTag()));
//			auto n = g.vertexNumber();
//
//			discoverTimeMap = chooseVertexIndexComposeMap<size_t>(p.discoverTimeMap(), indexMap, n);
//
//			rootMap = chooseVertexIndexComposeMap<V>(p.rootMap(), indexMap, n);
//
//			inStack.assign(n, 0);
//		}
//
//		DiscoverTimeMap colorMap() { return discoverTimeMap; }
//		TimeType white() { return 0; }
//		TimeType black()
//		{
//			return ++ dfsTime;
//		}
//		void discoverVertex(V u)
//		{
//			rootMap[u] = u;
//			stack.push_back(u);
//			inStack[u] = 1;
//		}
//		void treeEdgeReturn(E e, V u)
//		{
//			V to = opposite(g, e, u);
//			rootMap[u] = minRoot(rootMap[u], rootMap[to]);
//		}
//		void notTreeEdge(E e, V u)
//		{
//			V to = opposite(g, e, u);
//			if(inStack[to])
//			{
//
//				rootMap[u] = minRoot(rootMap[u], to);
//			}
//		}
//		void finishVertex(V u)
//		{
//			if(u == rootMap[u])
//			{
//				V tmp;
//				do
//				{
//					tmp = stack.back();
//					stack.pop_back();
//					compMap[tmp] = compNumber;
//					inStack[tmp] = 0;
//				}
//				while(tmp != u);
//				++compNumber;
//			}
//		}
//	private:
//		V minRoot(V u, V v)
//		{
//			return discoverTimeMap[u] < discoverTimeMap[v] ? u : v;
//		}
//	};
//
//    } // namespace StronglyConnectedComponentsPrivate
//
//    template<typename G, typename ComponentMap, typename Params = StronglyConnectedComponentsParams>
//
////    typename std::decay<typename MapTraits<ComponentMap>::ValueType>::type
//	auto stronglyConnectedComponents(const G &g, ComponentMap compMap, Params &&p = StronglyConnectedComponentsParams() )
//    {
//		static_assert(std::is_same<typename GraphTraits<G>::DirectedCategory, DirectedGraphTag>::value, "this graph is not directed");
//		using RealParams = typename std::remove_reference<Params>::type;
//
//		StronglyConnectedComponentsPrivate::Impl<G, ComponentMap, RealParams> impl(g, compMap, p);
//
//
//		depthFirstSearch(g, impl);
//
//
//		deleteVertexIndexComposeMap(impl.rootMap, p.rootMap());
//		deleteVertexIndexComposeMap(impl.discoverTimeMap, p.discoverTimeMap());
//		return impl.compNumber;
//
//    }



} // namespace lz




#endif // STRONG_COMPONENTS_H
























using namespace std;
using namespace lz;


using G = AdjacencyList<>;



int main()
{
//	freopen("in.txt", "r", stdin);

	G g;
	int n;
//	cin >> n;
	scanf("%d" , &n);

	for(int i = 0; i < n; ++ i) g.addVertex();
//	cout << "N " << n << endl;

	for(int i = 0; i < n; ++ i)
	{
		for(int j = 0; j < n; ++ j)
		{
			int x;
			scanf("%d", &x);
//			cin >> x;
			if(x)
			{
//				cout << "XX: " << endl;
				g.addEdge(i, j);
			}
		}
	}
//	cout << "FFFFJJJ " << endl;

	vector<int> compMap(n);
	int ans = stronglyConnectedComponents(g, makeIteratorMap(compMap.begin()));

//	for(int i = 0; i < n; ++ i)
//	{
//		cout << compMap[i] << endl;
//	}
	if(ans == 1) puts("YES");
	else puts("NO");




	return 0;





}





















