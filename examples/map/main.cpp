/*
 * main.cpp
 *
 *  Created on: 2015Äê8ÔÂ5ÈÕ
 *      Author: LZ
 */
#include <iostream>

#include <lz/map.h>

using namespace std;
using namespace lz;

int self(int &i)
{

	cout << "self: " << i << endl;
	return i;
}
//int self(const int &i)
//{
//	cout << "DOUBI " << endl;
//	return 100;
//}
int main()
{

	int a = 1123;

	auto fm = makeFunctionMap(self);
	fm[a];






	return 0;
}



