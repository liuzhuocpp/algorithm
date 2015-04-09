#ifndef BIGINTEGER_H
#define BIGINTEGER_H


#include <limits>
#include <vector>
#include <iostream>
#include "fft.h"

namespace lz {

using std::vector;
using std::numeric_limits;
using std::cout;
using std::endl;
using std::string;

    namespace BigIntegerPrivate {

        typedef unsigned uint;
        typedef unsigned long long duint;
        typedef vector<uint> super;




        class U: private super
        {
            template<typename T> 
            static int sz(const T &o) { return int(o.size()); }

            constexpr static duint radix = 10000000000;
            

            explicit U(super &a):super(a) {}
        public:
            U():super(1, 0) {}

            explicit U(const uint &a): super(1, a) { }
            string toString() const
            {

            }

            friend U operator+(const U &a, const U &b)
            {
                super r;
                duint t = 0;

                const U *pa, *pb;
                if(sz(a) < sz(b)) pa = &a, pb = &b;
                else pa = &b, pb = &a;
                const U &ra = *pa, &rb = *pb;

                for(int i = 0; i < sz(rb); ++ i)
                {
                    t += rb[i];
                    if(i < sz(ra)) t += ra[i];
                    
                    if(t >= radix) r.push_back(t - radix), t = 1;
                    else r.push_back(t), t = 0;
                }
                if(t != 0) r.push_back(t);
                return U(r);
            }



        
            

        };


    }

    class BigInteger 
    {

    };






}



#endif // BIGINTEGER_H
