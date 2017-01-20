#include <bits/stdc++.h>
#include <lz/big_integer/basic.h>
#include <lz/big_integer/binary.h>

#include <lz/debug.h>

#include <lz/iterator_range.h>

using namespace lz;
using namespace std;
using namespace lz::BigIntegerPrivate;


template<typename RandomIterator, typename ull >
bool validShiftHigh(RandomIterator first, RandomIterator last, ull b, ull log2Radix)
{
    using uint = typename std::iterator_traits<RandomIterator>::value_type;
    auto n = last - first;
    vector<uint> a(first, last);
    a.resize(a.size() + b);

    auto aend = shiftHigh(a.begin(), a.begin() + n, b, log2Radix);


    const vector<uint> oldRange = RadixTransformForOuput<uint>(first, last, (1ULL << log2Radix), 2ULL).out;
    const vector<uint> newRange = RadixTransformForOuput<uint>(a.begin(), aend, (1ULL << log2Radix), 2ULL).out;

    auto oldSize = oldRange.size();
    if(oldRange.size() + b != newRange.size()) return false;

    if(!isEqual(oldRange.begin(), oldRange.end(), newRange.end() - oldSize, newRange.end()) ) return false;
    if(!all_of(newRange.begin(), newRange.begin() + b, [](uint x) { return x == 0; })) return false;
    return true;
}
void testShiftHigh()
{
    vector<unsigned> a;
    vector<unsigned>::iterator aend;
    unsigned long long radix, an;


    a = {234, 2, 123, 3456, 54635636, 45646};
    bool isValid = validShiftHigh(a.begin(), a.end(), 8913ULL, 32ULL);
    cout << isValid << endl;
    assert(isValid);

}











template<typename RandomIterator, typename ull >
bool validShiftLow(RandomIterator first, RandomIterator last, ull b, ull log2Radix)
{
    using uint = typename std::iterator_traits<RandomIterator>::value_type;
    auto n = last - first;
    vector<uint> a(first, last);

    auto aend = shiftLow(a.begin(), a.begin() + n, b, log2Radix);


    const vector<uint> oldRange = RadixTransformForOuput<uint>(first, last, (1ULL << log2Radix), 2ULL).out;
    const vector<uint> newRange = RadixTransformForOuput<uint>(a.begin(), aend, (1ULL << log2Radix), 2ULL).out;

    if(oldRange.size() - b != newRange.size()) return false;

    if(!isEqual(oldRange.end() - newRange.size(), oldRange.end(), newRange.begin(), newRange.end()) ) return false;
    return true;
}
void testShiftLow()
{
    OUT_FUNCTION_NAME

    vector<unsigned> a;
    vector<unsigned>::iterator aend;
    unsigned long long radix, an;


    a = {234, 2, 123, 3456, 54635636, 45646};
    bool isValid = validShiftLow(a.begin(), a.end(), 23ULL, 32ULL);
    cout << isValid << endl;
    assert(isValid);



}

int main()
{

    testShiftHigh();
    testShiftLow();



	return 0;
}
