

#include <bits/stdc++.h>

#include <lz/debug.h>

#include <lz/big_integer/core.h>
#include <lz/std_utility.h>

using namespace lz;
using namespace std;
using namespace lz::BigIntegerPrivate;



template<typename T>
void callPlusAssign(vector<T> &a, typename vector<T>::iterator end, vector<T> & b)
{
    assert(a.size() >= b.size());
    auto carry = plusAssign(a.begin(), end, b.begin(), b.end(), 10);
    if(carry)
    {
        a.push_back(1);
    }
}

void testPlusAssign()
{
    vector<int> a = {8, 9, 9, 0, 0, 0};
    vector<int> b = {6, 4, 9, 9, 9, 9};
    callPlusAssign(a, a.begin() + 3, b);
    cout << a << endl;
    assert(isEqual(a.begin(), a.end(), {4,4,9,0,0,0,1} ));

    a = {};
    b = {};
    callPlusAssign(a, a.begin(), b);
    cout << a << endl;
    assert( isEqual(a.begin(), a.end(), {} ) );


    a = {0, 0, 0, };
    b = {1, 9, 9, };
    callPlusAssign(a, a.begin(), b);
    cout << a << endl;
    assert( isEqual(a.begin(), a.end(), {1, 9, 9} ) );

    a = {4, 5, 6, };
    b = {};
    callPlusAssign(a, a.begin(), b);
    cout << a << endl;
    assert( isEqual(a.begin(), a.end(), {4, 5, 6} ) );

}




void testMinusAssign()
{
    vector<int> a = {0, 0, 0, 9, 9, 9, };
    vector<int> b = {0, 9, 9, };
    auto aend = minusAssign(a.begin(), a.end(), b.begin(), b.end(), 10);
    cout << lz::makeIteratorRange(a.begin(), aend) << endl;
    assert(isEqual(a.begin(), aend, {0, 1, 0, 8, 9, 9} ));


    a = {};
    b = {};
    aend = minusAssign(a.begin(), a.end(), b.begin(), b.end(), 10);
    cout << lz::makeIteratorRange(a.begin(), aend) << endl;
    assert(isEqual(a.begin(), aend, {} ));


    a = {1, 5, 6, 7};
    b = {};
    aend = minusAssign(a.begin(), a.end(), b.begin(), b.end(), 10);
    cout << lz::makeIteratorRange(a.begin(), aend) << endl;
    assert(isEqual(a.begin(), aend, {1, 5, 6, 7} ));
}
void testMultiplySchool()
{
    vector<int> a = {9, 9, 9,};
    vector<int> b = {0, 9, 9,};
    vector<int> c = {2, 3, 5, 5, 6, 6, 7, 7, 7};
    auto cend = multiplySchool(a.begin(), a.end(), b.begin(), b.end(), c.begin(), 10);
    cout << makeIteratorRange(c.begin(), cend) << endl;
    assert(isEqual(c.begin(), cend, {0, 1, 0,9, 8, 9} )  );


    a = {9, 9, 9,};
    b = {};
    c.assign(10, -2);
    cend = multiplySchool(a.begin(), a.end(), b.begin(), b.end(), c.begin(), 10);
    cout << makeIteratorRange(c.begin(), cend) << endl;
    assert(isEqual(c.begin(), cend, {} )  );

    a = {};
    b = {3, 4, 5 };
    c.assign(10, -2);
    cend = multiplySchool(a.begin(), a.end(), b.begin(), b.end(), c.begin(), 10);
    cout << makeIteratorRange(c.begin(), cend) << endl;
    assert(isEqual(c.begin(), cend, {} )  );

    a = {};
    b = {};
    c.assign(10, -2);
    cend = multiplySchool(a.begin(), a.end(), b.begin(), b.end(), c.begin(), 10);
    cout << makeIteratorRange(c.begin(), cend) << endl;
    assert(isEqual(c.begin(), cend, {} )  );

}


void callMultiplyAssignSchool_multiplyWord(vector<int> &a, int b, int radix)
{
    int carry = multiplySchool(a.begin(), a.end(), b, a.begin(), radix);
    integerRadixTransform(carry, std::back_inserter(a), radix);
}

void testMultiplyAssignSchool_multiplyWord()
{

    vector<int> a = {1, 9, 9,};
    int b = 42;
    callMultiplyAssignSchool_multiplyWord(a, b, 10);
    cout << a << endl;
    assert(isEqual(a.begin(), a.end(), {2, 2, 6, 1, 4} )  );

    a = {};
    callMultiplyAssignSchool_multiplyWord(a, b, 10);
    cout << a << endl;
    assert(isEqual(a.begin(), a.end(), {} )  );
}


void callPlusAssign_plusWord(vector<int> &a, int b, int radix)
{
    auto carry = plusAssign(a.begin(), a.end(), b, radix);
    integerRadixTransform(carry, std::back_inserter(a), radix);
}


void testPlusAssign_plusWord()
{
    OUT_FUNCTION_NAME

    vector<int> a = {1, 9, 9,};
    int b = 42999;
    callPlusAssign_plusWord(a, b, 10);
    cout << a << endl;
    assert ( (a == vector<int>{0, 9, 9, 3, 4}) )  ;


    a = {1, 9, 9,};
    b = 0;
    callPlusAssign_plusWord(a, b, 10);
    cout << a << endl;
    assert ( (a == vector<int>{1, 9, 9}) )  ;

    a = {};
    b = 12313;
    callPlusAssign_plusWord(a, b, 10);
    cout << a << endl;
    assert ( (a == vector<int>{3, 1, 3, 2, 1, }) )  ;

}



void testDivideAndRemainderSchool()
{
    vector<int> a = {1, 9, 9,};
    int b = 3;
    vector<int>::iterator aend;
    int remainder;
    tie(aend, remainder) = divideAndRemainderSchool(a.begin(), a.end(), b, 10);
    cout << makeIteratorRange(a.begin(), aend) << endl;
    cout << "remainder: " << remainder << endl;
    assert(isEqual(a.begin(), aend, vector<int>{0, 3, 3} )  );
    assert(remainder == 1);



    a = {4, 6, 7, 2, 6,};
    b = 22222;
    tie(aend, remainder) = divideAndRemainderSchool(a.begin(), a.end(), b, 10);
    cout << makeIteratorRange(a.begin(), aend) << endl;
    cout << "remainder: " << remainder << endl;
    assert(isEqual(a.begin(), aend, vector<int>{2} )  );
    assert(remainder == 18320);



    a = {4, 6, 7, };
    b = 987;
    tie(aend, remainder) = divideAndRemainderSchool(a.begin(), a.end(), b, 10);
    cout << makeIteratorRange(a.begin(), aend) << endl;
    cout << "remainder: " << remainder << endl;
    assert(isEqual(a.begin(), aend, {} )  );
    assert(remainder == 764);



    a = {4, 6, 7, };
    b = 764;
    tie(aend, remainder) = divideAndRemainderSchool(a.begin(), a.end(), b, 10);
    cout << makeIteratorRange(a.begin(), aend) << endl;
    cout << "remainder: " << remainder << endl;
    assert(isEqual(a.begin(), aend, {1} )  );
    assert(remainder == 0);



    a = {};
    b = 764;
    tie(aend, remainder) = divideAndRemainderSchool(a.begin(), a.end(), b, 10);
    cout << makeIteratorRange(a.begin(), aend) << endl;
    cout << "remainder: " << remainder << endl;
    assert(isEqual(a.begin(), aend, {} )  );
    assert(remainder == 0);



}

void testRadixTransform()
{
    vector<unsigned> a = {1, 9, 9, 9, 9, 9, 9, 8, 1, 3, 3, 3, 4,4, 6, 7, };
    vector<unsigned> b(100, 0);
    vector<unsigned>::iterator bend;
    bend = radixTransform(a.begin(), a.end(), 10ULL,  b.begin(), 1ULL << 32);
    cout << makeIteratorRange(b.begin(), bend) << endl;
    assert(isEqual(b.begin(), bend, vector<unsigned>{72723831,1779835, } )  );

    a = {};
    bend = radixTransform(a.begin(), a.end(), 10ULL,  b.begin(), 1ULL << 32);
    cout << makeIteratorRange(b.begin(), bend) << endl;
    assert(isEqual(b.begin(), bend, vector<unsigned>{} )  );


}


//template<typename T, typename ull >
//void out(const vector<T>& _a, ull radix, ull newRadix)
//{
//    vector<T> a = _a;
//    vector<T> out;
//    radixTransform(a.begin(), a.end(), radix, std::back_inserter(out), newRadix);
//    cout << out << endl;;
//}
//
//template<typename RandomIterator, typename ull>
//void out(RandomIterator first, RandomIterator last, ull radix, ull newRadix)
//{
//    vector<typename iterator_traits<RandomIterator>::value_type> a(first, last);
//    out(a, radix, newRadix);
//}

void testDivideAndRemainderKnuthNormalized()
{
    vector<unsigned> a, b, q;
    vector<unsigned>::iterator aLast;
    vector<unsigned>::reverse_iterator qRlast;

    a = {0, 0, 0, 9, 9, 5, 1};
    b = {9, 9, 5};
    q.assign(calculateQuotientLength(a.begin(), a.end(), b.begin(), b.end()), -2);
    tie(qRlast, aLast) = divideAndRemainderKnuthNormalized(a.begin(), a.end(), b.begin(), b.end(), q.rbegin(), 10);
    cout << "q: " << q << endl;
    cout << "remainder: " << makeIteratorRange(a.begin(), aLast) << endl;
    assert((q == vector<unsigned>{9, 6, 6, 2}));
    assert(isEqual(a.begin(), aLast, {9, 6, 2})) ;




    cout << string(100, '-') << endl;
    a = {123u, 234u, 234u, 12313u, 427567u, 112313u, };
    b = {4234u, 123u, 234u, 0xFFF678FFu};
    cout << "A: " << a << endl;
    cout << "B: " << b << endl;
    cout << RadixTransformForOuput<unsigned>(a, (1ULL << 32), 10ULL) << endl;
    cout << RadixTransformForOuput<unsigned>(b, (1ULL << 32), 10ULL) << endl;
//    out(a, (1ULL << 32), 10ULL);
//    out(b, (1ULL << 32), 10ULL);
    q.assign(calculateQuotientLength(a.begin(), a.end(), b.begin(), b.end()), -2);
    tie(qRlast, aLast) = divideAndRemainderKnuthNormalized(a.begin(), a.end(), b.begin(), b.end(), q.rbegin(), (1ULL << 32) );
    cout << "q: " << q << endl;
    cout << "remainder: " << makeIteratorRange(a.begin(), aLast) << endl;

    cout << RadixTransformForOuput<unsigned>(q, (1ULL << 32), 10ULL) << endl;
    cout << RadixTransformForOuput<unsigned>(a.begin(), aLast, (1ULL << 32), 10ULL) << endl;

//    out(q, (1ULL << 32), 10ULL);
//    out(a.begin(), aLast, (1ULL << 32), 10ULL);
    assert((q == vector<unsigned>{1417699595U,112329U}  ));
    assert(isEqual(a.begin(), aLast, {1824194701U,1241006801U,3251927584U,1870613020U})) ;





}



int main()
{
    testPlusAssign();
    testMinusAssign();
    testMultiplySchool();


    testMultiplyAssignSchool_multiplyWord();
    testPlusAssign_plusWord();
    testDivideAndRemainderSchool();
    testRadixTransform();

    testDivideAndRemainderKnuthNormalized();


	return 0;
}








