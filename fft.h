#ifndef FFT_H
#define FFT_H

#include <vector>
#include <iostream>
namespace lz {





using std::vector;
using std::cout;
using std::endl;
using std::max;
using std::ostream;

/*
****0.w(n, k), k = {0, 1, ..., n - 1} is different from each other
****1.n is power of 2
****2.w(n, 2 * k) == w(n / 2, k), k = {0, 1, ..., n /2 - 1}
****3.w(n, k) == w(n, k + n), k = {0, 1, ..., n - 1}
  **4.w(n, 0) == 1
    5.w(n, k) == -w(n, k + n / 2), 0 < k < n/2

    [w(n, 0*0),     w(n, 0*1), ...,    w(n, 0*(n - 1))]        a(0)       y(0)
    [w(n, 1*0),     w(n, 1*1), ...,    w(n, 1*(n - 1))]        a(1)       y(1)
        .
        .                                                   =
        .
    [w(n, (n-1)*0), w(n, (n-1)*1), ...,w(n, (n-1)*(n-1))]      a(n-1)     y(n - 1)
*/

    namespace FFTPrivate {

        inline int bitL(int n)
        {
            int t = 1, bn = 0;
            while(t != n) t <<= 1, bn ++;
            return bn;
        }
        template<typename T, typename W, typename Vector>
        void transform(Vector &a) // a[0] + a[1] * x ^ 1 + a[2] * x ^ 2 + ...
        {
            W w;
            Vector y(a);
            int n = a.size();
            int bn = bitL(n);

            int *id = new int[n];
            id[0] = 0;
            for(int i = 0; i < bn; ++ i)
            {
                for(int s = 0; s < (1 << i); ++ s)
                {
                    int ns = s | (1 << i);
                    id[ns] = id[s] | (1 << (bn - 1 - i));
                }
                
            }        
            for(int i = 0; i < n; ++ i) a[id[i]] = y[i];
            delete [] id;

            for(int l = 2; l <= n; l <<= 1)
            {
                int l_2 = l >> 1;
                T w1 = w(l, 1);
                T w0 = w(l, 0);
                T wn_2 = w(l, l_2);

                for(int i = 0; i < n; i += l)
                {
                    T wb = w0;
                    for(int k = i; k < i + l_2; ++ k)
                    {                    
                        T u = a[k], v = a[k + l_2];
                        a[k] = u + wb * v;
                        a[k + l_2] = u + wb * wn_2 * v;
                        wb = wb * w1;
                    }
                }
            }
        }
    }


template<typename Data, typename Vector = vector<typename Data::T> >
class FFT
{        
    typedef typename Data::T T;
public:
    static void multiply(Vector &a, Vector &b)
    {
        int an = a.size();
        int bn = b.size();
        int n = 1;
        while(n < max(an, bn)) n <<= 1;
        n <<= 1;
        while(a.size() < n) a.push_back(T(0));
        while(b.size() < n) b.push_back(T(0));
        FFTPrivate::transform<typename Data::T, typename Data::W, Vector>(a);
        FFTPrivate::transform<typename Data::T, typename Data::W, Vector>(b);        
        for(int i = 0; i < n; ++ i) a[i] = a[i] * b[i];
        FFTPrivate::transform<typename Data::T, typename Data::NW, Vector>(a);

        for(int i = 0; i < n; ++ i) a[i] = a[i] / T(n);        
    }

};


class IntegerFFTData
{

public:
    typedef long long LL;
    const static LL P = (7 << 26) + 1;
    const static LL g = 3;

    static LL power(LL a, LL b, LL c = P)
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
public:

    struct T
    {
        int i;
        T(LL _i = 0):i(_i){}
        friend T operator*(const T &a, const T &b)
        {
            return T(1LL * a.i * b.i % P);
        }
        friend T operator+(const T &a, const T &b)
        {
            return T((0LL + a.i + b.i) % P);
        }
        friend T operator/(const T &a, const T &b)
        {
            return T(a.i * power(b.i, P - 2) % P);
        }
        friend ostream& operator<<(ostream& out, const T &o)
        {
            out << o.i;
            return out;
        }
    };

    struct W
    {    
        T operator()(int n, int k)
        {
            return T(power(g, (P - 1) / n * k));
        }
    };

    struct NW  /// 1/W
    {
        T operator()(int n, int k)
        {
            W w;
            return w(n, n - k);
        }
    };

};




} // namespace lz

#endif // FFT_H