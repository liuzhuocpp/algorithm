/*
 * main.cpp
 *
 *  Created on: 2016Äê3ÔÂ8ÈÕ
 *      Author: LZ
 */



#include <iostream>
#include <cstdio>
#include <cstring>
#include "lz/shared_ptr.h"

using namespace std;
using namespace lz;

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


void f()
{
	SharedPtr<AA> sp(new AA());
	SharedPtr<AA> sp2 = sp;


}

int main()
{
	f();


	cout << "GG" << endl;



	return 0;
}














