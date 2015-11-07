/*
 * main2.cpp
 *
 *  Created on: 2015Äê11ÔÂ3ÈÕ
 *      Author: LZ
 */

#include <iostream>
#include <cstdio>
#include <cstring>
#include <tuple>
#include <vector>


#include "lz/adjacency_list.h"
#include "lz/map.h"
#include "lz/property.h"
#include "lz/iterator.h"

using namespace std;
using namespace lz;
using II = IntegerIterator<int>;
II gg(0);


int main()
{
//	gg = 2;
//	*gg = 4;
	++gg;
	++gg;
	++gg;
	gg++;
	II kk = gg;
//	cout << *gg << endl;

	kk = 2 + gg;
	kk = 10 + gg;
//	cout << (kk - gg) << endl;
	gg += 11;
	cout << *gg << endl;
	cout <<  *kk << endl;

	gg -= 10;
	cout << *gg << endl;
	cout << gg[10] << endl;





    return 0;
}











