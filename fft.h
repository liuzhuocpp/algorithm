#ifndef FFT_H
#define FFT_H

#include <vector>
#include <iostream>
namespace lz {





using std::vector;
using std::cout;
using std::endl;
using std::max;


/*
****0.w(n, k), k = {0, 1, ..., n - 1} is different from each other
****1.n is power of 2
****2.w(n, 2 * k) == w(n / 2, k), k = {0, 1, ..., n /2 - 1}
****3.w(n, k) == w(n, k + n), k = {0, 1, ..., n - 1}
  **4.w(n, 0) == 1
    5.w(n, k) == -w(n, k + n / 2), 0 < k < n/2

    [w(n, 0*0),     w(n, 0*1), ...,    w(n, 0*(n - 1))]       a(0)       y(0)
    [w(n, 1*0),     w(n, 1*1), ...,    w(n, 1* (n - 1))]       a(1)       y(1)
        .
        .                                                   =
        .
    [w(n, (n-1)*0), w(n, (n-1)*1), ...,w(n, (n-1)*(n-1))]      a(n-1)     y(n - 1)




*/




template<typename T, typename W, typename Vector = vector<T> >
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
    static void transform(Vector &a) // a[0] + a[1] * x ^ 1 + a[2] * x ^ 2 + ...
    {
        W w;
        int bn = 0, t = 1, n = a.size();
        while(t != n) t <<= 1, bn ++;

        Vector y(a);
        for(int i = 0; i < n; ++ i) a[rev(bn, i)] = y[i];
        for(int l = 2; l <= n; l <<= 1)
        {
            T wn = w(l, 1);
            T wn1 = w(l, l / 2);
            for(int i = 0; i < n; i += l)
            {
                T wb = w(l, 0);
                for(int k = i; k < i + l / 2; ++ k)
                {                    
                    T u = a[k], v = a[k + l / 2];
                    a[k] = u + wb * v;
                    a[k + l / 2] = u + wb * wn1 * v;
                    wb = wb * wn;
                }
            }
        }
    }

    struct NW
    {
        T operator()(int n, int k) // k = 0, 1,..., n - 1
        {
            W w;            
            return w(n, n - k);
        }
    };

    static Vector multiply(Vector &a, Vector &b)
    {
        int an = a.size();
        int bn = b.size();
        int n = 1;
        while(n < max(an, bn)) n <<= 1;
        n <<= 1;
        while(a.size() < n) a.push_back(T(0));
        while(b.size() < n) b.push_back(T(0));

        // out(a);
        // out(b);
        transform(a);
        transform(b);
        // cout << "J "<< endl;
        // for(int i = 0; i < n; ++ i)
        // {
        //     cout << a[i] << " " ;
        // }
        // cout << endl;
        // cout << "J "<< endl;
        // for(int i = 0; i < n; ++ i)
        // {
        //     cout << b[i] << " " ;
        // }
        // cout << endl;


        
        Vector c(n);
        for(int i = 0; i < n; ++ i) c[i] = a[i] * b[i];

        FFT<T, typename FFT<T, W, Vector>::NW, Vector>::transform(c);

        // cout << "JJ "<< endl;
        // for(int i = 0; i < n; ++ i)
        // {
        //     cout << c[i].i << " " ;
        // }
        // cout << endl;


        for(int i = 0; i < n; ++ i) c[i] = c[i] / T(n);
        return c;
    }

};






} // namespace lz

#endif // FFT_H