#include <iostream>
#include <cstdio>
#include <limits>


#include "big_integer.h"

using namespace std;
using namespace lz;
using namespace lz::BigIntegerPrivate;


const int N = 10000;
string s;
U a, b;
int main()
{
    cin >> s;
    fromString(a, s);
    // cout << "FFS:" << s << endl;
    s.clear();
    toString(s, a);
    // cout << s << endl;

    cin >> s;
    fromString(b, s);
    s.clear();
    toString(s, b);
    // cout << s << endl;
    U c;

    multiply(c, a, b);
    s.clear();
    toString(s, c);
    cout << s << endl;


    return 0;
}