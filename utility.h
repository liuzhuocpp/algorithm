#ifndef UTILITY_H
#define UTILITY_H

#include <functional>
// some little and mess functions

namespace lz{

template<typename T> 
int sz(const T &o) { return int(o.size()); }

template<typename T>
int topBit(T x) 
{
    int r = 0;
    for(;x > 0; x >>= 1)
    {
        r ++;
    }
    return r;
}

template<typename T, typename Multiply = multiplies<T>, 
                     typename Divide = divides<T>, 
                     typename Modulus = modulus<T> >
static T powerMod(const T &a, const T &b, const T &c)
{

}


template<typename T>
static T powerMod(T a, T b, T c)
{
    T r = 1;
    while(b)
    {
        if(b & 1) r = r * a % c;
        a = a * a % c;
        b >>= 1;
    }
    return r;
}



} // namespace lz

#endif // UTILITY_H
