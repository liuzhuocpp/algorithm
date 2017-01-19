




//inline unsigned long long f(unsigned long long a, unsigned long long b)
//{
//    return a % b;
//}

inline
unsigned long long lower(unsigned long long a, unsigned long long b)
{
    return a % b;
}


unsigned long long g(unsigned long long a)
{
    return lower(a, (1ULL << 32));
}


//unsigned long long g(unsigned long long a)
//{
//    return a & ~0u;
//}
