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
using std::min;
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
            a.clear();
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


        


        string& toString(string &s, const U &a) // s should be empty
        {
            s.clear();
            s.reserve(sz(a) * L);

            char buf[10];
            sprintf(buf, "%d", a.back());
            s += buf;
            for(int i = sz(a) - 2; i >= 0; -- i)
            {
                sprintf(buf, "%09d", a[i]);
                s += buf;
            }
            return s;
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

        template<typename T>
        void toVector10(vector<T> &target, const U &source) // s should be empty, Ascending
        {
            target.clear();
            target.reserve(sz(source) * L);
            char buf[L + 1];
            for(int i = 0; i <= sz(source) - 2; ++ i)
            {
                sprintf(buf, "%09d", source[i]);
                for(int j = L - 1; j >= 0; -- j) target.push_back(buf[j] - '0');
            }
            sprintf(buf, "%d", source.back());
            int lb = strlen(buf);
            for(int j = lb - 1; j >= 0; -- j) target.push_back(buf[j] - '0');
        }

        template<typename T>
        void fromVector10(U &target, const vector<T> &source) // a should be empty, Ascending
        {   
            target.clear();         
            dint x = 0;

            for(int i = 0; i < sz(source); ++ i)
            {
                x = x / 10 + int(source[i]);
                target.push_back(x % 10);
            }
            while(x > 0) x /= 10, target.push_back(x % 10);
            while(sz(target) > 1 && target.back() == 0) target.pop_back();

            // cout << "target " << endl;
            // for(int i = 0; i < sz(target); ++ i) cout << target[i]; cout << endl;

            for(int i = 0; i < sz(target); i += L)
            {
                int x = 0;
                for(int j = min(i + L - 1, sz(target) - 1); j >= i; -- j)
                {
                    x = x * 10 + target[j];
                }
                target[i / L] = x;
            }
            int n = sz(target) / L + bool(sz(target) % L);
            while(sz(target) > n) target.pop_back();

        }
        void simulationMultiply(U &c, const U &a, const U &b)
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
        void fftMultiply(U &c, const U &a, const U &b)
        {
            // cout << "in fftMultiply" << endl;
            // string o;
            // cout << toString(o, a) << endl;
            // cout << toString(o, b) << endl;



            typedef IntegerFFTData::T T;
            vector<T> ra, rb;
            toVector10<T>(ra, a);
            toVector10<T>(rb, b);


            // for(int i = 0; i < sz(ra); ++ i) cout << ra[i]; cout << endl;

            // for(int i = 0; i < sz(rb); ++ i) cout << rb[i]; cout << endl;


            FFT<IntegerFFTData>::multiply(ra, rb);
            // for(int i = 0; i < sz(ra); ++ i) cout << ra[i]; cout << endl;


            fromVector10<T>(c, ra);

            // cout << "end fftMultiply" << endl;

        }
        void multiply(U &c, const U & a, const U &b) // c should be empty, c = a + b;
        {
            dint n1 = 1LL * sz(a) * sz(b);
            dint n2 = 2LL * L * max(sz(a), sz(b));
            n2 = n2 * topBit(n2);            

            if(n1 <= n2)
            {
                simulationMultiply(c, a, b);

            }
            else 
            {
                fftMultiply(c, a, b);
                
            }
        }





    }

    class BigInteger 
    {

    };






}



#endif // BIGINTEGER_H
