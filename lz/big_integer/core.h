/*
 * core.h
 *
 *  Created on: 2017年1月16日
 *      Author: LZ
 */

#ifndef LZ_BIG_INTEGER_CORE_H_
#define LZ_BIG_INTEGER_CORE_H_



namespace lz { namespace BigIntegerPrivate {



/**
 * UintSeq represent the unsigned BigInteger.
 * The length of a word sequence is at least 1(specially for zero),
 * and no more than INT_MAX.
 * A UintSeq do not contain leading zeros.
 * Use little endian to storage the word sequence.
 * a[0], a[1], a[2]... radix bits increase
 *        -->
 */


/**
 * This type is used to hold a word
 */
using uint32_t = std::uint_fast32_t;

/**
 * This type is used to hold two words
 */
using uint64_t = std::uint_fast64_t;

/**
 * This type is used to hold word sequence
 */
using WordVector = std::vector<uint32_t>;

/**
 * For index loop
 */
using diff_t = WordVector::difference_type;


template<typename RandomIterator>
RandomIterator removeLeadingZeros(RandomIterator first, RandomIterator last)
{
    while(last - first > 1 && *(last - 1) == 0)
        last --;
    return last;
}


/**
 * Plus the contents of the WordVector a and b. And put the result into a, namely a += b.
 * a , b should not have leading zeors.
 * If x is the max value of uint, the ull type must can hold x * x + x
 *
 * @param  a an add value.
 * @param  b value to be added to a.
 */
template<typename RandomIterator, typename ull>
RandomIterator plusAssign(RandomIterator afirst, RandomIterator aend, RandomIterator bfirst, RandomIterator bend, ull radix)
{
    using uint = typename std::iterator_traits<RandomIterator>::value_type;
    using diff_t = typename std::iterator_traits<RandomIterator>::difference_type;
    diff_t aSize = aend - afirst, bSize = bend - bfirst;
    diff_t minSize = std::min(aSize, bSize);
    ull t = 0;
    RandomIterator ansEnd = aend;
    for(diff_t i = 0; i < minSize; ++ i)
    {
        t += afirst[i];
        t += bfirst[i];
        afirst[i] = t % radix;
        t /= radix;
    }
    if(aSize < bSize)
    {
        for(diff_t i = aSize; i < bSize; ++ i)
        {
            t += bfirst[i];
            *aend++ = t % radix;
            t /= radix;
        }
    }
    else
    {
        for(diff_t i = bSize; t > 0 && i < aSize; ++ i)
        {
            t += afirst[i];
            afirst[i] = t % radix;
            t /= radix;
        }
    }
    while(t > 0) *aend++ = t % radix, t /= radix;
    return aend;
}


/**
 * Minus the UintSeq b from a, and put the result into a, namely a -= b.
 * @param  a value to be minuend by b.
 * @param  b the minus value.
 */
template<typename RandomIterator, typename ull>
RandomIterator minusAssign(RandomIterator afirst, RandomIterator aend, RandomIterator bfirst, RandomIterator bend, ull radix)
{
    using uint = typename std::iterator_traits<RandomIterator>::value_type;
    using diff_t = typename std::iterator_traits<RandomIterator>::difference_type;
    diff_t bsize = bend - bfirst, asize = aend - afirst;
    ull t = 0;
    for(diff_t i = 0; i < bsize; ++ i)
    {
        t += bfirst[i];
        if(afirst[i] < t)
            afirst[i] = radix + afirst[i] - t, t = 1;
        else
            afirst[i] -= t, t = 0;
    }
    for(diff_t i = bsize; t > 0 && i < asize; ++ i)
    {
        if(afirst[i] < t)
            afirst[i] = radix + afirst[i] - t, t = 1;
        else
            afirst[i] -= t, t = 0;
    }
    return removeLeadingZeros(afirst, aend);
}


/**
 * Multiply the contents of the UintSeq a and b using school method..
 * c must be all zeros.
 * And put the result into c, namely c = a * b.
 * @param  a the multiply value.
 * @param  b value to be multiplied to a.
 * @return  c the result will be stored.
 */
template<typename RandomIterator, typename ull>
RandomIterator multiplySchool(RandomIterator afirst, RandomIterator aend, RandomIterator bfirst, RandomIterator bend,
        RandomIterator cfirst, ull radix)
{
    using uint = typename std::iterator_traits<RandomIterator>::value_type;
    using diff_t = typename std::iterator_traits<RandomIterator>::difference_type;
    diff_t asize = aend - afirst, bsize = bend - bfirst;

    RandomIterator cend = cfirst + asize + bsize;

    for(diff_t i = 0; i < bsize; ++ i)
    {
        ull t = 0;
        for(diff_t j = 0; j < asize; ++ j)
        {
            t += ull(bfirst[i]) * afirst[j] + cfirst[i + j];
            cfirst[i + j] = t % radix;
            t /= radix;
        }
        if(t > 0)
        {
            cfirst[i + asize] = t;
        }
    }
    return removeLeadingZeros(cfirst, cend);
}


/**
 * Multiply the contents of the UintSeq a and uint b using school method.
 * And put the result into a, namely a *= b.
 * @param  a the multiply value.
 * @param  b value to be multiplied to a.
 */
template<typename RandomIterator, typename ull>
RandomIterator multiplyAssignSchool(RandomIterator afirst, RandomIterator alast,
    typename std::iterator_traits<RandomIterator>::value_type b, ull radix)
{
    using uint = typename std::iterator_traits<RandomIterator>::value_type;
    using diff_t = typename std::iterator_traits<RandomIterator>::difference_type;
    ull t = 0;
    for(diff_t i = 0; i < alast - afirst; ++ i)
    {
        t += ull(afirst[i]) * b;
        afirst[i] = t % radix;
        t /= radix;
    }
    while(t > 0) *alast ++ = t % radix, t /= radix;
    return alast;
}


/**
 * Adds the contents of the UintSeq a and uint b.
 * And put the result into a, namely a += b.
 * @param  a add value.
 * @param  b value to be added to a.
 */
template<typename RandomIterator, typename ull>
RandomIterator plusAssign(RandomIterator afirst, RandomIterator alast,
    typename std::iterator_traits<RandomIterator>::value_type b, ull radix)
{
    ull t = b;
    for(int i = 0; i < alast - afirst; ++ i)
    {
        if(t == 0) break;
        t += afirst[i];
        afirst[i] = t % radix;
        t /= radix;
    }
    while(t > 0) *alast ++ = t % radix, t /= radix;
    return alast;
}



/**
 * Divide uint b from UintSeq a using school method.
 * And put the result into a, namely a /= b.
 * @param  a the divided value by b.
 * @param  b the divide value.
 * @return the remainder a % b.
 */
template<typename RandomIterator, typename buint, typename ull>
std::pair<RandomIterator, ull> divideAndRemainderSchool(RandomIterator afirst, RandomIterator alast,
    buint b, ull radix)
{
//    std::cout << "divideAndRemainderSchool "  << radix << " " << b << std::endl;
    ull t = 0;
    using uint = typename std::iterator_traits<RandomIterator>::value_type;
    using diff_t = typename std::iterator_traits<RandomIterator>::difference_type;
    diff_t asize = alast - afirst;
    for(diff_t i = asize - 1; i >= 0; -- i)
    {
        t = t * radix + afirst[i];
        afirst[i] = t / b;
        t %= b;
    }
    return std::make_pair(removeLeadingZeros(afirst, alast), t);
}


template<typename RandomIteratorA, typename aull, typename RandomIteratorB, typename bull>
RandomIteratorB radixTransform(RandomIteratorA afirst, RandomIteratorA alast, aull aradix,
        RandomIteratorB bfirst, bull bradix )
{
    using buint = typename std::iterator_traits<RandomIteratorB>::value_type;
    while(alast - afirst > 1 || *afirst > 0)
    {

        buint remainder;
        tie(alast, remainder) = divideAndRemainderSchool(afirst, alast, bradix, aradix);
//        std::cout << "remainder"  << remainder << std::endl;
//        out(afirst, alast);
//        std::cout << std::endl;

        *bfirst++ = remainder;
    }
    return bfirst;
}

/**
 * Compare the UintSeq a to b.
 * @param  a first UintSeq used to compare.
 * @param  b second UintSeq used to compare.
 * @return  if a > b: return 1;
 *          if a == b: return 0;
 *          if a < b: return -1.
 */
template<typename RandomIterator>
int compare(RandomIterator afirst, RandomIterator alast,
    RandomIterator bfirst, RandomIterator blast)
{
    using diff_t = typename std::iterator_traits<RandomIterator>::difference_type;
    diff_t asize = alast - afirst, bsize = blast - bfirst;

    if(asize < bsize) return -1;
    if(asize > bsize) return 1;
    for(int i = asize - 1; i >= 0; -- i)
    {
        if(afirst[i] < bfirst[i]) return -1;
        if(afirst[i] > bfirst[i]) return 1;
    }
    return 0;
}

//
//
///**
// * Transform the string into a UintSeq and the string should be big endian
// * @param s the first iterator of string
// * @param end the end iterator of string
// * @param radix the radix of string
// * @param a the result of transformed uint sequence
// */
//template<uint64_t StoreRadix, typename Iterator>
//WordVector fromString(Iterator begin, Iterator end, uint inputRadix)
//{
//    WordVector a;
//
////    a.clear();
////    int n = end - s;
////    uint max_radix, l;
////    getMaxRadix(radix, l, max_radix);
////
////    int begin_pos = n % l;
////    uint x = 0;
////    for(int i = 0; i < begin_pos; ++ i)
////    {
////        x = x * radix + fromChar(s[i]);
////    }
////    a.push_back(x);
////    for(int i = begin_pos; i < n; i += l)
////    {
////        uint x = 0;
////        for(int j = i; j < i + l; ++ j)
////        {
////            x = x * radix + fromChar(s[j]);
////        }
////        multiplySchool(a, max_radix);
////        plus(a, x);
////    }
////    removeLeadingZeros(a);
//
//
//
//
//
//
//}
//






































}} // namespace lz::BigIntegerPrivate





#endif /* LZ_BIG_INTEGER_CORE_H_ */
