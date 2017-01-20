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

//    a = {1, 1, 0, 1, 0, 0, 0, 0};
//    aend = shiftHigh(a.begin(), a.begin() + 4, 3, 1);
//    cout << makeIteratorRange(a.begin(), aend) << endl;
//
//
//    a = {234, 4234, 5345, 345, 0, 0, 0, 0};
//    radix = (1ULL << 20);
//    an = 8;
//    cout << "          A: " << RadixTransformForOuput<unsigned>(a.begin(), a.begin() + an, radix, 2ULL) << endl;
//    aend = shiftHigh(a.begin(), a.begin() + an, 3ULL, 20);
//    cout << "new A: " << RadixTransformForOuput<unsigned>(a.begin(), aend, radix, 2ULL) << endl;

//    radix = (1ULL << 10);
//    a = {234, 2, (1U<<10) - 1};
//    cout << validShiftHigh(a.begin(), a.end(), 100ULL, 10ULL) << endl << endl;;
//
//    a = {234, 5, 45,56, 456, 75675, 56};
//    cout << validShiftHigh(a.begin(), a.end(), 1221ULL, 23ULL) << endl << endl;;

    a = {234, 2, 123, 3456, 54635636, 45646};
    cout << validShiftHigh(a.begin(), a.end(), 8913ULL, 32ULL) << endl << endl;;



//    cout << "            A: " << RadixTransformForOuput<unsigned>(a.begin(), a.begin() + an, radix, 2ULL) << endl;
//    aend = shiftHigh(a.begin(), a.begin() + an, 100ULL, 10ULL);
//    cout << "new A: " << RadixTransformForOuput<unsigned>(a.begin(), aend, radix, 2ULL) << endl;




//    cout << makeIteratorRange(a.begin(), aend) << endl;

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
    vector<unsigned> a;
    vector<unsigned>::iterator aend;
    unsigned long long radix, an;


    a = {234, 2, 123, 3456, 54635636, 45646};
    cout << validShiftLow(a.begin(), a.end(), 23ULL, 32ULL) << endl << endl;;



}

int main()
{

    testShiftHigh();
    testShiftLow();



	return 0;
}
