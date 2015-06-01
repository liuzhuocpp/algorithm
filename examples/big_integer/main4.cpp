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

    int first, second;



    string s1, s2;
    UintSeq a, b, q, r;
    cout <<"EE" << endl;

    int n = 3e5;
    s1.assign(n, '1');
    s2.assign(n, '1');
    s2.back() = '4';

    fromString(s1.begin(), s1.end(), 10, a);
    fromString(s2.begin(), s2.end(), 10, b);

    cout <<"CCC" << endl;

    q.clear();

    cout << "A.size(): " << a.size() << endl;

    second=clock();  
    printf("toString complete:%.3f\n", double(second - first) / 1000.0);

    // multiplyFFT(a, b, 4, q);
    multiplyKaratsuba(a, b,  q);
    // multiplySchool(a, b, q);
    // divideAndRemainderKnuth(a,  b, q, r);

    int third=clock();  
    printf("multiply complete:%.3f\n", double(third - second) / 1000.0);
    cout << "bitlength a:" << bitLength(a) << endl;


    // cout << toStringSlow(q, 10) << endl;



    


    // int T;
    // scanf("%d", &T);

    // string s1, s2;
    // UintSeq a, b, q, r;
    // while(cin >> s1 >> s2)    
    // {
        
    //     fromString(s1.begin(), s1.end(), 10, a);
    //     fromString(s2.begin(), s2.end(), 10, b);


    //     // cout << toStringSlow(a, 10) << endl;
    //     // cout << toStringSlow(b, 10) << endl;

    //     q.clear();
    //     // cout << "SS" << endl;
    //     // multiply(a, b, q);
    //     multiplyKaratsuba(a, b, q);
    //     // multiplyFFT(a, b, 4, q );
    //     // divideAndRemainderKnuth(a,  b, q, r);


    //     cout << toStringSlow(q, 10) << endl;

    // }



    return 0;
}


