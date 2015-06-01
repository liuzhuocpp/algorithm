#include <iostream>
#include <typeinfo>
#include <vector>

using namespace std;



struct NoP{};

template<typename P>
struct CountP
{
    static constexpr int num =
        CountP<typename P::NP>::num + 1;

};
template<>
struct CountP<NoP>
{
    static constexpr int num = 0;
};

template<typename...T>
struct CountV
{
    static constexpr int num = 0;
};
template<typename Head, typename...T>
struct CountV<Head, T...>
{
    static constexpr int num = CountV<T...>::num + 1;

};

template<>
struct CountV<>
{
    enum {
        num = 0,
    };
};



template<typename _T, typename _Tag, typename _NP = NoP>
struct P
{
    typedef _Tag Tag;
    typedef _NP NP;
    typedef _T T;

//    P(_T _val):val(_val) {}
    P(){}
    template<typename Head, typename... Args>
    P(const Head &_val, Args... args): val(_val)
    , np(args...)
    {
        cout <<"NUMP: " <<  CountP<NP>::num << endl;
        cout <<"NUMV: " <<  CountV<Args...>::num << endl;
//        constexpr int pnum = CountP<P>::num;
//        constexpr int vnum = CountV<Head, Args...>::num;

//        assert(pnum == vnum);
//        if(pnum != vnum)
//        {
//            assert(pnum == vnum)
//            cout << "CAO" << endl;
//            return;
//        }
        static_assert(CountP<P>::num == CountV<Head, Args...>::num,
                       "Parameters number is not equal");
//         np = args ...;


//        static_assert(is_same<Head, _T>::value, "type error");
//        cout <<"SB" << endl;
    }

    _T val;
    _NP np;

};

template<typename _T, typename _Tag>
struct P<_T, _Tag, NoP>
{
    typedef _Tag Tag;
    typedef NoP NP;
    typedef _T T;
    P(){}
    P(const _T &_val):val(_val){}
    _T val;
};

template<typename P, typename Tag, typename QueryTag>
struct GetP
{
    static auto getP(P &p)
    ->decltype(GetP<typename P::NP, typename P::NP::Tag, QueryTag>::getP(p.np))
    {
        return GetP<typename P::NP, typename P::NP::Tag, QueryTag>::getP(p.np);
    }
};

template<typename P, typename QueryTag>

struct GetP<P, QueryTag, QueryTag>
{
    static typename P::T getP(P &p)
    {
        return p.val;
    }
};


template<typename QueryTag, typename P>
auto getP(P &p)
-> decltype(GetP<P, typename P::Tag, QueryTag>::getP(p))
{
    return GetP<P, typename P::Tag, QueryTag>::getP(p);
}



struct W1Tag{};
struct W2Tag{};
struct W3Tag{};
struct W4Tag{};






int main()
{




//    return 0;
    P<int,    W1Tag,
    P<double, W2Tag,
    P<char,   W3Tag,
    P<string, W4Tag> > > >
//    cnt(888, 22.999);
    cnt(888, 22.999, 'z'
        , "SBSB"
        );
//    cout << sizeof(cnt) << endl;
//    cnt.val = 11;
//    cnt.np.val = 123.11;
//    cnt.np.np.val = 'A';


    cout << cnt.val << endl;
    cout << cnt.np.val << endl;
    cout << cnt.np.np.val << endl;
    cout << cnt.np.np.np.val << endl;

    cout << getP<W1Tag>(cnt) << endl;
    cout << getP<W2Tag>(cnt) << endl;
    cout << getP<W3Tag>(cnt) << endl;
    cout << getP<W4Tag>(cnt) << endl;

    cout << CountP<decltype(cnt)>::num << endl;

    cout << CountV<>::num << endl;









//    cout << get()

//    cout << typeid(cnt.val).name() << endl;
//    cout << typeid(cnt.np->val).name() << endl;
//    cout << typeid(cnt.np->np->val).name() << endl;


//    cnt.val = 10.123;
//    cout << cnt.val << endl;










    return 0;
}

