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
vector<unsigned> a(N), b(N), c(N);
int main()
{
    while(~scanf("%s%s", s1, s2))
    {
        a.clear();
        b.clear();
        string ss1(s1), ss2(s2);
        fromString(a, ss1);
        fromString(b, ss2);
        // out(a);
        // out(b);
        // minusAssign(a, b, int(1e9));
        // fftMultiply(a, b, 10, 9);

        // simulationMultiply(c, a, b, int(1e9));
        // cout << toString(c) << endl;
        
        // multiply(a, b, int(1e9), 10, 9);
        // cout << toString(a) << endl;

        multiply(c, a, b, int(1e9), 10, 9);
        cout << toString(c) << endl;
        
    }
    


    return 0;
}


