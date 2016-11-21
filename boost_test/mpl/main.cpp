// Copyright David Abrahams 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <boost/mpl/map.hpp>
#include <boost/mpl/map/map50.hpp>
#include <boost/mpl/assert.hpp>
//using namespace std;
using namespace boost;
using namespace mpl;


int main()
{
	typedef map<
	      pair<int,unsigned>
	    , pair<char,unsigned char>
	    , pair<long_<5>,char[17]>
	    , pair<int[42],bool>
	    > m;

//	BOOST_MPL_ASSERT_RELATION( size<m>::value, ==, 4 );
//	BOOST_MPL_ASSERT_NOT(( empty<m> ));
//
//	BOOST_MPL_ASSERT(( is_same< at<m,int>::type, unsigned > ));
//	BOOST_MPL_ASSERT(( is_same< at<m,long_<5> >::type, char[17] > ));
//	BOOST_MPL_ASSERT(( is_same< at<m,int[42]>::type, bool > ));
//	BOOST_MPL_ASSERT(( is_same< at<m,long>::type, void_ > ));



	return 0;
}











