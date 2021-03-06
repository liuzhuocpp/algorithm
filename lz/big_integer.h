#ifndef BIGINTEGER_H
#define BIGINTEGER_H


#include <limits>
#include <vector>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <utility>

#include "fft.h"
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
using std::copy;
using std::pair;
using std::make_pair;
using std::fill;

    namespace BigIntegerPrivate {

    /**
     * UintSeq represent the unsigned BigInteger.
     * The length of a word sequence is at least 1(specially for zero), 
     * and no more than INT_MAX.
     * A UintSeq do not contain leading zeros.
     * Use little endian to storage the word sequence.     
     * a[0], a[1], a[2]... radix bits increase
     *        --> 
     */

    /*
     * Some convenient functions and typedefs.  
     */
    template<typename T> static int sz(const T &o) { return o.size(); }

    typedef long long ll;
    
    /**
     * This type is used to hold a word
     */    
    typedef unsigned int uint;

    /**
     * This type is used to hold two words
     */
    typedef unsigned long long ull;

    /**
     * This type is used to hold word sequence
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
     * This constant is used to represent to one
     */
    const UintSeq OneUintSeq(1, 1);

    /**
     * Get maximum radix that a word can hold.
     * And radix ^ l = max_radix
     * @pram radix need be calculated for max radix
     * @pram l  the exponent.
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


    /** Remove leading zeros in UintSeq a */
    static void removeLeadingZeros(UintSeq &a)
    {
        while(sz(a) >= 2 && *--a.end() == 0) 
            //v.pop_back();
            a.erase(--a.end());
    }

    /** Get uint from char */
    static uint fromChar(char x)
    {
        if(x >= '0' && x <= '9') return x - '0';
        if(x >= 'a' && x <= 'z') return x - 'a' + 10;
        if(x >= 'A' && x <= 'Z') return x - 'A' + 10;
    }


    /** Get char from uint */
    static char toChar(uint x)
    {
        if(x >= 0 && x <= 9) return x + '0';
        return x - 10 + 'a';
    }

    /**
     * Plus the contents of the UintSeq a and b. And put the result into a, namely a += b.
     * @param  a an add value.
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
     * Adds the contents of the UintSeq a and uint b. 
     * And put the result into a, namely a += b.
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
     * Multiply the contents of the UintSeq a and b using school method..
     * And put the result into c, namely c = a * b.
     * @param  a the multiply value.
     * @param  b value to be multiplied to a.
     * @param  c the result will be stored.
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
        // if(c.back() == 0) c.pop_back();
        removeLeadingZeros(c);
    }


    /**
     * Multiply the contents of the UintSeq a and uint b using school method.
     * And put the result into a, namely a *= b.
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
     * Minus the UintSeq b from a, and put the result into a, namely a -= b.
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
     * Divide uint b from UintSeq a using school method.
     * And put the result into a, namely a /= b.
     * @param  a the divided value by b.
     * @param  b the divide value.
     * @return the remainder a % b.
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
     * Compare the UintSeq a to b.     
     * @param  a first UintSeq used to compare.
     * @param  b second UintSeq used to compare.
     * @return  if a > b: return 1;
     *          if a == b: return 0;
     *          if a < b: return -1.
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
     * Transform the string into a UintSeq and the string should be big endian
     * @param s the first iterator of string
     * @param end the end iterator of string
     * @param radix the radix of string 
     * @param a the result of transformed uint sequence
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
     * Tranform uint a to string representation with little endian
     * @param  a  uint that will be tranformed.
     * @param  radix  radix of the string representation.
     * @return  the string tranformed.
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
     * Transform UintSeq a to string representation with big endian
     * @param  a  UintSeq that will be tranformed.
     * @param  radix  radix of the String representation.
     */
    static string toStringSlow(const UintSeq &a, uint radix = 10)
    {
        if(compare(a, ZeroUintSeq) == 0) return string("0");
        string o = "";
        uint l, max_radix;
        getMaxRadix(radix, l, max_radix);
        UintSeq ta = a;
        while(compare(ta, ZeroUintSeq) > 0)
        {                
            uint r = divideAndRemainderSchool(ta, max_radix);
            string tmp = toString(r, radix);
            o += tmp + string(l - sz(tmp), '0');
        }
        int i = sz(o) - 1;
        while(i > 0 && o[i] == '0') --i;
        o.erase(i + 1);
        reverse(o.begin(), o.end());
        return o;
    }



    static ll bitLength(const UintSeq & a)
    {
        ll bl = integerBitLength(a.back());
        bl += (sz(a) - 1ll) << 5;
        return bl;
    }


    /**
     * Make the value of a UintSeq shift to high bit.
     * The shift distance, {@code shift_bits}, should be non-negative.
     *
     * @param  shift_bits shift distance, in bits.
     * @return a * (2 ^ shift_bits).
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
                
                a.push_back(highword >> (32 - shift_bits ));
                begin_pos = sz(a) - 2;
            }
            for(int i = begin_pos; i >= 1; -- i)
            {                
                a[i] <<= shift_bits;
                a[i] |= a[i - 1] >> (32 - shift_bits);
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
     * The shift distance, {@code shift_bits}, should be non-negative.
     * @param  shift_bits shift distance, in bits.
     * @return {@code a / (2^shift_bits)}     
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


    string integerToString(ull x, int radix = 10)
    {
        if(x == 0) return "0";
        string o = "";
        while(x > 0)
        {
            o.push_back(toChar(x % radix));
            x /= radix;
        }
        reverse(o.begin(), o.end());
        return o;
    }

    /**
     * Divide the contents of the UintSeq a and UintSeq b using an O(n^2) algorithm from Knuth.
     * Uses Algorithm D in Knuth section 4.3.1.
     * Quotient will be in UintSeq a.
     * @param  a the divided value by b.
     * @param  b the divide value.
     * @return the remainder a % b
     */            

    static void divideAndRemainderKnuth(UintSeq &a, UintSeq &b, 
                                        UintSeq &quotient, UintSeq &remainder)
    {
        if(compare(b, ZeroUintSeq) == 0)
        {
            cout << "divide 0" << endl;
            return ;
        }
        if(sz(a) < sz(b))
        {
            quotient = ZeroUintSeq;
            remainder = b;
            return ;
        }
        if(sz(b) == 1)
        {
            quotient = a;
            uint r = divideAndRemainderSchool(quotient, b[0]);
            remainder.assign(1, r);
            return ;
        }
        
        
        ll b_bitlen = bitLength(b);
        int need_shift_bits = 32 - (b_bitlen & 31);
        need_shift_bits &= 31;

        if(need_shift_bits > 0)
        {
            shiftHigh(a, need_shift_bits);
            shiftHigh(b, need_shift_bits);
        }

        quotient.clear();
        remainder.clear();

        for(int i = sz(a) - 1; i >= sz(a) - sz(b) + 1; -- i)
        {
            remainder.push_back(a[i]);
        }

        reverse(remainder.begin(), remainder.end());

        for(int i = sz(a) - sz(b); i >= 0; -- i)
        {
            
            shiftHigh(remainder, 32);
            remainder[0] = a[i];
            if(compare(remainder, b) < 0)
            {
                quotient.push_back(0);
                continue;
            }
            
            ull q_;
            if(sz(remainder) == sz(b))
            {
                q_ = remainder.back() / b.back();
            }
            else
            {
                q_ = *remainder.rbegin();
                q_ = q_ << 32 | *++remainder.rbegin();
                q_ /= b.back();
                if(q_ > UllMask) q_ = UllMask;
            }
            UintSeq tmp = b;
            multiplySchool(tmp, q_);

            // this loop at most twice
            while(compare(tmp, remainder) > 0)
            {
                q_ --;
                minus(tmp, b);
            }
            minus(remainder, tmp);
            quotient.push_back(q_);
        }
        reverse(quotient.begin(), quotient.end());
        if(need_shift_bits > 0)
        {
            shiftLow(a, need_shift_bits);
            shiftLow(b, need_shift_bits);
            shiftLow(remainder, need_shift_bits);
        }     
    }



    static bool testBit(const UintSeq &a, ll n)
    {
        return a[n >> 5] >> (n & 31) & 1;
    }
    static void setBit(UintSeq &a, ll n, bool val = 1)
    {
        if(testBit(a, n) == val) return;
        a[n >> 5] ^= 1 << (n & 31);
    }


    /**
     * Translate the binary representation of a UintSeq to a UintSeq that 
     * every uint stores bits that the length is {@code l}.
     * @param a  a UintSeq will be translated.
     * @param l  the length that every uint will be stored.
     * @param b  the result UintSeq.
     */
    static void toL(const UintSeq &a, int l, UintSeq &b)
    {

        ll n = bitLength(a);
        b.assign(n / l + bool (n % l), 0);
        
        int j = 0;
        for(ll i = 0; i < n; i += l)
        {
            uint x = 0;
            for(ll j = min(n - 1, i + l - 1); j >= i; -- j)
            {
                x <<= 1;
                x += testBit(a, j);
            }            
            b[j ++] = x;
        }
    }


    /**
     * Multiply the contents of the UintSeq a and b using FFT method.
     * And put the result into c, namely c = a * b.
     * @param  a the multiply value.
     * @param  b value to be multiplied to a.
     * @param  c the result will be stored.
     */
    static void multiplyFFT(const UintSeq &a, const UintSeq &b, int l, UintSeq &c)
    {
        UintSeq ta;
        // make the radix of {@code a, b} 2^l
        toL(a, l, ta);
        toL(b, l, c);

        
        lz::fftMultiply<IntegerFFTData<uint> >(ta, c);

        // make the radix of {@code c} 2^l
        c.clear();
        int end = sz(ta) - 1;
        while(end >= 0 && ta[end] == 0) end --;
        int lbits_mask = (1 << l) - 1;
        ull x = 0;
        for(int i = 0; i <= end; ++ i)
        {   
            x += ta[i];
            c.push_back(x & lbits_mask);
            x >>= l;
        }
        while(x > 0)
        {
            c.push_back(x & lbits_mask);
            x >>= l;
        }
        // cout << "sf" << endl;
        // make the radix of {@code c} 2^32
        int num_per_uint = 32 / l, k = 0;
        for(int i = 0; i < sz(c); i += num_per_uint)
        {
            uint x = 0;
            for(int j = min(sz(c) - 1, i + num_per_uint - 1); j >= i; -- j)
            {
                x = (x << l) + c[j];
            }
            c[k ++] = x;
        }
        
        c.resize(k);
        return ;

        // ta.clear();
        // for(int i = 0; i < sz(c); i += num_per_uint)
        // {
        //     uint x = 0;
        //     for(int j = min(sz(c) - 1, i + num_per_uint - 1); j >= i; -- j)
        //     {
        //         x = (x << l) + c[j];
        //     }
        //     ta.push_back(x);
        // }
        // c = std::move(ta);

    }


    static void getLower(const UintSeq &a, int n, UintSeq &b)
    {
        if(sz(a) <= n)
        {
            b = a;
            return ;
        }        
        for(int i = 0; i < n; ++ i) b.push_back(a[i]);
    }
    static void getUpper(const UintSeq &a, int n, UintSeq &b)
    {
        if(sz(a) <= n) 
        {
            b = ZeroUintSeq;
            return ;
        }        
        for(int i = n; i < sz(a); ++ i)
        {
            b.push_back(a[i]);
        }

    }

    int tot = 0;
    int tot2 = 0;
    static void multiplyKaratsuba(const UintSeq &a, const UintSeq &b, UintSeq &c)
    {
        // cout << "A.size:" << a.size() << endl;
        // tot ++;
        // cout << "TOT: " << tot << endl;
        if(max(sz(a), sz(b)) <= 80)
        {
            // multiplyFFT(a, b, 4, c);
            multiplySchool(a, b, c);
            return ;
        }

        // tot2 ++;
        // cout << "TOT2: " << tot2 << endl;
        // cout << "JJ" << endl;
        int half = (max(sz(a), sz(b)) + 1) >> 1;
        UintSeq a0, a1, b0, b1;

        getLower(a, half, a0);
        getUpper(a, half, a1);

        getLower(b, half, b0);
        getUpper(b, half, b1);

        UintSeq p0, p1;
        multiplyKaratsuba(a0, b0, p0);
        multiplyKaratsuba(a1, b1, p1);
        plus(a0, a1);
        plus(b0, b1);
        multiplyKaratsuba(a0, b0, c);
        minus(c, p0);
        minus(c, p1);

        shiftHigh(p1, 64LL * half);
        shiftHigh(c, 32LL * half);
        plus(c, p1);
        plus(c, p0);





    }




    static void multiply(const UintSeq &a, const UintSeq &b, UintSeq &c)
    {
        ll school_method_complexity = ((ll)sz(a)) * sz(b);
        ll max_size = max(sz(a), sz(b));
        ll fft_method_complexity;
        if(max_size <= 1e6)
        {
            max_size *= 8;
            fft_method_complexity = max_size * integerBitLength(max_size);
            if(school_method_complexity < fft_method_complexity)
                multiplySchool(a, b, c);
            else 
                multiplyFFT(a, b, 4, c);
        }
        else 
        {
            max_size *= 32;
            fft_method_complexity = max_size * integerBitLength(max_size);
            if(school_method_complexity < fft_method_complexity)
                multiplySchool(a, b, c);
            else 
                multiplyFFT(a, b, 1, c);
        }
    }

    static void divideAndRemainder3n2n(const UintSeq &a, const UintSeq &b,
                                       UintSeq &quotient, UintSeq &remainder, int deep);

    int num = 0;
    static void divideAndRemainder2n1n(const UintSeq &a, const UintSeq &b, 
                                       UintSeq &quotient, UintSeq &remainder, int deep)
    {
        num ++;
        
        // cout << "DEEP" << num << " " << deep << " " << sz(a) << " " << sz(b) << endl;
        if(sz(a) != sz(b) * 2) 
        {
            cout <<"EROOR" << endl;
            return;
        }
        if(sz(a) == 2)
        {            
            quotient = a;
            remainder.resize(1);
            remainder[0] = divideAndRemainderSchool(quotient, b[0]);
            return ;
        }

        int n = sz(b);
        int half_n = n >> 1;

        UintSeq ta(a.begin() + half_n, a.end());
        UintSeq tq;
        
        divideAndRemainder3n2n(ta, b, tq, remainder, deep + 1);

        // cout <<"IN:" << toStringSlow(ta, 16) << endl;
        // cout <<"IN:" << toStringSlow(b, 16) << endl;
        // cout <<"IN:" << toStringSlow(tq, 16) << endl;
        // cout <<"IN:" << toStringSlow(remainder, 16) << endl;
        // UintSeq tt;
        // multiply(tq, b, tt);
        // plus(tt, remainder);
        // cout << "CMP" << toStringSlow(tt, 16) << endl;

        // int ST= 10;
        // while(ST--)
        // cout <<"STOP" << endl;
        fill(ta.begin(), ta.end(), 0);
        copy(a.begin(), a.begin() + half_n, ta.begin());
        copy(remainder.begin(), remainder.end(), ta.begin() + half_n);

        remainder.clear();


        
        
        divideAndRemainder3n2n(ta, b, quotient, remainder, deep + 1);
        // cout <<"@@:" << toStringSlow(ta, 16) << endl;
        // cout <<"@@:" << toStringSlow(b, 16) << endl;
        // cout <<"@@:" << toStringSlow(quotient, 16) << endl;
        // cout <<"@@:" << toStringSlow(remainder, 16) << endl;
        for(int i = 0; i < sz(tq); ++ i) 
            quotient.push_back(tq[i]);




    }
    static void divideAndRemainder3n2n(const UintSeq &a, const UintSeq &b, 
                                       UintSeq &quotient, UintSeq &remainder, int deep)
    {
        if(sz(a) * 2 != sz(b) * 3) 
        {
            cout <<"EROOR" << endl;
            return ;
        }

        int n = sz(b) >> 1;
        int n2 = sz(b);

        UintSeq ta(a.begin() + n2, a.end()), tb(b.begin() + n, b.end());

        if(compare(ta, tb) < 0)
        {
            // cout << "enterJJJ" << endl;            
            ta.clear();
            for(int i = n; i < n + n2; ++ i) ta.push_back(a[i]);
            // cout << "3n:" << toStringSlow(ta, 16) << endl;
            // cout << "3n:" << toStringSlow(tb, 16) << endl;
            divideAndRemainder2n1n(ta, tb, quotient, remainder, deep + 1);
            // cout << "3n:" << toStringSlow(quotient, 16) << endl;
            // cout << "3n:" << toStringSlow(remainder, 16) << endl;
        }
        else 
        {            
            for(int i = 0; i < n; ++ i) quotient.push_back(~0u);
        }
        

        // cout << "I am in 3n / 2n " << endl;
        ta.clear();
        // multiplySchool(quotient, b, ta);
        // multiplyFFT(quotient, b, 4,  ta);
        multiplyKaratsuba(quotient, b, ta);

        // cout << "34*****" << endl;
        // cout  << toStringSlow(quotient) << endl;
        // cout  << toStringSlow(b) << endl;
        // cout  << toStringSlow(ta) << endl;

        // UintSeq tta;
        // multiplySchool(quotient, b, tta);
        // cout << "TTA:" << toStringSlow(tta) << endl;



        // multiply(quotient, b, ta);
        while(compare(a, ta) < 0)
        {
            // cout << " OIUEOIU" << endl;
            minus(quotient, OneUintSeq);
            minus(ta, b);
        }
        // cout << " SLDKJFKLSJD" << endl;
        remainder = a;
        minus(remainder, ta);


        
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
