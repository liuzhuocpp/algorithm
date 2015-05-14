#ifndef BIGINTEGER_H
#define BIGINTEGER_H


#include <limits>
#include <vector>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <utility>

// #include "fft.h"
#include "utility.h"

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
     * The word sequence do not contain leading zeros.
     * a[0], a[1], a[2]... radix bits increase
     *        --> 
     */

    /*
     * some convenient functions     
     */
    template<typename T> static int sz(const T &o) { return o.size(); }

    typedef long long ll;
    
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
    typedef vector<uint> UintSeq;


    /**
     * This constant is the radix of every word
     */
    static const ull WordRadix = 0xffffffffull + 1ull;

    /**
     * This constant is used to get lower word
     */
    static const ull UllMask = ~0u;

    /**
     * This constant is used to represent to zero
     */
    const UintSeq ZeroUintSeq(1, 0);

    /**
     * This constant is used to represent to zero
     */
    const UintSeq OneUintSeq(1, 1);

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
        while(tmax_radix * radix <= UllMask) 
            tmax_radix *= radix, l ++;
        max_radix = tmax_radix;
    }


    /**
     * Output the contents of UintSeq o
     * @param o need be output
     */
    void out(const UintSeq & o)
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
    static void removeLeadingZeros(UintSeq &a)
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
     * Adds the contents of the UintSeq a and b. And put the result into a : a += b.
     * @param  a add value.
     * @param  b value to be added to a.     
     */
    static void plus(UintSeq &a, const UintSeq &b)
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
     * Adds the contents of the UintSeq a and Word b. 
     * And put the result into a : a += b.
     * @param  a add value.
     * @param  b value to be added to a.     
     */
    static void plus(UintSeq &a, uint b)
    {
        ull t = b;
        for(int i = 0; i < sz(a); ++ i)
        {
            if(t == 0) break;
            t += a[i];
            a[i] = t & UllMask;
            t >>= 32;
        }
        if(t > 0) a.push_back(t & UllMask);
    }


    /**
     * Multiply the contents of the UintSeq a and b using school method: c = a * b.
     * And put the result into c.
     * @param  a the multiply value.
     * @param  b value to be multiplied to a.
     */        
    static void multiplySchool(const UintSeq &a, const UintSeq &b, UintSeq &c)
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
     * Multiply the contents of the UintSeq a and Word b using school method:a *= b.         
     * @param  a the multiply value.
     * @param  b value to be multiplied to a.
     */        
    static void multiplySchool(UintSeq &a, uint b)
    {
        ull t = 0ull;
        for(int i = 0; i < sz(a); ++ i)
        {
            t += (a[i] & UllMask) * b;
            a[i] = t & UllMask;
            t >>= 32;
        }
        if(t > 0ull) a.push_back(t & UllMask);
    }




    /**
     * minus the contents of the UintSeq b from a: a -= b.
     * And put the result into a.
     * @param  a value to be minuend by b.
     * @param  b the minus value.
     */
    static void minus(UintSeq &a, const UintSeq &b) 
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
     * Divide the contents of the UintSeq a and Word b using school method:a /= b.
     * @param  a the divided value by b.
     * @param  b the divide value.
     * @return the remainder a % b
     */        
    static uint divideAndRemainderSchool(UintSeq &a, uint b)
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
        return t;
    }


    /**
     * compare the contents of the UintSeq a and b.     
     * @param  a first UintSeq used to compare
     * @param  b second UintSeq used to compare
     * @return  if a > b: return 1;
     *          if a == b: return 0;
     *          if a < b: return -1; 
     */
    static int compare(const UintSeq &a, const UintSeq &b)
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
    static void fromString(Iterator s, Iterator end, uint radix, UintSeq &a)
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
        if(a == 0) return string("0");
        string o = "";
        while(a > 0)
        {                
            o += toChar(a % radix);
            a /= radix;
        }            

        return o;        
    }


    /**
     * Translate UintSeq a to string representation with big endian
     * @param  a  UintSeq that will be translated
     * @param  radix  radix of the String representation.
     */
    static string toStringSlow(UintSeq &a, uint radix = 10)
    {
        if(compare(a, ZeroUintSeq) == 0) return string("0");
        string o = "";
        uint l, max_radix;
        getMaxRadix(radix, l, max_radix);
        while(compare(a, ZeroUintSeq) > 0)
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



    static ll bitLength(UintSeq & a)
    {
        ll bl = integerBitLength(a.back());
        bl += (sz(a) - 1ll) << 5;
        return bl;
    }


    /**
     * Make the value of a UintSeq shift to high bit.
     * The shift distance, {@code n}, should be non-negative.
     *
     * @param  n shift distance, in bits.
     * @return {@code a * (2^n)}
     */
    static void shiftHigh(UintSeq &a, ll shift_bits)
    {
        if(compare(a, ZeroUintSeq) == 0) return ;

        int shift_words = shift_bits >> 5;
        shift_bits &= 31;

        // shift bits
        if(shift_bits > 0)
        {
            uint highword = a.back();
            int highword_bitlen = integerBitLength(highword);

            int begin_pos = sz(a) - 1;
            if(32 - highword_bitlen < shift_bits)
            {
                a.push_back(highword >> (32 - highword_bitlen));
                begin_pos = sz(a) - 2;
            }
            for(int i = begin_pos; i >= 1; -- i)
            {                
                a[i] <<= shift_bits;
                a[i] |= a[i - 1] & (((1 << shift_bits) - 1) << (32 - shift_bits) );                
            }
            a[0] <<= shift_bits;
        }

        // shift words
        if(shift_words > 0)
        {        
            a.resize(sz(a) + shift_words);
            for(int i = sz(a) - 1; i >= shift_words; -- i)
                a[i] = a[i - shift_words];
            for(int i = shift_words - 1; i >= 0; -- i)
                a[i] = 0;
        }
    }

    /**
     * Make the value of a UintSeq shift to low bit.
     * The shift distance, {@code n}, should be non-negative.
     * @param  n shift distance, in bits.
     * @return {@code a / (2^n)}     
     */
     static void shiftLow(UintSeq &a, ll shift_bits)
     {
        if(compare(a, ZeroUintSeq) == 0) return ;

        int shift_words = shift_bits >> 5;
        shift_bits &= 31;

        if(shift_bits > 0)
        {            
            for(int i = 0; i < sz(a) - 1; ++ i)
            {
                a[i] = a[i] >> shift_bits | 
                      (a[i + 1] & ((1u << shift_bits) - 1)) << (32 - shift_bits);
            }
            if(shift_bits >= integerBitLength(a.back()))
            {
                if(sz(a) > 1) a.pop_back();
                else a[0] = 0;
            }
            else a.back() >>= shift_bits;
        }


        if(shift_words > 0)
        {
            if(shift_words >= sz(a)) 
            {
                a = ZeroUintSeq; 
                return ;
            }

            for(int i = 0; i < sz(a) - shift_words; ++ i)
            {
                a[i] = a[i + shift_words];
            }
            a.resize(sz(a) - shift_words);
        }
     }


    /**
     * Divide the contents of the UintSeq a and UintSeq b using an O(n^2) algorithm from Knuth.
     * Uses Algorithm D in Knuth section 4.3.1.
     * Quotient will be in UintSeq a.
     * @param  a the divided value by b.
     * @param  b the divide value.
     * @return the remainder a % b
     */            

    static UintSeq divideAndRemainderKnuth(UintSeq &a, UintSeq &b, bool needRemainder)
    {
        if(compare(b, ZeroUintSeq) == 0) 
        {
            cout << "divide 0" << endl;
            return ZeroUintSeq;
        }
        if(sz(a) < sz(b))
        {
            a = ZeroUintSeq;
            if(needRemainder) return b;            
            else return ZeroUintSeq;                
        }
        
        ll b_bitlen = bitLength(b);
        int b_bitlen_and_5 = b_bitlen & 5;
        if(b_bitlen_and_5 > 0)
        {
            shiftHigh(a, b_bitlen_and_5);
            shiftHigh(b, b_bitlen_and_5);
        }

        // UintSeq quotient(a.rbegin(), a.rbegin() + sz(b));
        UintSeq quotient;        
        for(int i = sz(a) - sz(b); i >= 0; -- i)
        {
            ull ta = a[i] & UllMask;
            ull tb = b[i] & UllMask;
            ull q_ = ta / tb;
            q_ --

        }




        // return t;
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
