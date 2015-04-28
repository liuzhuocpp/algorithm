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
int L = 7;
int main()
{
    while(~scanf("%s%s", s1, s2))
    {
        a.clear();
        b.clear();
        string ss1(s1), ss2(s2);
        fromString(a, ss1, L);
        fromString(b, ss2, L);
        // out(a);
        // out(b);
        // minusAssign(a, b, int(1e9));
        // fftMultiply(a, b, 10, 9);

        // simulationMultiply(c, a, b, int(1e9));
        // cout << toString(c) << endl;
        
        // multiply(a, b, int(1e9), 10, 9);
        // cout << toString(a) << endl;

        multiply(c, a, b, int(1e7), 10, L);
        cout << toString(c, L) << endl;
        
    }
    


    return 0;
}


