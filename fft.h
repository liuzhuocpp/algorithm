#ifndef FFT_H
#define FFT_H

#include <vector>
#include <iostream>
#include <functional>
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

    typedef unsigned long long ull;
    // judge  whether "x" is the prime
    bool isPrime(const ull &x)
    {
        for(ull i = 2; i <= x / i; ++ i)
        {
            if(x % i == 0) return 0;
        }
        return 1;
    }

    template<typename T>
    static T powerMod(T a, T b, T c)
    {
        T r = 1;
        while(b)
        {
            if(b & 1) r = r * a % c;
            a = a * a % c;
            b >>= 1;
        }
        return r;
    }

    // get the primary root for prime "p"
    ull getG(const ull &p)
    {
        for(ull g = 2; ; ++ g)
        {
            ull t = p - 1;
            bool ok = 1;
            for(ull i = 2; i <= t / i; ++ i)
            {
                if(t % i == 0)
                {
                    while(t % i == 0) t /= i;
                    if(powerMod(g, p / i, p) == 1)
                    {
                        ok = 0; break;
                    }
                }
            }
            if(ok && t > 1)
            {
                if(powerMod(g, p / t, p) == 1) ok = 0;
            }
            if(ok) return g;
        }
    }

    void get(ull i)
    {
        ull n = 1ull << i;
        for(ull k = 1; ; k ++)
        {
            if(isPrime(k * n + 1))
            {
                cout<< (k * n + 1) <<" " << k << " "  << getG(k * n + 1) << endl;
                break;
            }
        }
    }



        


    }


template<typename Data, typename Vector = vector<typename Data::Type> >
class FFT
{        
    typedef typename Data::Type Type; // structure type
    typedef typename Data::Weight Weight;  // weight function 
    typedef typename Data::InverseWeight InverseWeight; // inverse weight function

    typedef typename Data::Plus Plus;
    typedef typename Data::Divide Divide;
    typedef typename Data::Multiply Multiply;


    static inline int bitL(int n)
    {
        int t = 1, bn = 0;
        while(t != n) t <<= 1, bn ++;
        return bn;
    }
    template<typename W>
    static void transform(Vector &a) // a[0] + a[1] * x ^ 1 + a[2] * x ^ 2 + ...
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

        Plus plus;
        Multiply multiply;
        for(int l = 2; l <= n; l <<= 1)
        {
            int l_2 = l >> 1;
            Type w1 = w(l, 1);
            Type w0 = w(l, 0);
            Type wn_2 = w(l, l_2);

            for(int i = 0; i < n; i += l)
            {
                Type wb = w0;
                for(int k = i; k < i + l_2; ++ k)
                {                    
                    Type u = a[k], v = a[k + l_2];
                    // a[k] = u + wb * v;
                    // a[k + l_2] = u + wb * wn_2 * v;
                    // wb = wb * w1;
                    a[k] = plus(u, multiply(wb, v));
                    a[k + l_2] = plus(u, multiply(wb, multiply(wn_2, v)));
                    wb = multiply(wb, w1);
                }
            }
        }
    }

public:
    static void multiply(Vector &a, Vector &b)
    {
        Multiply multiply;
        int an = a.size();
        int bn = b.size();
        int n = 1;
        while(n < max(an, bn)) n <<= 1;
        n <<= 1;
        while(a.size() < n) a.push_back(Type(0));
        while(b.size() < n) b.push_back(Type(0));

        transform<Weight>(a);
        transform<Weight>(b);
        for(int i = 0; i < n; ++ i) a[i] = multiply(a[i], b[i]);

        transform<InverseWeight>(a);

        Type inv = Divide()(Type(1), Type(n));
        for(int i = 0; i < n; ++ i) a[i] = multiply(a[i], inv);
    }

};





template<typename Integer> // Integer is int or unsigned int, long long or unsigned long long
class IntegerFFTData
{
    typedef unsigned long long LL;
    // const static LL P = (7 << 26) + 1;
    // const static LL g = 3;
    const static LL P = (15 * (1 << 27) + 1);
    const static LL g = 31;


    static LL power(LL a, LL b, LL c)
    {
        LL r = 1uLL;
        while(b)
        {
            if(b & 1uLL) r = r * a % c;
            a = a * a % c;
            b >>= 1uLL;
        }
        return r;
    }
public:

    typedef Integer Type;
    struct Weight
    {    
        Type operator()(int n, int k)
        {
            return power(g, (P - 1) / n * k, P);
        }
    };
    struct InverseWeight  /// 1/W
    {
        Type operator()(int n, int k)
        {
            Weight w;
            return w(n, n - k);
        }
    };
    struct Plus
    {
        Type operator()(const Type &a, const Type &b)
        {
            return (0uLL + a + b) % P;
        }        
    };
    struct Divide
    {
        Type operator()(const Type &a, const Type &b)
        {
            return power(b, P - 2, P) * a % P;
        }   
    };
    struct Multiply
    {
        Type operator()(const Type &a, const Type &b)
        {
            return 1uLL * a * b % P;
        }        
    };



};




} // namespace lz

#endif // FFT_H