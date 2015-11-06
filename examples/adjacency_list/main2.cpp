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

IntegerIterator<int> gg(0);


int main()
{
//	gg = 2;
//	*gg = 4;
	++gg;
	++gg;
	++gg;

	--gg;

	cout << *gg << endl;


    return 0;
}











