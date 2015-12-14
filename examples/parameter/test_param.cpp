/*
 * main.cpp
 *
 *  Created on: 2015Äê11ÔÂ20ÈÕ
 *      Author: LZ
 */


#include <iostream>
#include <cstdio>
#include <cstring>
#include <tuple>
#include <ctime>

#include "lz/adjacency_list.h"
#include "lz/parameter.h"
//#include "lz/strongly_connected_components.h"

using namespace std;
using namespace lz;


using G = AdjacencyList<>;
















LZ_PARAMETER_KEYWORD(tag, aa)
LZ_PARAMETER_KEYWORD(tag, bb)
LZ_PARAMETER_KEYWORD(tag, cc)
LZ_PARAMETER_KEYWORD(tag, dd)
LZ_PARAMETER_KEYWORD(tag, ee)



long long Test1 = 0;


template <class ArgumentPack>
void f1(ArgumentPack && args)
{
	   Test1 += args[aa] ^ args[bb]+ args[cc] * args[dd] - args[ee];
}




long long Test2 = 0;
void f2(int&aa,int&bb,int&cc,int&dd,int&ee)
{
	Test2 += aa ^ bb + cc * dd - ee;


}

const int N = 1e9 + 9;
int test()
{

  int a1, b1, c1, d1, e1;
  scanf("%d%d%d%d%d", &a1, &b1, &c1, &d1, &e1);

  double first = clock();


  for(int i = 0; i < N; ++ i)
  {
 	 f1(

 		 (
		   aa = a1,
		   bb = b1,
		   cc = c1,
		   dd = d1,
		   ee = e1

		   )
				 );
  }




  double second = clock();

  cout << (second - first) / 1000.0 << endl;

  first = clock();

  for(int i = 0; i < N; ++ i)
  {
 	 f2(
 			 a1, b1, c1, d1, e1);
  }
  second = clock();

  cout << (second - first) / 1000.0 << endl;


  cout << Test1 << endl;
  cout << Test2 << endl;

  return 0;
}




int main()
{

	test();
	return 0;

//	Tmp && t = Tmp();

//	cout << "JJJJFDKJLSF" << endl;


//	int tbb = 133;
//	auto bb = f(int(133));
//
//	cout << bb.a << endl;
//
//	auto cc = g(int(5555));
//	cout << cc.a << endl;







//	return 0;


//	int ta = 1333;
//	auto param = (
//			aa = move(ta)
//			);
//	cout << "PPP " << param.ref << endl;
//	cout << param[aa] << endl;
//	test();


//	G g;
//	int n = 5;
//	for(int i = 0; i < n; ++ i)
//		g.addVertex();
//	g.addEdge(1, 2);
//	g.addEdge(2, 3);
//	g.addEdge(3, 4);
//	g.addEdge(4, 1);
//
//	vector<int> a(n);
//	cout << "FFF " << endl;




//	using namespace DepthFirstSearchKeywords;
//	stronglyConnectedComponents
//	int comp = stronglyConnectedComponents(g, makeIteratorMap(a.begin()));
//
//	cout << comp << endl;


//	P p(g);

//	depthFirstSearch(g, p);









	return 0;
}

















