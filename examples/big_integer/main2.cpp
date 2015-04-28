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

string ss("102973498273");

typedef  vector<uint> Sequence;
Sequence zero(1, 0);
ull radix = 1e9;
int L = 9;
void out2(ull x)
{
    cout << "[";
    while(x)
    {
        if(x & 1) cout << 1;
        else cout << 0;        
        cout << ",";
        x >>= 1;
    }
    cout << "]" << endl;
}

void out2(const Sequence & ta)
{
    Sequence a = ta;

    // cout << "[";
    while(cmp(a, zero) > 0)
    {
        uint mod = divide(a, 2, radix);
        // cout << mod;
        // cout << ",";        
    }
    cout << "]" << endl;
}

int main()
{
    ss.assign(100000, '1');
    Sequence a;
    

    fromString(a, ss, L);




    tuple<Sequence,Sequence> b = radixTransform(0, sz(a) - 1, a, int(1e9), 2, 2, 1);

    // cout << toString(get<0>b, L) << endl;
    // out(get<0>(b));
    // out2(a);


   


    return 0;
}


