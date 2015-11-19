//#include <iostream>

//using namespace std;
template<bool con>
struct Choose
{
    template<typename T, typename F>
    static void run(T t, F f)
    {
        t();
    }
};
template<>
struct Choose<0>
{
    template<typename T, typename F>
    static void run(T t, F f)
    {
        f();
    }
};

void GG()
{
	int aaa;

}
template<typename FFF>
void f()
{

}



int test()
{
    constexpr bool con = 1;

    auto t = [](){ int a = 9998; };
    auto f = [](){ int b = 10010010; };

//    t();
    Choose<0>::run(f, t);
    Choose<0>::run(t, f);



    return 0;
}





















