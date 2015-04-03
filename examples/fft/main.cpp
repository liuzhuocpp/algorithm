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

const double PI = acos(-1);
typedef complex<double> C;
struct W
{
    C operator()(int n, int k)
    {
        double ang = 2 * PI * k / n;
        return C(cos(ang), sin(ang));
    }
};
struct NW
{
    C operator()(int n, int k)
    {
        double ang = 2 * PI * k / n;
        return 1.0 / C(cos(ang), sin(ang));
    }
};

const int N = 1e5 + 9;
char s1[N], s2[N];
char ans[N];
vector<C> a, b, o;
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
    W w;
    w(1, 2);
    
    while(~scanf("%s%s", s1, s2))
    {
        int an = strlen(s1);
        int bn = strlen(s2);

        int n = 1;
        while(n < max(an, bn)) n <<= 1;
        n <<= 1;
        a.assign(n, C());
        b.assign(n, C());
        o.assign(n, C());

        reverse(s1, s1 + an);
        reverse(s2, s2 + bn);
        for(int i = 0; s1[i]; ++ i) a[i] = (s1[i] - '0') * 1.0;
        for(int i = 0; s2[i]; ++ i) b[i] = (s2[i] - '0') * 1.0;
        while(an < n) a[an ++] = 0;
        while(bn < n) b[bn ++] = 0;


        // out(a);
        // out(b);

        // cout << "FF " << endl;
        a = FFT<C, W>::transform(a);
        // cout << "FFPP " << endl;
        b = FFT<C, W>::transform(b);
        // cout << "FF " << endl;



        for(int i = 0; i < n; ++ i) o[i] = a[i] * b[i];

        // out(a);
        // out(b);
        // out(o);



        o = FFT<C, NW>::transform(o);
        for(int i = 0; i < n; ++ i) o[i] /= n;
        

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


