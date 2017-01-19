

#include <functional>


//inline unsigned long long f(unsigned long long a, unsigned long long b)
//{
//    return a % b;
//}

template<typename Mod = std::modulus<unsigned long long> >
//inline
unsigned long long lower(unsigned long long a, unsigned long long b, Mod mod = std::modulus<unsigned long long>())
{
//    return mod(a);
    return mod(a , b);
//    return a % b;
}


unsigned long long g(unsigned long long a)
{
    return lower(a, (1ULL << (sizeof(int) * 8)));
//    return lower(a, (1ULL << 32));
}


//unsigned long long g(unsigned long long a)
//{
//    return a & ~0u;
//}
