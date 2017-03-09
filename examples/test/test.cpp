
#include <bits/stdc++.h>


#include <vector>
using namespace std;

template<typename ...T>
void f(T&& ...t)
{
    (cout << t<< " ", ...);
}

//#define a
//#ifdef a
//
//#else
//
//#endif
template<int t, typename FF>
void g(FF ff)
{
    if constexpr (t == 0)
    {
        cout << 'h' << endl;
    }
    else
    {
        ff(10);
    }
}


int main()
{
//    f(1, 3, 5, 6, 76);
//    auto [a, b] = make_tuple(3, 'z');

    g<0, int>(int());


	return 0;
}







