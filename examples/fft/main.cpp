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
const int N = 2e5 + 9;
char s1[N], s2[N], ans[N];

#define sz(x) int(x.size())
typedef complex<double> T;
vector<T> a(N), b(N);
int main()
{

    while(~scanf("%s%s", s1, s2))
    {
        int an = strlen(s1);
        int bn = strlen(s2);

        
        reverse(s1, s1 + an);
        reverse(s2, s2 + bn);
        for(int i = 0; s1[i]; ++ i) a[i] = s1[i] - '0';
        for(int i = 0; s2[i]; ++ i) b[i] = s2[i] - '0';

        // cout << "vector:" << endl;
        // for(int i = 0; i < sz(a); ++ i) cout << a[i]; cout << endl;

        // cout << "vector:" << endl;  
        // for(int i = 0; i < sz(b); ++ i) cout << b[i]; cout << endl;


        int n = FFT<ComplexFFTData<double> >::multiply(a.begin(), a.begin() + an, 
                                                 b.begin(), b.begin() + bn);

        // FFT<IntegerFFTData<unsigned> > ::multiply(a, b);

        // cout << "vector:" << endl;
        // for(int i = 0; i < sz(a); ++ i) cout << a[i]; cout << endl;


        vector<T> &o = a;
        // int n = o.size();


        int cnt = 0;
        for(int i = 0; i < n; ++ i)
        {
            cnt = cnt / 10 + int(o[i].real() + 0.1);
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


