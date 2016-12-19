/*
 * AA.cpp
 *
 *  Created on: 2015Äê11ÔÂ19ÈÕ
 *      Author: LZ
 */



#include <iostream>
#include <functional>
#include <map>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <set>
#include <tuple>
using namespace std;

struct AA
{
    AA()
    {
        cout << "CON" << endl;
    }
    AA(const AA &o)
    {
        cout << "copy con" << endl;
    }
    AA(AA &&o)
    {
        cout << "move con" << endl;
    }
    AA& operator=(const AA &o)
    {
        cout << "= copy" << endl;
    }
    AA& operator=(AA &&o)
    {
        cout << "= move " << endl;
    }
    ~AA()
    {
        cout << "des " << endl;
    }
};

int main()
{


    return 0;
}





















