

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
#include "heap.h"
using namespace std;
using namespace lz;
using namespace lz::BigIntegerPrivate;

class BB
{
public: 
    void gg()
    {

    }   
     int b;

};


int main()
{
    time_t first, second;
    BB b;
    // Heap h;


    



    first = clock();
    string s = "9999123123121231231231241230000000001231231233123";

    s.assign(500000, '9');
    // cout << s << endl;
    WordSeq a;
    a.reserve(100000);

    fromString(s.begin(), s.end(), 10, a);
    // cout << "SB" << endl;
    // out(a);
    cout << "comp" << endl;
    second=clock();  
    printf("fromString complete:%.3f\n", double(second - first) / 1000.0);
    
    string ans = toStringSlow(a);    
    // cout << s << endl;
    // cout << ans << endl;

    cout << "OK: " <<  (s == ans) << endl;
    second=clock();  
    printf("toString complete:%.3f\n", double(second - first) / 1000.0);
    // cout << (second - first)<< endl;
    


    return 0;
}


