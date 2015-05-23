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
#include "big_integer.h"
// #include "fft.h"

using namespace std;
using namespace lz;
using namespace lz::BigIntegerPrivate;




int main()
{
    UintSeq a, b, c;

    string s1 = "2147483649";
    string s2 = "18446744073709551615";
    fromString(s1.begin(), s1.end(), 10, a);
    fromString(s2.begin(), s2.end(), 10, b);

    multiplyFFT(a, b, 4, c);
    
    cout << toStringSlow(c, 10) << endl;

    c.clear();
    multiplySchool(a, b, c);
    
    cout << toStringSlow(c, 10) << endl;
    // UintSeq a, b, q, r;
    // int n = 1 << 13;
    // a.assign(n, ~0u);
    // a.back() = 1u << 31;

    // b.assign(n >> 1, ~0u);

    // divideAndRemainder2n1n(a, b, q, r, 0);


    // cout << "q::" <<toStringSlow(q, 16) << endl;
    // cout << "r::" <<toStringSlow(r, 16) << endl;






    // UintSeq tmp;
    // multiply(b, q, tmp);
    // lz::BigIntegerPrivate::plus(tmp, r);
    // cout << toStringSlow(tmp, 16) << endl;



    // time_t first, second;
    // first = clock();


    // string s1, s2;
    // UintSeq a, b, q, r;
    // cout <<"EE" << endl;

    // s1.assign(100000, '1');
    // s2.assign(100000, '9');

    // fromString(s1.begin(), s1.end(), 10, a);
    // fromString(s2.begin(), s2.end(), 10, b);

    // cout <<"CCC" << endl;
    // // cout << toStringSlow(a, 10) << endl;
    // // cout << toStringSlow(b, 10) << endl;

    // q.clear();
    // // cout << "SS" << endl;
    // second=clock();  
    // printf("toString complete:%.3f\n", double(second - first) / 1000.0);

    // multiply(a, b, q);
    // // multiplyFFT(a, b, 4, q );
    // // divideAndRemainderKnuth(a,  b, q, r);

    // second=clock();  
    // printf("multiply complete:%.3f\n", double(second - first) / 1000.0);


    // cout << toStringSlow(q, 10) << endl;



    


    // int T;
    // scanf("%d", &T);

    // string s1, s2;
    // UintSeq a, b, q, r;
    // while(T --)    
    // {
    //     cin >> s1 >> s2;
    //     fromString(s1.begin(), s1.end(), 10, a);
    //     fromString(s2.begin(), s2.end(), 10, b);


    //     // cout << toStringSlow(a, 10) << endl;
    //     // cout << toStringSlow(b, 10) << endl;

    //     q.clear();
    //     // cout << "SS" << endl;
    //     multiply(a, b, q);
    //     // multiplyFFT(a, b, 4, q );
    //     // divideAndRemainderKnuth(a,  b, q, r);


    //     cout << toStringSlow(q, 10) << endl;

    // }



    return 0;
}


