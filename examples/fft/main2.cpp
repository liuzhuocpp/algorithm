#include <iostream>
#include <cstdio>
#include <limits>
#include <vector>
#include <complex>
#include <cmath>
#include <algorithm>
#include <cstring>

#include "fft.h"

using namespace std;
using namespace lz;

#define LL long long
const int NN = 28;
int PP[NN] = {
2,
3,
5,
17,
17,
97,
193,
257,
257,
7681,
12289,
12289,
12289,
40961,
65537,
65537,
65537,
786433,
786433,
5767169,
7340033,
23068673,
104857601,
167772161,
167772161,
167772161,
469762049,
2013265921, };



LL P = (479 << 21) + 1;
LL g = 3;

struct C
{
    int i;
    // template<typename T> 
    C(LL _i = 0):i(_i){}    
    
    friend C operator*(const C &a, const C &b)
    {
        return C(1LL * a.i * b.i % P);
    }
    friend C operator+(const C &a, const C &b)
    {
        return C((0LL + a.i + b.i) % P);
    }
    friend C operator/(const C &a, const C &b)
    {
        return C(a.i / b.i);
    }
    friend ostream& operator<<(ostream& out, const C &o)
    {
        out << o.i;
        return out;
    }
};


LL power(LL a, LL b, LL c = P)
{
    LL r = 1;
    while(b)
    {
        if(b & 1) r = r * a % c;
        a = a * a % c;
        b >>= 1;
    }
    return r;
}


struct W
{    
    C operator()(int n, int k)
    {
        return power(g, (P - 1) / n * k);
    }
};







const int N = 2e5 + 9;
char s1[N], s2[N];
char ans[N];

void out(vector<C> &v)
{
    cout << "vector: " << v.size() << endl;
    for(int i = 0; i < v.size(); ++ i)
    {
        cout << v[i] << " " ;
    }
    cout << endl;
}
int main()
{

    while(~scanf("%s%s", s1, s2))
    {

        int an = strlen(s1);
        int bn = strlen(s2);

        vector<C> a, b, o;
        reverse(s1, s1 + an);
        reverse(s2, s2 + bn);
        for(int i = 0; s1[i]; ++ i) a.push_back((s1[i] - '0'));
        for(int i = 0; s2[i]; ++ i) b.push_back((s2[i] - '0'));

        // out(a);
        // out(b);

        o = FFT<C, W>::multiply(a, b);
        // out(o);



        int n = o.size();
        int cnt = 0;
        for(int i = 0; i < n; ++ i)
        {
            cnt = cnt / 10 + o[i].i;
            ans[i] =  cnt % 10;
        }
        int L = n;
        while(L > 1 && ans[L - 1] == 0) L --;
        for(int i = L - 1; i >= 0; -- i)
        {
            printf("%d", ans[i]);
        }
        puts("");

    }
    return 0;
}


