#include <bits/stdc++.h>


//using namespace lz;
using namespace std;


//template<typename Tuple, int i, typename F>
//struct ForEachTuple
//{
//
//    static void run(Tuple t, F f)
//    {
//        f(std::get<i>(t));
//        if constexpr(i < std::tuple_size<t>::value)
//        {
//            ForEachTuple<Tuple, i + 1, F>::run(t, f);
//        }
//    }
//};

template<int i, typename Tuple, typename F>
void forEachTuple(Tuple t, F f)
{
    f(i, std::get<i>(t));
    if constexpr(i + 1 < std::tuple_size<Tuple>::value)
    {
        forEachTuple<i + 1>(t, f);
    }

}






auto f = [](int i, auto x)
{
    cout << " i = " << i << " " <<   x << endl;
};
int main()
{

    forEachTuple<0>(make_tuple(123, 'z', 2.3), f);
    return 0;
}


















