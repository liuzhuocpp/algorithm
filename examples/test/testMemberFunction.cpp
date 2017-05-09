#include <bits/stdc++.h>

#include <regex>
#include <iterator>
#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <unordered_set>
#include <tuple>
#include <fstream>
#include <list>
#include <map>
using namespace std;


template<typename T, T t>
struct Test
{
    static void apply(map<int, int> &a)
    {
        cout << (a.*t)(222) << endl;
    }
};

template<auto t>
constexpr auto makeTest()
{
    return Test<decltype(t), t>();
}


template<typename Ret, typename T, Ret T::* t>
struct UnaryTest
{
    static void apply(map<int, int> &a)
    {
//        cout << a.*t(455) << endl;
        cout << mem_fn(t)(a, 455) << endl;
    }
};



template<typename _Ret,
    typename _T>
struct UnaryTestDetail
{
    using Ret = _Ret;
    using T = _T;

};

template<typename Ret,   typename T >
auto unaryTestImpl(Ret (T::* pm)    )
{
    return UnaryTestDetail<Ret,  T>();
}

template<auto t>
constexpr auto makeUnaryTest()
{

    using Detail = decltype( unaryTestImpl(t)  );

    return UnaryTest<
        typename Detail::Ret,
        typename Detail::T,
        t>();



}

int main()
{
    cout << "FFF" << endl;

    map<int, int > a;

    a[222] = 333;
    a[455] = 111;
    auto ansTest = makeTest<static_cast<const int& (map<int,int>::*)(const int&) const>  ( &map<int, int >::at)>();
    ansTest.apply(a);




    auto unaryTestAns = makeUnaryTest<static_cast<const int& (map<int,int>::*)(const int&) const>  ( &map<int, int >::at)>();
    unaryTestAns.apply(a);







    return 0;
}



















