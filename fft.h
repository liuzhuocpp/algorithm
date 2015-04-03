#ifndef FFT_H
#define FFT_H

#include <vector>
#include <iostream>
namespace lz {

using std::vector;
using std::cout;
using std::endl;



/*
    0.n is power of two
    1.w(n, 0) == 1
    2.w(n, k), k = {0, 1, ..., n - 1} is different from each other
    3.w(n, k) == w(n, k + n), k = {0, 1, ..., n - 1}
****4.w(n, 2 * k) == w(n / 2, k), k = {0, 1, ..., n /2 - 1}
****5.w(n, k) == -w(n, k + n / 2)
    [w(n, 0 * 0),       w(n, 0 * 1), ...,       w(n, 0 * (n - 1))]       a(0)       y(0)
    [w(n, 1 * 0),       w(n, 1 * 1), ...,       w(n, 1 * (n - 1))]       a(1)       y(1)
        .
        .                                                                         =
        .
    [w(n, (n - 1) * 0), w(n, (n - 1) * 1), ..., w(n, (n - 1) * (n - 1))] a(n-1)     y(n - 1)




*/

template<typename T, typename W>
class FFT
{    
    static int rev(int n, int x)
    {
        int r = 0;
        for(int i = 0; i < n; ++ i)       
            if(x >> i & 1) r |= 1 << (n - 1 - i);
        return r;
    }
public:
    static vector<T> transform(const vector<T> &a) // a[0] + a[1] * x ^ 1 + a[2] * x ^ 2 + ...
    {
        W w;
        int bn = 0, t = 1, n = a.size();
        while(t != n) t <<= 1, bn ++;

        // cout << bn << " " << n << endl;
        vector<T> y(n);

        for(int i = 0; i < n; ++ i) y[rev(bn, i)] = a[i];
        for(int l = 2; l <= n; l <<= 1)
        {
            for(int i = 0; i < n; i += l)
            {
                for(int k = i; k < i + l / 2; ++ k)
                {
                    T y0 = y[k];
                    T y1 = y[k + l / 2];

                    y[k] = y0 + w(l, k) * y1;
                    y[k + l / 2] = y0 + w(l, k + l / 2) * y1;
                }
            }
        }
        return y;
    }

};




} // namespace lz

#endif // FFT_H