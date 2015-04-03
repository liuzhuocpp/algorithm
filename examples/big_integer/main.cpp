#include <iostream>
#include <cstdio>
#include <limits>


#include "big_integer.h"

using namespace std;
using namespace lz;
using namespace lz::BigIntegerPrivate;



int main()
{
    uint a = 1000000000;
    duint b = 10000000000ULL;
    uint c = 0xffffffff;
    cout << c << endl;
    cout << (c + 1) << endl;    
    cout << a << endl;
    cout << b << endl;
    
    // cout << numeric_limits<unsigned>::max() << endl;
    // cout << numeric_limits<unsigned long long>::max() << endl;
    // U<unsigned, unsigned long long >  a, b, c;
    // c = a + b;

    // cout << "SF" << endl;



    return 0;
}