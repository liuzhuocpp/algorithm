/*
 * parameter.cpp
 *
 *  Created on: 2015��11��30��
 *      Author: LZ
 */



#include <iostream>
#include <cstdio>
#include <cstring>
#include <tuple>
#include <vector>
#include <ctime>
#include <forward_list>
#include <functional>
#include "lz/parameter.h"


using namespace std;
using namespace lz;





LZ_PARAMETER_KEYWORD_TAG_NAMESPACE(graph)    // Note: no semicolon
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

int f()
{
    return 0;
}
int main()
{


    auto store = EmptyParamPack();

    cout << is_same<decltype(store[aa | 0]), int&&>::value << endl;

    int a;
    cout << is_same<decltype(store[aa | f()]), int&&>::value << endl;



    auto i = store[aa | f()];

    cout << is_same<decltype(i), int>::value << endl;

//    cout << is_same<decltype(store[aa | 0]), int&&>::value << endl;


//	const int &a = int(213123);
//
//	double K = 1222.312221111;
//
//
//	auto store =
//			EmptyParamPack();
//
////			(aa = 3333
////			, bb = K
//////			, cc= "***********"
////			);
//
//	 auto aa_val = store[aa | 10];
//
//	 cout << store[aa | 9999] << endl;
//	 cout << store[cc | string("LSKDFJLSJDFLJ")] << endl;
//
//	 auto lazy = std::bind(std::plus<string>(), string("IIIIIIIi"), string("JJJJJJJJj")  );
//
////	 cout << lazy() << endl;
////	 cout << lazy() << endl;
//
//	 store[cc ||  lazy  ];
//
//	 cout << store[cc ||  lazy  ] << endl;


















	return 0;
}


















