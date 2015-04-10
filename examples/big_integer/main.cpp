#include <iostream>
#include <cstdio>
#include <limits>
#include <vector>
#include <complex>
#include <cmath>
#include <algorithm>
#include <cstring>

#include "big_integer.h"

using namespace std;
using namespace lz;
using namespace lz::BigIntegerPrivate;



const int N = 2e5 + 9;
char s1[N], s2[N];

U a, b, c;
int main()
{

    while(~scanf("%s%s", s1, s2))
    {

        int an = strlen(s1);
        int bn = strlen(s2);
        a.clear();
        b.clear();
        fromString(a, s1, an);
        fromString(b, s2, bn);
        c.clear();
        multiply(c, a, b);

        string o;
        o.reserve(N);
        printf("%s\n", toString(o, c).c_str());






        


    }
    return 0;
}


