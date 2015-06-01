#include <iostream>
#include <cstdio>
#include <limits>
#include <vector>
// #include <complex>
// #include <cmath>
#include <algorithm>
#include <cstring>
#include <ctime>
#include <bitset>
#ifndef BIGINTEGER_H
#define BIGINTEGER_H


#include <limits>
#include <vector>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <utility>

#ifndef UTILITY_H
#define UTILITY_H

#include <functional>
// some little and mess functions

namespace lz {



template<typename T>
int integerBitLength(T x)
{
    if(x == 0) return 1;
    int r = 0;
    for(;x > 0; x >>= 1)
    {
        r ++;
    }
    return r;
}


template<typename T>
T modPow(T a, T b, const T &c)
{
    T r = 1;
    while(b > 0)
    {
        if(b & 1) r = r * b % c;
        a = a * a % c;
        b >>= 1;
    }
    return r;
}





} // namespace lz

#endif // UTILITY_H


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

    template<typename T> static int sz(const T &o) { return o.size(); }

    typedef long long ll;

    typedef unsigned int uint;

    typedef unsigned long long ull;

    typedef vector<uint> UintSeq;


//    static const ull WordRadix = 0xffffffffull + 1ull;
    static const ull WordRadix = 10;


    const UintSeq ZeroUintSeq(1, 0);

    const UintSeq OneUintSeq(1, 1);



    void out(const UintSeq & o)
    {
        cout << "[";
        for(int i = 0; i < sz(o); ++ i)
        {
            cout << o[i]<< ",";
        }

        cout << "]"<< endl;
    }


    static void removeLeadingZeros(UintSeq &a)
    {
        while(sz(a) >= 2 && *--a.end() == 0)
            //v.pop_back();
            a.erase(--a.end());
    }

    static uint fromChar(char x)
    {
        if(x >= '0' && x <= '9') return x - '0';
        if(x >= 'a' && x <= 'z') return x - 'a' + 10;
        if(x >= 'A' && x <= 'Z') return x - 'A' + 10;
    }



    static char toChar(uint x)
    {
        if(x >= 0 && x <= 9) return x + '0';
        return x - 10 + 'a';
    }


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










    static void shiftHigh(UintSeq &a, ll shift_words)
    {
        if(compare(a, ZeroUintSeq) == 0) return ;
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
        tot ++;
//        cout << "TOT: " << tot << endl;
        if(max(sz(a), sz(b)) <= 100)
        {
            multiplySchool(a, b, c);
            return ;
        }
        tot2 ++;
//        cout << "TOT2: " << tot2 << endl;
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

        shiftHigh(p1, 2 * half);
        shiftHigh(c, half);
        plus(c, p1);
        plus(c, p0);





    }









    }// namespace BigIntegerPrivate













}



#endif // BIGINTEGER_H



using namespace std;
using namespace lz;
using namespace lz::BigIntegerPrivate;



const int N = 2e5 + 9;

UintSeq a, b, q;
char s1[N], s2[N];
int main()
{
    int second, first;
    int NN = 1e5;
//    while(~scanf("%s%s", s1, s2))
    for(int i = 0; i < NN; ++ i) s1[i] = s2[i] = '1';
    s1[NN] = s2[NN] = 0;


    {
        a.clear();
        b.clear();
        q.clear();
        int l1 = strlen(s1);
        int l2 = strlen(s2);
        for(int i = l1 - 1; i >= 0; -- i) a.push_back(s1[i] - '0');
        for(int i = l2 - 1; i >= 0; -- i) b.push_back(s2[i] - '0');
        first = clock();
        multiplyKaratsuba(a, b, q);
        second = clock();
        double dif = (second - first) / 1000.0;

        printf( "multiply time: %.3f\n", dif);


//        int j = 0;
//        for(int i = sz(q) - 1; i >= 0; -- i)
//        {
//            s1[j ++] = q[i] + '0';
//        }
//        s1[j] = 0;
//        puts(s1);

    }



    return 0;
}


