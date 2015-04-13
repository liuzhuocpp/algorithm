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

char s[N];
vector<uint> a(N), b(N);
int main()
{    
    
    b.assign(1, 0u);
    while(~scanf("%s", s))
    {
        int n = strlen(s);
        string ss(s);
        fromString(a, ss);


        // out(a);
        // cout << "III" << endl;

        plusAssign(b, a, uint(1e9));
        // out(b);

        // cout << "IIIKKKKK" << endl;
    }

    cout << toString(b) << endl;

    return 0;
}


