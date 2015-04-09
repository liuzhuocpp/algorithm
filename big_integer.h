#ifndef BIGINTEGER_H
#define BIGINTEGER_H


#include <limits>
#include <vector>
#include <iostream>
#include <cstring>

#include "fft.h"

namespace lz {

using std::vector;
using std::numeric_limits;
using std::cout;
using std::endl;
using std::string;
using std::strlen;
using std::max;
    namespace BigIntegerPrivate {

        // typedef unsigned uint;
        // typedef unsigned long long duint;
        typedef long long dint;
        typedef vector<int> U;
        const dint radix = 1000000000;
        const int L = 9;
        template<typename T> 
        int sz(const T &o) { return int(o.size()); }


        template<typename T>
        int topBit(T x) 
        {
            int r = 0;
            for(;x > 0; x >>= 1)
            {
                r ++;
            }
            return r;
        }

        void fromString(U &a, const char *s, int n) // a should be empty
        {            
            for(int i = n - 1; i >= 0; i -= L) 
            {
                int x = 0;     
                for(int j = max(0, i - L + 1); j <= i; ++ j)                
                    x = x * 10 + int(s[j] - '0');                
                a.push_back(x);
            }
        }
        void fromString(U &a, const string &s) // a should be empty
        {
            fromString(a, s.c_str(), s.size());
        }

        void toString(string &s, const U &a) // s should be empty
        {
            // cout << a.size() << endl;
            // cout << a[0] << endl;
            s.reserve(sz(a) * 9);

            char buf[10];
            sprintf(buf, "%d", a.back());
            s += buf;
            for(int i = sz(a) - 2; i >= 0; -- i)
            {
                sprintf(buf, "%09d", a[i]);
                s += buf;
            }
            // cout << "FF " << s << endl;
        }

        void addAssign(U &a, const U &b) //a += b
        {                
            int t = 0;
            for(int i = 0; i < sz(a); ++ i)
            {
                t += a[i];
                if(i < sz(b)) t += b[i];
                
                if(t >= radix) a[i] = t - radix, t = 1;
                else a[i] = t, t = 0;
            }
            for(int i = sz(a); i < sz(b); ++ i)
            {
                t += b[i];
                if(t >= radix) a.push_back(t - radix), t = 1;
                else a.push_back(t), t = 0;
            }
            if(t != 0) a.push_back(t);
        }
        void substractAssign(U &a, const U &b) // a -= b
        {
            int t = 0;
            for(int i = 0; i < sz(a); ++ i)
            {                    
                if(i < sz(b)) t += b[i];
                if(a[i] < t) 
                    a[i] = a[i] + radix - t, t = 1;
                else 
                    a[i] -= t, t = 0;
            }
            int i;
            for(i = sz(a) - 1; i > 0 && a[i] == 0; -- i);
            a.resize(i + 1);
            
        }
        void multiply(U &c, const U & a, const U &b) // c should be empty, c = a + b;
        {
            dint n1 = 1LL * sz(a) * sz(b);
            dint n2 = 2LL * 9LL * max(sz(a), sz(b));
            n2 = n2 * topBit(n2);
            // if(n1 <= n2)
            {
                c.resize(a.size() + b.size());
                for(int i = 0; i < sz(b); ++ i)
                {
                    dint t = 0;
                    for(int j = 0; j < sz(a); ++ j)
                    {
                        t += 1LL * b[i] * a[j] + c[i + j];
                        c[i + j] = t % radix;
                        t /= radix;
                    }
                    if(t > 0)
                    {
                        c[i + sz(a)] = t;
                    }
                }
                if(c.back() == 0) c.pop_back();
                
            }
            // else 
            // {

            // }
        }



        // class U: private super
        // {
        //     template<typename T> 
        //     int sz(const T &o) { return int(o.size()); }

            

        //     const static duint radix = 1000000000;
        //     const static int L = 9; // radix length
            

        //     explicit U(super &a):super(a) {}
        // public:
        //     U():super(1, 0) {}
        //     explicit U(const char *s) /// decimal string
        //     {                
        //         for(int i = strlen(s) - 1; i >= 0; i -= L)
        //         {
        //             uint x = 0;
        //             for(int j = max(0, i - L + 1); j <= i; ++ j)
        //             {
        //                 x = x * 10 + uint(s[j] - '0');
        //             }
        //             push_back(x);
        //         }
        //     }
        //     // explicit U(const string &s)
        //     // {

        //     // }
        //     // explicit U(const uint &a): super(1, a) { }
        //     string toString() const
        //     {
        //         char buf[10];
        //         sprintf(buf, "%u", this->back());
        //         string r(buf);

        //         for(int i = int(size()) - 2; i >= 0; -- i)
        //         {
        //             sprintf(buf, "%09u", operator[](i));
        //             r += buf;
        //         }
        //         return r;
        //     }

        //     U& addAssign(const U &o)
        //     {                
        //         uint t = 0;
        //         for(int i = 0; i < sz(*this); ++ i)
        //         {
        //             t += operator[](i);
        //             if(i < sz(o)) t += o[i];
                    
        //             if(t >= radix) operator[](i) = t - radix, t = 1;
        //             else operator[](i) = t, t = 0;
        //         }
        //         for(int i = sz(*this); i < sz(o); ++ i)
        //         {
        //             t += o[i];
        //             if(t >= radix) push_back(t - radix), t = 1;
        //             else push_back(t), t = 0;
        //         }
        //         if(t != 0) push_back(t);
        //         return *this;
        //     }
        //     U& substractAssign(const U &o)
        //     {
        //         uint t = 0;
        //         for(int i = 0; i < sz(*this); ++ i)
        //         {                    
        //             if(i < sz(o)) t += o[i];
        //             if(operator[](i) < t) 
        //                 operator[](i) = operator[](i) + radix - t, t = 1;
        //             else 
        //                 operator[](i) -= t, t = 0;
        //         }
        //         int i;
        //         for(i = sz(*this) - 1; i > 0 && operator[](i) == 0; -- i);
        //         resize(i + 1);
        //         return *this;
        //     }
        //     U& multiplyAssign(const U &o)
        //     {
        //         duint n1 = 1ULL * size() * o.size();
        //         duint n2 = 2ULL * max(size(), o.size());
        //         n2 = n2 * topBit(n2);
        //         if(n1 <= n2)
        //         {
        //             int n = size();
        //             resize(size() + o.size());
        //             for(int i = 0; i < sz(o); ++ i)
        //             {
        //                 for(int j = 0; j < n; ++ j)
        //                 {
        //                     duint t = 1ULL * o[i] * operator[](j);
                            
        //                 }
        //             }
        //         }
        //         else 
        //         {

        //         }
        //     }




        
            

        // };


    }

    class BigInteger 
    {

    };






}



#endif // BIGINTEGER_H
