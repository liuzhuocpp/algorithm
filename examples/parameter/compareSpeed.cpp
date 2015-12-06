/*
 * compareSpeed.cpp
 *
 *  Created on: 2015Äê12ÔÂ4ÈÕ
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
//struct int_tag{};
//struct string_tag{};




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


long long Test1 = 0;
template <class ArgumentPack>
void depth_first_search(ArgumentPack && args)
{
       Test1 += args[aa] ^ args[bb]+ args[cc] * args[dd] - args[ee];
}

long long Test2 = 0;


//forward_list<int> fl;
void test(char &&a, int &&b, double &&c, string &&d, bool &&e,
          int&aa,int&bb,int&cc,int&dd,int&ee)
{



    Test2 += aa ^ bb + cc * dd - ee;

}

const int N = 1e7 + 9;
struct tag_int{};
struct tag_char{};
int main()
{

//  int a = 10;
//  char ch = 'z';
//  tuple<int&, char&> tp(a, ch);
//
//  Property<tag_int, int&,
//  Property<tag_char, char&> > p(tp);

    using PChar = Property<tag_char, char>;
    PChar pchar('z', NoProperty());

    Property<tag_int, int,
    PChar> p(11111, pchar);


    cout << "FJJF      " << p[tag_int()] << endl;
    cout << "FJJF      " << p[tag_char()] << endl;



//    int a1, b1, c1, d1, e1;
//    scanf("%d%d%d%d%d", &a1, &b1, &c1, &d1, &e1);
//    int tp;
//    cout << "_____" << a1 << " " << b1 << " " << c1 << " " << d1 << " " << e1 << endl;
//    auto  params = (graph = 'T', visitor = 2, root_vertex = 3.5,
//                     index_map = string("hello, world"), color_map = false,
//                     aa = tp,
//                     bb = b1,
//                     cc = c1,
//                     dd = d1,
//                     ee = e1
//
//                     );
//
//    string ss = string("hello, world");
//    double first = clock();
//
//    for(int i = 0; i < N; ++ i)
//    {
//      int ta1 = a1 + i;
//
////        decltype(cnt) params;
//      params[aa] = ta1;
//      params[bb] = b1;
//      params[cc] = c1;
//      params[dd] = d1;
//      params[ee] = e1;
//
//      params[graph] = 'T';
//      params[visitor] = 2;
//      params[root_vertex] = 3.5;
//
////        params[index_map] = move(ss) ;
//
//      params[color_map] = false;
////
//
//           depth_first_search(
//                       params
//
//                       );
//    }
//
//
//
//
//    double second = clock();
//
//    cout << (second - first) / 1000.0 << endl;
//    cout << "_____" << a1 << " " << b1 << " " << c1 << " " << d1 << " " << e1 << endl;
//    first = clock();
//
//    for(int i = 0; i < N; ++ i)
//    {
//      int ta1 = a1 + i;
//
//          test('T', 2, 3.5, move(ss), false,
//           ta1, b1, c1, d1, e1);
//    }
//    second = clock();
//
//    cout << (second - first) / 1000.0 << endl;
//
//
//    cout << Test1 << endl;
//    cout << Test2 << endl;
//
//
//
//
//    cout << "_____" << a1 << " " << b1 << " " << c1 << " " << d1 << " " << e1 << endl;
//
//
//
//
//
//








    return 0;
}


















