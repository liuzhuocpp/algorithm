/*
 * asm.cpp
 *
 *  Created on: 2015��8��8��
 *      Author: LZ
 */




#include <vector>

//struct D
//{
//	int a, b, c, d;
//};
typedef std::vector<int> D;
D f()
{
	D a;
	return a;
}

void g()
{
	int a = 10;
	D d
	= f()
	;
}
