#include <iostream>
#include <cstdio>
#include <limits>


#include "big_integer.h"

using namespace std;
using namespace lz;
using namespace lz::BigIntegerPrivate;


const int N = 10000;
char s[N];

int main()
{
    U a;
    while(~scanf("%s", s))
    {
        U b(s);
        a.addAssign(b);
    }
    cout << a.toString() << endl;

    return 0;
}