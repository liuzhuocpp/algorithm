#ifndef FFT_H
#define FFT_H

#include <vector>
#include <iostream>
#include <functional>
#include <cmath>
namespace lz {





using std::vector;
using std::cout;
using std::endl;
using std::max;
using std::ostream;
using std::swap;
using std::complex;


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








template<typename Data>
class FFT
{        
    typedef typename Data::Type Type; // structure type   
    
    typedef typename Data::Weight Weight;  // weight function 
    typedef typename Data::InverseWeight InverseWeight; // inverse weight function
    typedef typename Data::Plus Plus;
    typedef typename Data::Divide Divide;
    typedef typename Data::Multiply Multiply;

    static const Weight w;
    static const InverseWeight iw;
    static const Plus plus;
    static const Divide div;
    static const Multiply mul;



    static inline int bitL(int n)
    {
        int t = 1, bn = 0;
        while(t != n) t <<= 1, bn ++;
        return bn;
    }
    template<typename Iterator, typename W>
    static void transform(Iterator a, Iterator end, const W &w) // a[0] + a[1] * x ^ 1 + a[2] * x ^ 2 + ...
    {      
        int n = end - a;
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

        for(int i = 1; i < n; ++ i)
        {
            if(id[i] == 0) continue;
            if(id[i] == i) continue;
            // id[i], id[id[i]];
            swap(a[id[i]], a[i]);
            id[id[i]] = 0;

        }



        delete [] id;

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
                    a[k] = plus(u, mul(wb, v));
                    a[k + l_2] = plus(u, mul(wb, mul(wn_2, v)));
                    wb = mul(wb, w1);
                }
            }
        }
    }

public:
    template<typename IteratorA, typename IteratorB>
    static int multiply(IteratorA a, IteratorA aend, 
                         IteratorB b, IteratorB bend  )
    {        
        int an = aend - a;
        int bn = bend - b;
        int n = 1;
        while(n < max(an, bn)) n <<= 1;
        n <<= 1;

        while(aend - a < n) *(aend++) = Type(0);
        while(bend - b < n) *(bend++) = Type(0);

        transform(a, aend, w);
        transform(b, bend, w);
        for(int i = 0; i < n; ++ i) a[i] = mul(a[i], b[i]);

        transform(a, aend, iw);
        Type inv = div(Type(1), Type(n));
        for(int i = 0; i < n; ++ i) a[i] = mul(a[i], inv);
        return n;
    }

};

template<typename Data>
const typename FFT<Data>::Weight  FFT<Data>::w;

template<typename Data>
const typename FFT<Data>::InverseWeight  FFT<Data>::iw;

template<typename Data>
const typename FFT<Data>::Plus  FFT<Data>::plus;

template<typename Data>
const typename FFT<Data>::Divide  FFT<Data>::div;

template<typename Data>
const typename FFT<Data>::Multiply  FFT<Data>::mul;









// "Integer" is int or unsigned int, long long or unsigned long long
// sequence size must not more than (1 << 27)
// after using fft, every element in sequence should be less than (15 * (1 << 27) + 1)
template<typename Integer> 
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
        Type operator()(int n, int k) const
        {
            return power(g, (P - 1) / n * k, P);
        }
    };
    struct InverseWeight  /// 1/W
    {
        Type operator()(int n, int k) const
        {
            Weight w;
            return w(n, n - k);
        }
    };
    struct Plus
    {
        Type operator()(const Type &a, const Type &b)  const
        {
            return (0uLL + a + b) % P;
        }        
    };
    struct Divide
    {
        Type operator()(const Type &a, const Type &b)  const
        {
            return power(b, P - 2, P) * a % P;
        }   
    };
    struct Multiply
    {
        Type operator()(const Type &a, const Type &b)  const
        {
            return 1uLL * a * b % P;
        }        
    };
};

// "Float" is float, double, long double
// using std::complex<Float> Type
template<typename Float>
struct ComplexFFTData
{
    typedef complex<Float> Type;
    static const Float PI;

    struct Weight
    {    
        Type operator()(int n, int k) const
        {
            Float angle = 2 * PI * k / n;
            return Type(cos(angle), sin(angle));
        }
    };
    struct InverseWeight  /// 1/W
    {
        Type operator()(int n, int k) const
        {
            return Weight()(n, n - k);
        }
    };
    typedef std::plus<Type> Plus;
    typedef std::divides<Type> Divide;
    typedef std::multiplies<Type> Multiply;


};

template<typename Float>
const Float ComplexFFTData<Float>::PI = acos(-1);



} // namespace lz

#endif // FFT_H