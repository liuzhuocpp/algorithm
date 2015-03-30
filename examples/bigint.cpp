#include <iostream>
#include <cstdio>
#include <limits>
#include <big_integer.h>

using namespace std;
using namespace lz;
using namespace lz::BigIntegerPrivate;



int main()
{
    cout << numeric_limits<unsigned>::max() << endl;
    cout << numeric_limits<unsigned long long>::max() << endl;
    U<unsigned, unsigned long long >  a, b, c;
    c = a + b;

    cout << "SF" << endl;



    return 0;
}