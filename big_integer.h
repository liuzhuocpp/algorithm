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

        typedef unsigned uint;
        typedef unsigned long long ull;

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

        template<typename Vector>
        void out(Vector &a)
        {            
            cout << "[";
            for(int i = 0; i < sz(a); ++ i) cout << a[i] << "," ; cout << "]" << endl;
        }


        template<typename Vector>
        void removeLeadingZeros(Vector &v)
        {
            while(sz(v) >= 2 && *--v.end() == 0) 
                //v.pop_back();
                v.erase(--v.end());
        }


        // sequence representation is ascending
        // sequence representation  transform to uint on base "radix"
        // return the value
        template<typename InputIterator>
        uint seqToUint(InputIterator first, InputIterator end, const uint &radix)
        {
            uint x = 0u;            
            for(-- end;end >= first; --end)
            {
                x = x * radix + *end;                
            }
            return x;
        }


        // uint transform to sequence representation on base "radix"
        // return length of transformed
        // the sequence by "v" is ascending
        // "L" define the alignment length of the  transformed sequence, and use "0" to align
        // if "L" equal to "-1", then not to align
        template<typename OutputIterator>
        int uintToSeq(OutputIterator it, uint x, const ull &radix, int L = -1) 
        {
            // cout << "KK" << x << " " << radix << endl;
            int r = 0;          
            while(x > 0)
            {
                *it = x % radix;
                it ++;

                x /= radix;                
                r ++;
            }
            while(r < L) 
            {
                *it++ = 0;
                r ++;
            }
            return r;            
        }


        // transform the sequence "t" to the base of "s_radix"         
        // "s_radix" means source radix
        // "L" : target_radix = source_radix ^ "L"
        // &"t" may be equel to &"s"
        // "t" size must have not less than "n"
        template<typename TargetVector, typename SourceVector>
        void toBigRadix(TargetVector &t, const SourceVector &s, const ull &s_radix, const int & L) 
        {
            // cout << "JJ:" << sz(s) << endl;
            // cout <<"string:";
            // for(int i = 0; i < sz(s); ++ i) cout << int(s[i]) << "|" << endl; cout << "VAO" << endl;
            for(int i = 0; i < sz(s); i += L)
            {
                t[i / L] = seqToUint(s.begin() + i, min(s.begin() + i + L, s.end()), s_radix);
            }
            int n = sz(s) / L + bool(sz(s) % L);
            t.resize(n);
        }


        // transform the sequence "t" to the base of "t_radix" 
        // "t_radix" means target radix
        // "L" : source_radix = target_radix ^ "L"
        // &"t" may be equal to &"s"
        template<typename TargetSequence, typename SourceSequence>
        void toSmallRadix(TargetSequence &t, const SourceSequence &s, const ull &t_radix, const int & L) 
        {
            // cout <<"SZ " << sz(s) << endl;
            t.resize(sz(s) * L);
            for(int i = sz(s) - 1; i >= 0; -- i)
            {
                // cout << "CAO " << s[i] << " " << (i - L + 1) << endl;
                uintToSeq(t.begin() + i * L, s[i], t_radix, L);
            }
            // for(int i = 0; i < sz(t); ++ i)
            // {
            //     cout << "T:" << int(t[i]) << endl;
            // }
            removeLeadingZeros(t);
        }


        // normalize "v" of base "radix": if some elements in "v" is not less than "radix", 
        // then make them less than "radix", and adjust "v"
        template<typename Sequence>
        void normalize(Sequence &v, const ull & radix)
        {
            ull x = 0;
            for(int i = 0; i < sz(v); ++ i)
            {
                x = x / radix + v[i];
                v[i] = x % radix;
            }            
            while(x > 0) x /= radix, v.push_back(x % radix);
        }



        template<typename Sequence>
        void fromString(Sequence &v, string &s)
        {
            // cout << "formString" << s << " " << sz(s) <<  endl;
            reverse(s.begin(), s.end());
            for(int i = 0; i < sz(s); ++ i) s[i] -= '0';

            v.resize(sz(s) / 9 + bool(sz(s) % 9));
            toBigRadix(v, s, uint(10u), 9u);
        }
        template<typename Sequence>
        string toString(Sequence &v)
        {
            string s;
            toSmallRadix(s, v, 10u, 9u);
            for(int i = 0; i < sz(s); ++ i) s[i] += '0';
            reverse(s.begin(), s.end());
            return s;
        }


        // Sequence should be vector<uint> currently
        // a += b, assert(!a.empty()), assert(!b.empty())
        template<typename Sequence>
        void plusAssign(Sequence &a, const Sequence &b, const ull &radix) 
        {
            ull t = 0ull;
            int mi = min(sz(a), sz(b));
            for(int i = 0; i < mi; ++ i)
            {
                t += a[i];
                t += b[i];                
                a[i] = t % radix;
                t /= radix;
            }
            if(sz(a) < sz(b))
            {
                for(int i = sz(a); i < sz(b); ++ i) 
                {
                    t += b[i];
                    a.push_back(t % radix);
                    t /= radix;
                }                
            }
            else 
            {
                for(int i = sz(b); t > 0ull && i < sz(a); ++ i)
                {
                    t += a[i];
                    a[i] = t % radix;
                    t /= radix;
                }
            }
            while(t > 0ull) a.push_back(t % radix), t /= radix;
        }


        // Sequence should be vector<uint> currently
        // a -= b, assert(!a.empty()), assert(!b.empty()), assert(sz(a) >= sz(b))
        template<typename Sequence>
        void minusAssign(Sequence &a, const Sequence &b, const ull &radix) 
        {
            ull t = 0;
            for(int i = 0; i < sz(b); ++ i)
            {                    
                t += b[i];
                if(a[i] < t) 
                    a[i] = radix + a[i] - t, t = 1;
                else 
                    a[i] -= t, t = 0;
            }
            for(int i = sz(b); t > 0ull && i < sz(a); ++ i)
            {
                if(a[i] < t) 
                    a[i] = radix + a[i] - t, t = 1;
                else 
                    a[i] -= t, t = 0;
            }
            removeLeadingZeros(a);            
        }


        



        // typedef long long dint;
        // typedef vector<int> U;
        // const dint radix = 1000000000;
        // const int L = 9;


        // void fromString(U &a, const char *s, int n) // a should be empty
        // {
        //     a.clear();
        //     for(int i = n - 1; i >= 0; i -= L) 
        //     {
        //         int x = 0;     
        //         for(int j = max(0, i - L + 1); j <= i; ++ j)                
        //             x = x * 10 + int(s[j] - '0');                
        //         a.push_back(x);
        //     }
        // }
        // void fromString(U &a, const string &s) // a should be empty
        // {
        //     fromString(a, s.c_str(), s.size());
        // }


        


        // string& toString(string &s, const U &a) // s should be empty
        // {
        //     s.clear();
        //     s.reserve(sz(a) * L);

        //     char buf[10];
        //     sprintf(buf, "%d", a.back());
        //     s += buf;
        //     for(int i = sz(a) - 2; i >= 0; -- i)
        //     {
        //         sprintf(buf, "%09d", a[i]);
        //         s += buf;
        //     }
        //     return s;
        // }

        



        // void addAssign(U &a, const U &b) //a += b
        // {                
        //     int t = 0;
        //     for(int i = 0; i < sz(a); ++ i)
        //     {
        //         t += a[i];
        //         if(i < sz(b)) t += b[i];
                
        //         if(t >= radix) a[i] = t - radix, t = 1;
        //         else a[i] = t, t = 0;
        //     }
        //     for(int i = sz(a); i < sz(b); ++ i)
        //     {
        //         t += b[i];
        //         if(t >= radix) a.push_back(t - radix), t = 1;
        //         else a.push_back(t), t = 0;
        //     }
        //     if(t != 0) a.push_back(t);
        // }



        // void substractAssign(U &a, const U &b) // a -= b
        // {
        //     int t = 0;
        //     for(int i = 0; i < sz(a); ++ i)
        //     {                    
        //         if(i < sz(b)) t += b[i];
        //         if(a[i] < t) 
        //             a[i] = a[i] + radix - t, t = 1;
        //         else 
        //             a[i] -= t, t = 0;
        //     }
        //     int i;
        //     for(i = sz(a) - 1; i > 0 && a[i] == 0; -- i);
        //     a.resize(i + 1);
            
        // }

        // template<typename T>
        // void toVector10(vector<T> &target, const U &source) // s should be empty, Ascending
        // {
        //     target.clear();
        //     target.reserve(sz(source) * L);
        //     char buf[L + 1];
        //     for(int i = 0; i <= sz(source) - 2; ++ i)
        //     {
        //         sprintf(buf, "%09d", source[i]);
        //         for(int j = L - 1; j >= 0; -- j) target.push_back(buf[j] - '0');
        //     }
        //     sprintf(buf, "%d", source.back());
        //     int lb = strlen(buf);
        //     for(int j = lb - 1; j >= 0; -- j) target.push_back(buf[j] - '0');
        // }

        // template<typename T>
        // void fromVector10(U &target, const vector<T> &source) // a should be empty, Ascending
        // {   
        //     target.clear();         
        //     dint x = 0;

        //     for(int i = 0; i < sz(source); ++ i)
        //     {
        //         x = x / 10 + int(source[i]);
        //         target.push_back(x % 10);
        //     }
        //     while(x > 0) x /= 10, target.push_back(x % 10);
        //     while(sz(target) > 1 && target.back() == 0) target.pop_back();

        //     // cout << "target " << endl;
        //     // for(int i = 0; i < sz(target); ++ i) cout << target[i]; cout << endl;

        //     for(int i = 0; i < sz(target); i += L)
        //     {
        //         int x = 0;
        //         for(int j = min(i + L - 1, sz(target) - 1); j >= i; -- j)
        //         {
        //             x = x * 10 + target[j];
        //         }
        //         target[i / L] = x;
        //     }
        //     int n = sz(target) / L + bool(sz(target) % L);
        //     while(sz(target) > n) target.pop_back();

        // }
        // void simulationMultiply(U &c, const U &a, const U &b)
        // {
        //     c.resize(a.size() + b.size());
        //     for(int i = 0; i < sz(b); ++ i)
        //     {
        //         dint t = 0;
        //         for(int j = 0; j < sz(a); ++ j)
        //         {
        //             t += 1LL * b[i] * a[j] + c[i + j];
        //             c[i + j] = t % radix;
        //             t /= radix;
        //         }
        //         if(t > 0)
        //         {
        //             c[i + sz(a)] = t;
        //         }
        //     }
        //     if(c.back() == 0) c.pop_back();

        // }
        // void fftMultiply(U &c, const U &a, const U &b)
        // {
        //     // cout << "in fftMultiply" << endl;
        //     // string o;
        //     // cout << toString(o, a) << endl;
        //     // cout << toString(o, b) << endl;



        //     typedef IntegerFFTData::T T;
        //     vector<T> ra, rb;
        //     toVector10<T>(ra, a);
        //     toVector10<T>(rb, b);


        //     // for(int i = 0; i < sz(ra); ++ i) cout << ra[i]; cout << endl;

        //     // for(int i = 0; i < sz(rb); ++ i) cout << rb[i]; cout << endl;


        //     FFT<IntegerFFTData>::multiply(ra, rb);
        //     // for(int i = 0; i < sz(ra); ++ i) cout << ra[i]; cout << endl;


        //     fromVector10<T>(c, ra);

        //     // cout << "end fftMultiply" << endl;

        // }
        // void multiply(U &c, const U & a, const U &b) // c should be empty, c = a + b;
        // {
        //     dint n1 = 1LL * sz(a) * sz(b);
        //     dint n2 = 2LL * L * max(sz(a), sz(b));
        //     n2 = n2 * topBit(n2);            

        //     if(n1 <= n2)
        //     {
        //         simulationMultiply(c, a, b);

        //     }
        //     else 
        //     {
        //         fftMultiply(c, a, b);
                
        //     }
        // }









    }





class BigInteger 
{

};






}



#endif // BIGINTEGER_H
