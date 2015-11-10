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

template<typename Key>
struct IdentityMap
{
	Key operator[](Key key) const
	{
		return key;
	}
};



template<typename UnaryFunction>
class FunctionMap
{
	using F = UnaryFunction*;

	F f;
public:
	explicit FunctionMap() = default;

	explicit FunctionMap(const F &f):f(f) {}

	template<typename Key>
	auto operator[](Key i) const ->decltype(f(i))
	{
		return f(i);
	}

};

template<typename UnaryFunction>
FunctionMap<UnaryFunction> makeFunctionMap(const UnaryFunction &f)
{
	return FunctionMap<UnaryFunction>(f);
}


template<typename T>
class ClassMap
{
	T *t;
public:
	ClassMap() = default;

    ClassMap(T *t):t(t){}

    template<typename P>
    auto operator[](P p) const ->decltype(t->*p)
    {
        return t->*p;
    }
};

template<typename T>
ClassMap<T> makeClassMap(T *t)
{
	return ClassMap<T>(t);
}








}// namespace lz




#endif /* LZ_MAP_H_ */
