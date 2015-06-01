#include <iostream>
#include <typeinfo>
using namespace std;

struct NoP{};

template<typename _T, typename _Tag, typename _NP = NoP>
struct P
{
    typedef _Tag Tag;
    typedef _NP NP;
    typedef _T T;

    _T val;
    _NP np;

};

template<typename _T, typename _Tag>
struct P<_T, _Tag, NoP>
{
    typedef _Tag Tag;
    typedef NoP NP;
    typedef _T T;
    _T val;
};

template<typename P, typename Tag, typename QueryTag>
struct GetP
{
    static auto getP(P &p)
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
{
    return GetP<P, typename P::Tag, QueryTag>::getP(p);
}



struct W1Tag{};
struct W2Tag{};
struct W3Tag{};
struct W4Tag{};




int main()
{
//    P<int,    W1Tag, NoP> p2;
//    CC<decltype(p2)>::getP(p2);



//    return 0;
    P<int,    W1Tag,
    P<double, W2Tag,
    P<char,    W3Tag, NoP> > > cnt;
//    cout << sizeof(cnt) << endl;
    cnt.val = 11;
    cnt.np.val = 123.11;
    cnt.np.np.val = 'A';


    cout << cnt.val << endl;
    cout << cnt.np.val << endl;
    cout << cnt.np.np.val << endl;

    cout << getP<W1Tag>(cnt) << endl;




//    cout << get()

//    cout << typeid(cnt.val).name() << endl;
//    cout << typeid(cnt.np->val).name() << endl;
//    cout << typeid(cnt.np->np->val).name() << endl;


//    cnt.val = 10.123;
//    cout << cnt.val << endl;










    return 0;
}
