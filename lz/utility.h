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



template<bool Con, typename T, typename F>
struct ChooseValue
{
	static T get(T t, F f)
	{
		return t;
	}
};
template<typename T, typename F>
struct ChooseValue<false, T, F>
{
	static F get(T t, F f)
	{
		return f;
	}
};

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
