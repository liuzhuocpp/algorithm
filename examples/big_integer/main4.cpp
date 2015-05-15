#include <iostream>
#include <cstdio>
#include <limits>
#include <vector>
// #include <complex>
// #include <cmath>
#include <algorithm>
#include <cstring>
#include <ctime>

#include "big_integer.h"


using namespace std;
using namespace lz;
using namespace lz::BigIntegerPrivate;




int main()
{


    UintSeq a, b, q, r;
    string s1(20000, '9');
    string s2(10000, '9');
    fromString(s1.begin(), s1.end(), 10, a);
    fromString(s2.begin(), s2.end(), 10, b);



    divideAndRemainderKnuth(a,  b, q, r);


    cout << toStringSlow(q, 10) << endl;

    // int T;
    // scanf("%d", &T);

    // string s1, s2;
    // UintSeq a, b, q, r;
    // while(T --)    
    // {
    //     cin >> s1 >> s2;
    //     fromString(s1.begin(), s1.end(), 10, a);
    //     fromString(s2.begin(), s2.end(), 10, b);



    //     divideAndRemainderKnuth(a,  b, q, r);


    //     cout << toStringSlow(q, 10) << endl;

    // }



    return 0;
}


