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


typedef IntegerFFTData::T C;

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
vector<C> a(N), b(N);
int main()
{

    while(~scanf("%s%s", s1, s2))
    {

        int an = strlen(s1);
        int bn = strlen(s2);
        a.clear();
        b.clear();
        
        reverse(s1, s1 + an);
        reverse(s2, s2 + bn);
        for(int i = 0; s1[i]; ++ i) a.push_back((s1[i] - '0'));
        for(int i = 0; s2[i]; ++ i) b.push_back((s2[i] - '0'));

        // int an = a.size();
        // int bn = b.size();
        // int n = 1;
        // while(n < max(an, bn)) n <<= 1;
        // n <<= 1;
        // while(a.size() < n) a.push_back(C(0));
        // while(b.size() < n) b.push_back(C(0));

        // FFT<C, W>::transform(a);
        // FFT<C, W>::transform(b);

        // for(int i = 0; i < n; ++ i) a[i] = a[i] * b[i];

        // vector<C> &c = a;
        // FFT<C, NW>::transform(c);

        // for(int i = 0; i < n; ++ i) c[i] = c[i] / C(n);

        // vector<C> o = FFT<C, NW>::multiply(a, b);
        // out(o);



        // n = c.size();
        FFT<IntegerFFTData>::multiply(a, b);
        vector<C> &c = a;




        int n = c.size();
        cout << "CAO " << endl;
        for(int i = 0; i < n; ++ i) cout << c[i] ; cout << endl;




        int cnt = 0;
        for(int i = 0; i < n; ++ i)
        {
            cnt = cnt / 10 + c[i].i;
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


