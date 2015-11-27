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
struct EqualityComparableFacade
{
	bool operator!=(const Derived o) const
	{
		return this->derived() == o;
	}
protected:
	Derived& derived()
	{ return *static_cast<Derived*>(this); }

	Derived const& derived() const
	{ return *static_cast<Derived const*>(this); }
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

template<typename Derived>
struct EqualityComparableFacade:public FacadeBase<Derived>
{
	bool operator!=(const Derived o) const
	{
		return this->derived() == o;
	}
};

template<typename Derived>
struct LessThanComparable: public FacadeBase<Derived>
{
	bool operator>(const Derived &o) const
	{
		return o < this->derived();
	}
	bool operator<=(const Derived &o) const
	{
		return !(o < this->derived());
	}
	bool operator>=(const Derived &o) const
	{
		return !(this->derived() < o);
	}

};





/*
 *  A params is param list, and every param is a member function in params.
 *  Every param have a default value.
 */

struct ParamNotFound{};

	namespace UtilityPrivate{

		template<typename UserParam, typename DefaultParam>
		struct ChooseParam
		{
			static UserParam& get(UserParam &up, DefaultParam &dp)
			{
				return up;
			}
		};
		template<typename DefaultParam>
		struct ChooseParam<ParamNotFound, DefaultParam>
		{
			static DefaultParam& get(ParamNotFound &up, DefaultParam &dp)
			{
				return dp;
			}
		};

	} // UtilityPrivate

template<typename ParamRetrunType, typename Default>
auto chooseParamReturnValue(ParamRetrunType && p, Default && d) ->
decltype(UtilityPrivate::ChooseParam<typename std::remove_reference<ParamRetrunType>::type,
									 typename std::remove_reference<Default>::type >::get(p, d))
{
	return UtilityPrivate::ChooseParam<typename std::remove_reference<ParamRetrunType>::type,
									   typename std::remove_reference<Default>::type>::get(p, d);
}

//template<typename ParamRetrunType, typename Default>
//Default& chooseParamReturnValue(ParamNotFound &&, Default && d)
//{
//	return d;
//}



// Choose the param type we hoped according to the ParamType, DefaultType
template<typename ParamReturnType, typename DefaultType>
using ChooseParamReturnType =  typename std::conditional<std::is_same<ParamReturnType, ParamNotFound>::value,
																DefaultType,
																ParamReturnType>::type;


//In Params, Choose the return type of the function Param that is no other params list
//template<typename Params, typename Param>
//using ParamReturnType = typename std::result_of< Param(Params)>::type;





template<typename ParamName>
using MemberFunctionReturnType =
		typename decltype(std::mem_fn(ParamName()))::result_type;



















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




//template<bool Con, typename T, typename F>
//struct ChooseValue
//{
//	static T get(T t, F f)
//	{
//		return t;
//	}
//};
//template<typename T, typename F>
//struct ChooseValue<false, T, F>
//{
//	static F get(T t, F f)
//	{
//		return f;
//	}
//};

//template<bool con, typename T, typename F>
//struct Choose
//{
//	static T get(T t, F f){ return  t; }
//};
//template<typename T, typename F>
//struct Choose<0, T, F>
//{
//	static F get(T t, F f){ return  f; }
//};








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
