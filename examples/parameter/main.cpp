/*
 * parameter.cpp
 *
 *  Created on: 2015Äê11ÔÂ30ÈÕ
 *      Author: LZ
 */



#include <iostream>
#include <cstdio>
#include <cstring>
#include <tuple>
#include <vector>
#include <ctime>
#include <forward_list>

#include "lz/parameter.h"


using namespace std;
using namespace lz;





LZ_PARAMETER_KEYWORD_TAG_NAMESPACE( graph)    // Note: no semicolon
LZ_PARAMETER_KEYWORD_TAG_NAMESPACE(visitor)
LZ_PARAMETER_KEYWORD_TAG_NAMESPACE(root_vertex)
LZ_PARAMETER_KEYWORD_TAG_NAMESPACE(index_map)
LZ_PARAMETER_KEYWORD_TAG_NAMESPACE(color_map)
LZ_PARAMETER_KEYWORD_TAG_NAMESPACE(aa)
LZ_PARAMETER_KEYWORD_TAG_NAMESPACE(bb)
LZ_PARAMETER_KEYWORD_TAG_NAMESPACE(cc)
LZ_PARAMETER_KEYWORD_TAG_NAMESPACE(dd)
LZ_PARAMETER_KEYWORD_TAG_NAMESPACE(ee)



template <class ArgumentPack>
void depth_first_search(ArgumentPack && args)
{

}


int main()
{
	const int &a = int(213123);

	double K = 1222.312221111;


	auto store = (aa = 3333
			, bb = K
			);

//	auto hehe = store[cc];

	cout << store[aa] << endl;
//	cout << store[bb] << endl;


	auto cc_def = cc | 200;
	auto cc_val = store[cc_def];

	cout << cc_val << endl;
	cout << is_same<decltype(store[cc | 200]), int&&>::value << endl;

	cout << store[cc | 200] << endl;

//	cout << is_same<decltype(store[aa]), int&&>::value << endl;
//	cout << is_same<decltype(store[bb]), double&>::value << endl;
//
//	cout << is_same<decltype(hehe), ParamNotFound>::value << endl;


//	printf("%f\n", K);
//	printf("%f\n", store[bb]);




















	return 0;
}


















