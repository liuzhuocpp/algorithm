// Copyright David Abrahams 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <boost/parameter/keyword.hpp>
#include <chrono>
#include <ctime>
#include <vector>
using namespace std;


   BOOST_PARAMETER_KEYWORD(tag, aa)
   BOOST_PARAMETER_KEYWORD(tag, bb)
   BOOST_PARAMETER_KEYWORD(tag, cc)
   BOOST_PARAMETER_KEYWORD(tag, dd)
   BOOST_PARAMETER_KEYWORD(tag, ee)



long long Test1 = 0;


template <class ArgumentPack>
void f1(ArgumentPack && args)
{
//	   Test1 += args[aa] ^ args[bb]+ args[cc] * args[dd] - args[ee];
}




long long Test2 = 0;
void f2(int&aa,int&bb,int&cc,int&dd,int&ee)
{
//	Test2 += aa ^ bb + cc * dd - ee;


}

const int N = 1e7 + 9;
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
}











