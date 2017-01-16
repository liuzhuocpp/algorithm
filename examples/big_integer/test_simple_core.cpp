

#include <bits/stdc++.h>
#include <lz/big_integer/core.h>
#include <lz/std_utility.h>

using namespace lz;
using namespace std;

template<typename InputIterator1, typename InputIterator2>
bool isEqual(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
{
    while(first1 != last1)
    {
        if(*first1 != *first2) return 0;

        first1 ++;
        first2 ++;
    }
    return 1;
}

template<typename InputIterator>
bool isEqual(InputIterator first1, InputIterator last1,
    const vector<typename std::iterator_traits<InputIterator>::value_type>& b)
{
    return isEqual(first1, last1, b.begin());
}


void testPlusAssign()
{
    vector<int> a = {9, 9, 9, 0, 0, 0, 0, 0 ,0, 0};

    vector<int> b = {9, 9, 9, 9, 9, 9};

    auto aend = BigIntegerPrivate::plusAssign(a.begin(), a.begin() + 3, b.begin(), b.end(), 10);

    out(a.begin(), aend);
    cout << endl;

    assert(isEqual(a.begin(), aend, vector<int>{8, 9, 9, 0, 0, 0, 1} )  );


}
void testMinusAssign()
{
    vector<int> a = {0, 0, 0, 9, 9, 9, 0, 0 ,0, 0};

    vector<int> b = {0, 9, 9, };

    auto aend = BigIntegerPrivate::minusAssign(a.begin(), a.begin() + 6, b.begin(), b.end(), 10);

    out(a.begin(), aend);
    cout << endl;
    assert(isEqual(a.begin(), aend, vector<int>{0, 1, 0,8, 9, 9} )  );

}
void testMultiplySchool()
{
    vector<int> a = {9, 9, 9,};
    vector<int> b = {0, 9, 9,};
    vector<int> c(10, 0);
    auto cend = BigIntegerPrivate::multiplySchool(a.begin(), a.end(), b.begin(), b.end(), c.begin(), 10);
    out(c.begin(), cend);
    cout << endl;
    assert(isEqual(c.begin(), cend, vector<int>{0, 1, 0,9, 8, 9} )  );
}
void testMultiplyAssignSchool_multiplyWord()
{
    vector<int> a = {1, 9, 9,};
    int b = 42;

    auto aend = BigIntegerPrivate::multiplyAssignSchool(a.begin(), a.end(), b,10);
    out(a.begin(), aend);
    cout << endl;
    assert(isEqual(a.begin(), aend, vector<int>{2, 2, 6, 1, 4} )  );
}

void testPlusAssign_plusWord()
{
    vector<int> a = {1, 9, 9,};
    int b = 42999;

    auto aend = BigIntegerPrivate::plusAssign(a.begin(), a.end(), b,10);
    out(a.begin(), aend);
    assert(isEqual(a.begin(), aend, vector<int>{0, 9, 9, 3, 4} )  );
}



void testDivideAndRemainderSchool()
{
    vector<int> a = {1, 9, 9,};
    int b = 3;
    vector<int>::iterator aend;
    int remainder;
    tie(aend, remainder) = BigIntegerPrivate::divideAndRemainderSchool(a.begin(), a.end(), b, 10);

    out(a.begin(), aend);
    cout << endl;
    cout << "remainder: " << remainder << endl;
    assert(isEqual(a.begin(), aend, vector<int>{0, 3, 3} )  );
    assert(remainder == 1);


}

void testRadixTransform()
{
    vector<unsigned> a = {1, 9, 9, 9, 9, 9, 9, 8, 1, 3, 3, 3, 4,4, 6, 7, };

    vector<unsigned> b(100, 0);
    vector<unsigned>::iterator bend;

//    bend = BigIntegerPrivate::radixTransform(a.begin(), a.end(), 10ULL,  b.begin(), 10000);

    bend = BigIntegerPrivate::radixTransform(a.begin(), a.end(), 10ULL,  b.begin(), 1ULL << 32);

//    cout << (1ULL << 32) << endl;

    out(b.begin(), bend);
    assert(isEqual(b.begin(), bend, vector<unsigned>{72723831,1779835, } )  );

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


	return 0;
}








