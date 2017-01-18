#include <bits/stdc++.h>

#include <lz/debug.h>

#include <lz/big_integer/vector_core.h>


using namespace lz;
using namespace std;
using namespace lz::BigIntegerPrivate;

void testDivideAndRemainderKnuth()
{
    std::vector<unsigned> a, b, q;

    a = {3, 5, 6, 6};
    b = {3, 5, };
    cout << "A: " << a << endl;
    cout << "B: " << b << endl;
    q = divideAndRemainderKnuth(a, b, 10U);
    cout << "q: " << q << endl;
    cout << "remainder: " << a << endl;
    assert((q == vector<unsigned>{5, 2, 1}));
    assert((a == vector<unsigned>{8, 2}));


    a = {12313U, 1313U, 123U, 4523453U, 0xF23FFFFU};
    b = {132U, 24U, 452345U, 0x1EFEBCADU };
    cout << "a = " << RadixTransformForOuput<unsigned>(a, (1ULL << 32), 10ULL) << endl;
    cout << "b = " << RadixTransformForOuput<unsigned>(b, (1ULL << 32), 10ULL) << endl;
    q = divideAndRemainderKnuth(a, b, (1ULL << 32));
    cout << "q = " << RadixTransformForOuput<unsigned>(q, (1ULL << 32), 10ULL) << endl;
    cout << "r = " << RadixTransformForOuput<unsigned>(a, (1ULL << 32), 10ULL) << endl;

    cout << q << endl << a << endl;
    assert((q == vector<unsigned>{2098027085}));
    assert((a == vector<unsigned>{2233311333,1186958760,1091829130,293934224}));






}
int main()
{

    testDivideAndRemainderKnuth();



	return 0;
}
