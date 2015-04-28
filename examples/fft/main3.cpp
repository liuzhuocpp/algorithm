#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>

#include "fft.h"
using namespace std;
using namespace lz;

typedef unsigned uint ;
typedef unsigned long long ull;
template<typename T> 
int sz(const T &o) { return int(o.size()); }

template<typename Sequence>
void plusAssign(Sequence &a, const Sequence &b, const ull &radix) 
{
    ull t = 0ull;
    int mi = min(sz(a), sz(b));
    for(int i = 0; i < mi; ++ i)
    {
        t += a[i];
        t += b[i];                
        a[i] = t % radix;
        t /= radix;
    }
    if(sz(a) < sz(b))
    {
        for(int i = sz(a); i < sz(b); ++ i) 
        {
            t += b[i];
            a.push_back(t % radix);
            t /= radix;
        }                
    }
    else 
    {
        for(int i = sz(b); t > 0ull && i < sz(a); ++ i)
        {
            t += a[i];
            a[i] = t % radix;
            t /= radix;
        }
    }
    while(t > 0ull) a.push_back(t % radix), t /= radix;
}

template<typename Sequence>
void normalize(Sequence &v, const ull & radix)
{
    ull x = 0;
    for(int i = 0; i < sz(v); ++ i)
    {
        x = x / radix + v[i];
        v[i] = x % radix;
    }            
    while(x > 0) x /= radix, v.push_back(x % radix);

}
template<typename Vector>
void removeLeadingZeros(Vector &v)
{
    while(sz(v) >= 2 && *--v.end() == 0) 
        //v.pop_back();
        v.erase(--v.end());
}
template<typename Vector>
void out(Vector &a)
{            
    cout << "[";
    for(int i = 0; i < sz(a); ++ i) cout << a[i] << "," ; cout << "]" << endl;
}

const int N = 2e5 + 9;
char s[N];
typedef vector<unsigned> Sequence;
Sequence a(N), c(N), d(N), p10(N);


void sol()
{
    a.clear();
    p10.clear();
    p10.push_back(2);
    // p10.push_back(1);

    // int T = 10;
    // while(T --)
    // {
    //     d = p10;
    //     FFT<IntegerFFTData<uint> >::multiply(p10, d);
    //     normalize(p10, 10);
    //     removeLeadingZeros(p10);
    //     out(p10);
    // }
    // return ;


    int n = strlen(s);
    for(int i = n - 1; i >= 0; -- i) a.push_back(s[i] - '0');

    // out(a);
    for(int L = 2; ; L += L)
    {
        // cout << "KK" << endl;
        for(int i = 0; i + L / 2 < n; i += L)
        {
            // i...i+L/2-1;
            // i+L/2...i+L-1;            


            c.clear();
            d.clear();
            int x1 = i, y1 = i + L / 2 - 1;
            int x2 = i + L / 2, y2 = min(n - 1, i + L - 1);
            for(int j = x2; j <= y2; ++ j) c.push_back(a[j]);
            // out(c);
            d = p10;
            FFT<IntegerFFTData<uint> >::multiply(c, d);
            normalize(c, 10);
            removeLeadingZeros(c);


            d.clear();
            for(int j = x1; j <= y1; ++ j) d.push_back(a[j]);
            plusAssign(d, c,10);

            // for(int j = x1; j <= y2; ++ j) a[j] = d[j - x1];
            int j;
            for(j = 0; j < sz(d) && j + x1 <= y2; ++ j) a[j + x1] = d[j];
            for(;j + x1 <= y2; ++j) a[j + x1] = 0;
        }

        if(L >= n) break;
        d = p10;
        FFT<IntegerFFTData<uint> >::multiply(p10, d);
        normalize(p10, 10);
        removeLeadingZeros(p10);
        // out(p10);

    }
    removeLeadingZeros(a);

}





int main()
{
    int L = 50000;
    for(int i = 0; i < L; ++ i) s[i] = '0';
    s[L] = 0;
    s[0] = '1';
    // cout << s << endl;
    sol();
    
    for(int i = sz(a) - 1; i >= 0; -- i) s[sz(a) - 1 - i] = a[i] + '0';
    s[sz(a)] = 0;
    puts(s);
    return 0;
}