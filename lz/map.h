/*
 * map.h
 *
 *  Created on: 2015Äê8ÔÂ5ÈÕ
 *      Author: LZ
 */

/*
 * Map concept is similar to boost property map library,
 * but Map concept is sample. It only contain operator[] for
 * every Map object.
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

template<typename UnaryFunction>
class FunctionMap
{

	typedef UnaryFunction F;

	const F *f;

public:
	explicit FunctionMap():f(nullptr){}

	explicit FunctionMap(const F &f):f(&f) {}

	template<typename Key>
	auto operator[](Key i) const ->decltype((*f)(i))
	{
		return (*f)(i);
	}

	template<typename Key>
	auto operator[](Key i) ->decltype((*f)(i))
	{
		return (*f)(i);
	}

};

template<typename UnaryFunction>
FunctionMap<UnaryFunction> makeFunctionMap(const UnaryFunction &f)
{
	return FunctionMap<UnaryFunction>(f);
}




}// namespace lz




#endif /* LZ_MAP_H_ */
