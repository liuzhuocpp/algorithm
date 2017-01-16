

#include <bits/stdc++.h>
#include <lz/big_integer/core.h>
#include <lz/std_utility.h>

using namespace lz;
using namespace std;


void testPlusAssign()
{
    vector<int> a = {9, 9, 9, 0, 0, 0, 0, 0 ,0, 0};

    vector<int> b = {9, 9, 9, 9, 9, 9};

    auto aend = BigIntegerPrivate::plusAssign(a.begin(), a.begin() + 3, b.begin(), b.end(), 10);

    out(a.begin(), aend);

}
void testMinusAssign()
{
    vector<int> a = {0, 0, 0, 9, 9, 9, 0, 0 ,0, 0};

    vector<int> b = {0, 9, 9, };

    auto aend = BigIntegerPrivate::minusAssign(a.begin(), a.begin() + 6, b.begin(), b.end(), 10);

    out(a.begin(), aend);

}
void testMultiplySchool()
{
    vector<int> a = {9, 9, 9,};
    vector<int> b = {0, 9, 9,};
    vector<int> c(10, 0);
    auto cend = BigIntegerPrivate::multiplySchool(a.begin(), a.end(), b.begin(), b.end(), c.begin(), 10);
    out(c.begin(), cend);
}
void testMultiplyAssignSchool_multiplyWord()
{
    vector<int> a = {1, 9, 9,};
    int b = 42;

    auto aend = BigIntegerPrivate::multiplyAssignSchool(a.begin(), a.end(), b,10);
    out(a.begin(), aend);
}

void testPlusAssign_plusWord()
{
    vector<int> a = {1, 9, 9,};
    int b = 42999;

    auto aend = BigIntegerPrivate::plusAssign(a.begin(), a.end(), b,10);
    out(a.begin(), aend);
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

}

void testRadixTransform()
{
    vector<unsigned> a = {1, 9, 9, 9, 9, 9, 9, 8, 1, 3, 3, 3, 4,4, 6, 7, };

    vector<unsigned> b(100, 0);
    vector<unsigned>::iterator bend;

//    bend = BigIntegerPrivate::radixTransform(a.begin(), a.end(), 10ULL,  b.begin(), 10000);

    bend = BigIntegerPrivate::radixTransform(a.begin(), a.end(), 10ULL,  b.begin(), 1ULL << 32);

    cout << (1ULL << 32) << endl;

    out(b.begin(), bend);
//    cout << endl;
//    cout << "remainder: " << remainder << endl;

}


int main()
{

    testRadixTransform();


	return 0;
}








