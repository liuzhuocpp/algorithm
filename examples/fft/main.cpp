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

// typedef double Float;
typedef float Float;
const Float PI = acos(-1);
typedef complex<Float> C;
struct W
{
    C operator()(int n, int k)
    {
        Float ang = 2 * PI * k / n;
        return C(cos(ang), sin(ang));
    }
};

struct NW
{
    C operator()(int n, int k)
    {
        return W()(n, n - k);
        Float ang = 2 * PI * k / n;
        return Float(1.0) / C(cos(ang), sin(ang));
    }
};






const int N = 2e5 + 9;
char s1[N], s2[N];
char ans[N];
vector<C> a(N), b(N), o(N);
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
        // cout << "CAO " << endl;
        int an = strlen(s1);
        int bn = strlen(s2);

        int n = 1;
        while(n < max(an, bn)) n <<= 1;
        n <<= 1;
        // a.assign(n, C());
        // b.assign(n, C());
        // o.assign(n, C());
        a.clear();
        b.clear();
        o.clear();
        // cout << "DUI" << endl;
        reverse(s1, s1 + an);
        reverse(s2, s2 + bn);
        for(int i = 0; s1[i]; ++ i) a.push_back((s1[i] - '0') * 1.0);
        for(int i = 0; s2[i]; ++ i) b.push_back((s2[i] - '0') * 1.0);


        o = FFT<C, W>::multiply(a, b);

   
        // while(a.size() < n) a.push_back(0);
        // while(b.size() < n) b.push_back(0);
        // FFT<C, W>::transform(a);
        // FFT<C, W>::transform(b);
        // for(int i = 0; i < n; ++ i) o.push_back(a[i] * b[i]);       
        // FFT<C, NW>::transform(o);
        // for(int i = 0; i < n; ++ i) o[i] /= n;
        

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


