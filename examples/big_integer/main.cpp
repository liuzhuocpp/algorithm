#include <iostream>
#include <cstdio>
#include <limits>


#include "big_integer.h"

using namespace std;
using namespace lz;
using namespace lz::BigIntegerPrivate;


const int N = 10000;
char s[N];

int main()
{
    scanf("%s", s);
    U a(s);
    // cout << a.toString() << endl;
    scanf("%s", s);
    U b(s);
    // cout << b.toString() << endl;


    // U c(a);
    // cout << c.addAssign(b).toString() << endl;

    
    cout << a.substractAssign(b).toString() << endl;


    return 0;
}