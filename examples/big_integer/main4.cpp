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
    
    string s = "100123123123123112312323";
    cout << s << "#size:" << s.size() << endl;
    
    UintSeq a;
    fromString(s.begin(), s.end(), 10, a);        
    // cout << "KK:" << toStringSlow(a) << endl;
    
    
    // shiftHigh(a, 66);
    // shiftLow(a, 40);
    s = toStringSlow(a, 10);

    // cout << "FF" << endl;
    cout << s << "#size:" << s.size() << endl;





    return 0;
}


