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

// "T" should be integer
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
