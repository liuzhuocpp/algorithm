#ifndef BIGINTEGER_H
#define BIGINTEGER_H


#include <limits>
#include <vector>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <utility>

// #include "fft.h"


namespace lz {

using std::vector;
using std::numeric_limits;
using std::cout;
using std::endl;
using std::string;
using std::strlen;
using std::max;
using std::min;

using std::pair;
using std::make_pair;

    namespace BigIntegerPrivate {

    /**
     * Class U is the unsigned BigInteger.
     * Use little endian to storage the word sequence.
     * The length of a word sequence is at least 1(specially for zero).
     * a[0], a[1], a[2]... radix bits increase
     *        --> 
     */

    /*
     * some convenient functions     
     */
    template<typename T> static int sz(const T &o) { return o.size(); }

    
    /**
     * this type is used to hold a word
     */    
    typedef unsigned int uint;

    /**
     * this type is used to hold two words
     */
    typedef unsigned long long ull;

    /**
     * this type is used to hold word sequence
     */
    typedef vector<uint> WordSeq;


    /**
     * This constant is the radix of every word
     */
    static const ull WordRadix = 0xffffffffull + 1ull;

    /**
     * This constant is used to get lower word
     */
    static const ull WordMask = ~0u;

    /**
     * This constant is used to represent to zero
     */
    const WordSeq ZeroWordSeq(1, 0);

    /**
     * This constant is used to represent to zero
     */
    const WordSeq OneWordSeq(1, 1);

    /**
     * Get maximum radix that a word can hold.
     * And radix ^ l = max_radix
     * @pram radix need be calculated for max radix
     * @pram l.
     * @pram max_radix the result max radix.
     */
    void getMaxRadix(uint radix, uint &l, uint &max_radix)
    {
        ull tmax_radix = 1;
        l = 0;        
        while(tmax_radix * radix <= WordMask) 
            tmax_radix *= radix, l ++;
        max_radix = tmax_radix;
    }


    /**
     * Output the contents of WordSeq o
     * @param o need be output
     */
    void out(const WordSeq & o)
    {
        cout << "[";
        for(int i = 0; i < sz(o); ++ i)
        {
            cout << o[i]<< ",";
        }
        
        cout << "]"<< endl;
    }


    /**
     * Remove leading zeros in word sequence a
     * @param a is the word sequence
     */
    static void removeLeadingZeros(WordSeq &a)
    {
        while(sz(a) >= 2 && *--a.end() == 0) 
            //v.pop_back();
            a.erase(--a.end());
    }

    /**
     * get uint from char
     * @param x need be translated char
     */
    static uint fromChar(char x)
    {
        if(x >= '0' && x <= '9') return x - '0';
        if(x >= 'a' && x <= 'z') return x - 'a' + 10;
        if(x >= 'A' && x <= 'Z') return x - 'A' + 10;
    }


    /**
     * get char from uint
     * @param x need be translated uint
     */
    static char toChar(uint x)
    {
        if(x >= 0 && x <= 9) return x + '0';
        return x + 'a';
    }

    /**
     * Adds the contents of the WordSeq a and b. And put the result into a : a += b.
     * @param  a add value.
     * @param  b value to be added to a.     
     */
    static void plus(WordSeq &a, const WordSeq &b)
    {
        ull t = 0ull;
        int mi = min(sz(a), sz(b));
        for(int i = 0; i < mi; ++ i)
        {
            t += a[i];
            t += b[i];                
            a[i] = t % WordRadix;
            t /= WordRadix;
        }
        if(sz(a) < sz(b))
        {
            for(int i = sz(a); i < sz(b); ++ i) 
            {
                t += b[i];
                a.push_back(t % WordRadix);
                t /= WordRadix;
            }                
        }
        else 
        {
            for(int i = sz(b); t > 0ull && i < sz(a); ++ i)
            {
                t += a[i];
                a[i] = t % WordRadix;
                t /= WordRadix;
            }
        }
        while(t > 0ull) a.push_back(t % WordRadix), t /= WordRadix;
    }

    /**
     * Adds the contents of the WordSeq a and Word b. 
     * And put the result into a : a += b.
     * @param  a add value.
     * @param  b value to be added to a.     
     */
    static void plus(WordSeq &a, uint b)
    {
        ull t = b;
        for(int i = 0; i < sz(a); ++ i)
        {
            if(t == 0) break;
            t += a[i];
            a[i] = t & WordMask;
            t >>= 32;
        }
        if(t > 0) a.push_back(t & WordMask);
    }


    /**
     * Multiply the contents of the WordSeq a and b using school method: c = a * b.
     * And put the result into c.
     * @param  a the multiply value.
     * @param  b value to be multiplied to a.
     */        
    static void multiplySchool(const WordSeq &a, const WordSeq &b, WordSeq &c)
    {
        c.assign(sz(a) + sz(b), 0);
        for(int i = 0; i < sz(b); ++ i)
        {
            ull t = 0;
            for(int j = 0; j < sz(a); ++ j)
            {
                t += 1ull * b[i] * a[j] + c[i + j];
                c[i + j] = t % WordRadix;
                t /= WordRadix;
            }
            if(t > 0)
            {
                c[i + sz(a)] = t;
            }
        }
        if(c.back() == 0) c.pop_back();
    }


    /**
     * Multiply the contents of the WordSeq a and Word b using school method:a *= b.         
     * @param  a the multiply value.
     * @param  b value to be multiplied to a.
     */        
    static void multiplySchool(WordSeq &a, uint b)
    {
        ull t = 0ull;
        for(int i = 0; i < sz(a); ++ i)
        {
            t += (a[i] & WordMask) * b;
            a[i] = t & WordMask;
            t >>= 32;
        }
        if(t > 0ull) a.push_back(t & WordMask);
    }




    /**
     * minus the contents of the WordSeq b from a: a -= b.
     * And put the result into a.
     * @param  a value to be minuend by b.
     * @param  b the minus value.
     */
    static void minus(WordSeq &a, const WordSeq &b) 
    {
        ull t = 0;
        for(int i = 0; i < sz(b); ++ i)
        {                    
            t += b[i];
            if(a[i] < t) 
                a[i] = WordRadix + a[i] - t, t = 1;
            else 
                a[i] -= t, t = 0;
        }
        for(int i = sz(b); t > 0ull && i < sz(a); ++ i)
        {
            if(a[i] < t) 
                a[i] = WordRadix + a[i] - t, t = 1;
            else 
                a[i] -= t, t = 0;
        }
        removeLeadingZeros(a);
    }


    /**
     * Divide the contents of the WordSeq a and Word b using school method:a /= b.
     * @param  a the divided value by b.
     * @param  b the divide value.
     * @return the remainder a % b
     */        
    static uint divideAndRemainderSchool(WordSeq &a, uint b)
    {
        ull t = 0;
        for(int i = sz(a) - 1; i >= 0; -- i)
        {
            t = t << 32;
            t |= a[i];
            a[i] = t / b;
            t %= b;               
        }
        removeLeadingZeros(a);
        // cout <<"!";
        // out(a);
        return t;
    }


    /**
     * compare the contents of the WordSeq a and b.     
     * @param  a first WordSeq used to compare
     * @param  b second WordSeq used to compare
     * @return  if a > b: return 1;
     *          if a == b: return 0;
     *          if a < b: return -1; 
     */
    static int compare(const WordSeq &a, const WordSeq &b)
    {
        if(sz(a) < sz(b)) return -1;
        if(sz(a) > sz(b)) return 1;
        for(int i = sz(a) - 1; i >= 0; -- i)
        {
            if(a[i] < b[i]) return -1;
            if(a[i] > b[i]) return 1;
        }
        return 0;

    }


    /**
     * The input string should be big endian
     * @param s the first of input string
     * @param end the end of input string
     * @param radix the radix of input string 
     * @param a the result of translated word sequence
     */
    template<typename Iterator>
    static void fromString(Iterator s, Iterator end, uint radix, WordSeq &a)
    {
        a.clear();
        int n = end - s;
        uint max_radix, l;
        getMaxRadix(radix, l, max_radix);            

        int begin_pos = n % l;
        uint x = 0;
        for(int i = 0; i < begin_pos; ++ i)
        {
            x = x * radix + fromChar(s[i]);
        }
        a.push_back(x);
        for(int i = begin_pos; i < n; i += l)
        {
            uint x = 0;
            for(int j = i; j < i + l; ++ j)
            {
                x = x * radix + fromChar(s[j]);
            }
            multiplySchool(a, max_radix);            
            plus(a, x);                
        }
        removeLeadingZeros(a);
    }

    /**
     * Translate word a to string representation with little endian
     * @param  a  word that will be translated
     * @param  radix  radix of the String representation.
     */
    static string toString(uint a, uint radix)
    {            
        string o = "";
        while(a > 0)
        {                
            o += toChar(a % radix);
            a /= radix;
        }            
        return o;        
    }


    /**
     * Translate WordSeq a to string representation with big endian
     * @param  a  WordSeq that will be translated
     * @param  radix  radix of the String representation.
     */
    static string toStringSlow(WordSeq &a, uint radix = 10)
    {
        string o = "";
        uint l, max_radix;
        getMaxRadix(radix, l, max_radix);
        while(compare(a, ZeroWordSeq) > 0)
        {                
            uint r = divideAndRemainderSchool(a, max_radix);             
            string tmp = toString(r, radix);
            o += tmp + string(l - sz(tmp), '0');
        }
        int i = sz(o) - 1;
        while(i > 0 && o[i] == '0') --i;
        o.erase(i + 1);
        reverse(o.begin(), o.end());
        return o;
    }







    class U
    {
    public:











    };

    


    }// namespace BigIntegerPrivate












class BigInteger 
{

};






}



#endif // BIGINTEGER_H

    // namespace BigIntegerPrivate {

    //     typedef unsigned uint;
    //     typedef unsigned long long ull;

    //     template<typename T> 
    //     int sz(const T &o) { return int(o.size()); }

    //     template<typename T>
    //     int topBit(T x) 
    //     {
    //         int r = 0;
    //         for(;x > 0; x >>= 1)
    //         {
    //             r ++;
    //         }
    //         return r;
    //     }

    //     template<typename Vector>
    //     void out(Vector &a)
    //     {            
    //         cout << "[";
    //         for(int i = 0; i < sz(a); ++ i) cout << a[i] << "," ; cout << "]" << endl;
    //     }


    //     template<typename Vector>
    //     void removeLeadingZeros(Vector &v)
    //     {
    //         while(sz(v) >= 2 && *--v.end() == 0) 
    //             //v.pop_back();
    //             v.erase(--v.end());
    //     }


    //     // sequence representation is ascending
    //     // sequence representation  transform to uint on base "radix"
    //     // return the value
    //     template<typename InputIterator>
    //     uint seqToUint(InputIterator first, InputIterator end, const uint &radix)
    //     {
    //         uint x = 0u;            
    //         for(-- end;end >= first; --end)
    //         {
    //             x = x * radix + *end;                
    //         }
    //         return x;
    //     }


    //     // uint transform to sequence representation on base "radix"
    //     // return length of transformed
    //     // the sequence by "v" is ascending
    //     // "L" define the alignment length of the  transformed sequence, and use "0" to align
    //     // if "L" equal to "-1", then not to align
    //     template<typename OutputIterator>
    //     int uintToSeq(OutputIterator it, uint x, const ull &radix, int L = -1) 
    //     {
    //         // cout << "KK" << x << " " << radix << endl;
    //         int r = 0;          
    //         while(x > 0)
    //         {
    //             *it = x % radix;
    //             it ++;

    //             x /= radix;                
    //             r ++;
    //         }
    //         while(r < L) 
    //         {
    //             *it++ = 0;
    //             r ++;
    //         }
    //         return r;            
    //     }


    //     // transform the sequence "s" to the base of "s_radix"         
    //     // "s_radix" means source radix, and "L" satisfy: target_radix = source_radix ^ "L"
    //     // &"t" may be equel to &"s", and "t" can be any form        
    //     template<typename TargetVector, typename SourceVector>
    //     void toBigRadix(TargetVector &t, const SourceVector &s, const ull &s_radix, const int & L) 
    //     {
    //         int n = sz(s) / L + bool(sz(s) % L);
    //         if(sz(t) < n) t.resize(n);
    //         for(int i = 0; i < sz(s); i += L)
    //         {
    //             t[i / L] = seqToUint(s.begin() + i, min(s.begin() + i + L, s.end()), s_radix);
    //         }
    //         if(sz(t) > n) t.resize(n);
    //     }


    //     // transform the sequence "s" to the base of "t_radix" 
    //     // "t_radix" means target radix, and "L" satisfy: source_radix = target_radix ^ "L"
    //     // &"t" may be equal to &"s","t" can be any form        
    //     template<typename TargetSequence, typename SourceSequence>
    //     void toSmallRadix(TargetSequence &t, const SourceSequence &s, const ull &t_radix, const int & L) 
    //     {
    //         int ns = sz(s);
    //         t.resize(ns * L);
    //         for(int i = ns - 1; i >= 0; -- i)
    //         {
    //             uintToSeq(t.begin() + i * L, s[i], t_radix, L);
    //         }
    //         removeLeadingZeros(t);
    //     }


    //     // normalize "v" of base "radix": if some elements in "v" is not less than "radix", 
    //     // then make them less than "radix", and adjust "v"
    //     template<typename Sequence>
    //     void normalize(Sequence &v, const ull & radix)
    //     {
    //         ull x = 0;
    //         for(int i = 0; i < sz(v); ++ i)
    //         {
    //             x = x / radix + v[i];
    //             v[i] = x % radix;
    //         }            
    //         while(x > 0) x /= radix, v.push_back(x % radix);
    //     }



    //     template<typename Sequence>
    //     void fromString(Sequence &v, string &s, int L)
    //     {
    //         reverse(s.begin(), s.end());
    //         for(int i = 0; i < sz(s); ++ i) s[i] -= '0';

    //         toBigRadix(v, s, uint(10u), L);
    //     }
    //     template<typename Sequence>
    //     string toString(Sequence &v, int L)
    //     {
    //         string s;
    //         toSmallRadix(s, v, 10u, L);
    //         for(int i = 0; i < sz(s); ++ i) s[i] += '0';
    //         reverse(s.begin(), s.end());
    //         return s;
    //     }


    //     // Sequence should be vector<uint> currently
    //     // a += b, assert(!a.empty()), assert(!b.empty())
    //     template<typename Sequence>
    //     void plusAssign(Sequence &a, const Sequence &b, const ull &radix) 
    //     {
    //         ull t = 0ull;
    //         int mi = min(sz(a), sz(b));
    //         for(int i = 0; i < mi; ++ i)
    //         {
    //             t += a[i];
    //             t += b[i];                
    //             a[i] = t % radix;
    //             t /= radix;
    //         }
    //         if(sz(a) < sz(b))
    //         {
    //             for(int i = sz(a); i < sz(b); ++ i) 
    //             {
    //                 t += b[i];
    //                 a.push_back(t % radix);
    //                 t /= radix;
    //             }                
    //         }
    //         else 
    //         {
    //             for(int i = sz(b); t > 0ull && i < sz(a); ++ i)
    //             {
    //                 t += a[i];
    //                 a[i] = t % radix;
    //                 t /= radix;
    //             }
    //         }
    //         while(t > 0ull) a.push_back(t % radix), t /= radix;
    //     }


    //     // Sequence should be vector<uint> currently
    //     // a -= b, assert(!a.empty()), assert(!b.empty()), assert(sz(a) >= sz(b))
    //     template<typename Sequence>
    //     void minusAssign(Sequence &a, const Sequence &b, const ull &radix) 
    //     {
    //         ull t = 0;
    //         for(int i = 0; i < sz(b); ++ i)
    //         {                    
    //             t += b[i];
    //             if(a[i] < t) 
    //                 a[i] = radix + a[i] - t, t = 1;
    //             else 
    //                 a[i] -= t, t = 0;
    //         }
    //         for(int i = sz(b); t > 0ull && i < sz(a); ++ i)
    //         {
    //             if(a[i] < t) 
    //                 a[i] = radix + a[i] - t, t = 1;
    //             else 
    //                 a[i] -= t, t = 0;
    //         }
    //         removeLeadingZeros(a);
    //     }

    //     // if r^n = x, return n, else return -1;
    //     template<typename I>
    //     int nthRoot(I r, I x) 
    //     {
    //         int n = 0;
    //         while(r % x == 0) r /= x, n ++;
    //         if(r == 1) return n;
    //         else return -1;
    //     }

    //     // accelerate to multiply using fft 
    //     // a *=b;         
    //     template<typename Sequence>
    //     void fftMultiply(Sequence &a, Sequence &b, const ull &radix)
    //     {
    //         ull t_radix;
    //         int L;
    //         if(radix == int(1e9)) t_radix = 10, L = 9;
    //         else if(radix == int(1e8)) t_radix = 10, L = 8;
    //         else if(radix == int(1 << 28)) t_radix = 1 << 4, L = 7;

    //         toSmallRadix(a, a, t_radix, L);
    //         toSmallRadix(b, b, t_radix, L);
    //         FFT<IntegerFFTData<uint> >::multiply(a, b);
    //         normalize(a, t_radix);
    //         toBigRadix(a, a, t_radix, L);
    //     }


    //     // simulate to multiply, "c"="a"*"b", c can be any form
    //     template<typename Sequence>
    //     void simulationMultiply(Sequence &c, const Sequence &a, const Sequence &b, const ull &radix)
    //     {
    //         c.assign(sz(a) + sz(b), 0);
    //         for(int i = 0; i < sz(b); ++ i)
    //         {
    //             ull t = 0;
    //             for(int j = 0; j < sz(a); ++ j)
    //             {
    //                 t += 1LL * b[i] * a[j] + c[i + j];
    //                 c[i + j] = t % radix;
    //                 t /= radix;
    //             }
    //             if(t > 0)
    //             {
    //                 c[i + sz(a)] = t;
    //             }
    //         }
    //         if(c.back() == 0) c.pop_back();
    //     }


    //     // using fftMultiply and simulationMultiply, c = a * b, &c != &a
    //     template<typename Sequence>
    //     void multiply(Sequence &c, const Sequence &a, const Sequence &b, 
    //                   const ull &s_radix, const ull &t_radix,int L)
    //     {
    //         ull n1 = (ull)sz(a) * sz(b);
    //         ull n2 = 2uLL * max(sz(a), sz(b)) * L;
    //         n2 = n2 * topBit(n2);
    //         if(n1 <= n2)
    //         {
    //             simulationMultiply(c, a, b, s_radix);
    //         }
    //         else
    //         {
    //             c = a;
    //             Sequence tb = b;
    //             fftMultiply(c, tb, t_radix, L);
    //         }
    //     }


    //     // using fftMultiply and simulationMultiply, a *= b
    //     // "s_radix" = "t_radix" ^ "L"
    //     template<typename Sequence>
    //     void multiply(Sequence &a, const Sequence &b, const ull &s_radix, const ull &t_radix, int L)
    //     {
    //         ull n1 = (ull)sz(a) * sz(b);
    //         ull n2 = 2uLL * max(sz(a), sz(b)) * L;
    //         n2 = n2 * topBit(n2);
    //         if(n1 <= n2)
    //         {
    //             Sequence ta = a;
    //             simulationMultiply(a, ta, b, s_radix);
    //         }
    //         else 
    //         {
    //             Sequence tb = b;
    //             fftMultiply(a, tb, t_radix, L);
    //         }
    //     }

    //     // using fftMultiply and simulationMultiply, a *= b
    //     // "s_radix" = "t_radix" ^ "L"
    //     // "b" can be changed
    //     template<typename Sequence>
    //     void multiply(Sequence &a, Sequence &b, const ull &s_radix, const ull &t_radix, int L)
    //     {         
    //         ull n1 = (ull)sz(a) * sz(b);
    //         ull n2 = 2uLL * max(sz(a), sz(b)) * L;
    //         n2 = n2 * topBit(n2);
    //         if(n1 <= n2)
    //         {
    //             Sequence ta = a;
    //             simulationMultiply(a, ta, b, s_radix);
    //         }
    //         else 
    //         {
    //             fftMultiply(a, b, t_radix, L);
    //         }
    //     }


    //     template<typename Sequence>
    //     int cmp(const Sequence &a, const Sequence &b)
    //     {
    //         if(sz(a) < sz(b)) return -1;
    //         if(sz(a) > sz(b)) return 1;
    //         for(int i = sz(a) - 1; i >= 0; -- i)
    //         {
    //             if(a[i] < b[i]) return -1;
    //             if(a[i] > b[i]) return 1;
    //         }
    //         return 0;

    //     }

    //     template<typename Sequence>
    //     uint divide(Sequence &v, const uint &x, const ull &radix)
    //     {
    //         ull t = 0;
    //         for(int i = sz(v) - 1; i >= 0; -- i)
    //         {
    //             t = t * radix + v[i];
    //             v[i] = t / x;
    //             t -= 1ULL * v[i] * x;
    //             // cout << "T:"<< t << endl;
    //         }
    //         removeLeadingZeros(v);
    //         return t;
    //     }




    //     // "Integer" 
    //     template<typename Sequence,typename Integer>
    //     void fromInteger(Sequence &v, Integer i, const ull &radix)
    //     {
    //         v.clear();
    //         while(i > 0)
    //         {
    //             v.push_back(i % radix);
    //             i /= radix;
    //         }
    //     }
        
    //     template<typename Sequence>
    //     tuple<Sequence, Sequence> 
    //     radixTransform(int L, int R, const Sequence &v, const ull &s_radix,
    //         const ull &t_radix, const ull &new_t_radix, const int & new_L)
    //     {            
    //         if(L == R)
    //         {
    //             Sequence a, pa;
    //             fromInteger(a, v[L], t_radix);
    //             fromInteger(pa, s_radix, t_radix);

    //             return make_tuple(a, pa);
    //         }
    //         int mid = (L + R) >> 1;

    //         Sequence a, pa, b, pb;
    //         tie(a, pa) = radixTransform(L, mid, v, s_radix, t_radix, new_t_radix, new_L);
    //         tie(b, pb) = radixTransform(mid + 1, R, v, s_radix, t_radix,new_t_radix, new_L);



    //         multiply(b, (const Sequence&)pa, t_radix, new_t_radix, new_L);
    //         plusAssign(a, b, t_radix);
    //         multiply(pa, pb, t_radix, new_t_radix, new_L);


    //         return make_tuple(a, pa);







    //     }
    // } // namespace BigIntegerPrivate
