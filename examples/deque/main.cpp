/*
 * main.cpp
 *
 *  Created on: 2015��9��13��
 *      Author: LZ
 */

#include <iostream>
#include <lz/deque.h>

using namespace std;
using namespace lz;

int main()
{

	Deque<int> d;
	d.push_back(10);
	d.push_back(30);
	d.push_back(40);
	d.push_back(1000);
	d.push_back(-10);
	d.push_back(-123);
	d.push_back(-1031231);





	d.out();




	return 0;
}



