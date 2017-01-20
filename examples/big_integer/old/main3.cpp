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
    






    int T, cas = 1;
    scanf("%d", &T);
    WordSeq a, b;
    string s;
    while(T --)
    {
        if(cas > 1) puts("");
        cin >> s;
        printf("Case %d:\n", cas ++);
        cout << s << " + ";
        fromString(s.begin(), s.end(), 10, a);        
        cin >> s;
        cout << s << " = ";
        fromString(s.begin(), s.end(), 10, b);
        
        BigIntegerPrivate::plus(a, b);
        s = toStringSlow(a);
        cout << s << endl;
    }


    

    



    return 0;
}


