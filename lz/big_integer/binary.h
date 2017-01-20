/*
 * binary_core.h
 *
 *  Created on: 2017年1月19日
 *      Author: LZ
 */

#ifndef LZ_BIG_INTEGER_BINARY_H_
#define LZ_BIG_INTEGER_BINARY_H_


/*
 *
这个头文件主要针对进制为radix = 2**k的大整数的操作，
主要是位操作
 */

namespace lz { namespace BigIntegerPrivate {


/*

BitOperator 可以是^, |, &
out所指向的空间大小必须为max(aSize, bSize)

可以有out == aFirst == bFirst相等


 */
template<typename RandomIterator1, typename RandomIterator2, typename OutputIterator, typename BitOperator>
void bitOperate(RandomIterator1 aFirst, RandomIterator1 aLast,
        RandomIterator2 bFirst, RandomIterator2 bLast,
        OutputIterator out, BitOperator bitOperator)
{
    using diff_t = typename std::iterator_traits<RandomIterator1>::difference_type;
    diff_t aSize = aLast - aFirst, bSize = bLast - bFirst;
    for(diff_t i = 0; i < std::max(aSize, bSize); ++ i)
    {
        *out++ = bitOperator(aFirst[i], bFirst[i]);
    }
}


/*

计算 [aFirst, aLast) 的位运算非
可以有out == aFirst相等

 */
template<typename RandomIterator, typename OutputIterator, typename BitOperator>
void bitNot(RandomIterator aFirst, RandomIterator aLast,
        OutputIterator out, BitOperator bitOperator)
{
    using diff_t = typename std::iterator_traits<RandomIterator>::difference_type;
    diff_t aSize = aLast - aFirst;
    for(diff_t i = 0; i < aSize; ++ i)
    {
        *out++ = ~aFirst[i];
    }
}


template<typename Int>
Int bitLength(Int x)
{
    Int ans = 0;
    while(x > 0) x >>= 1, ans ++;
    return ans;
}

template<typename RandomIterator, typename ull>
ull bitLength(RandomIterator a, RandomIterator aLast, ull log2Radix)
{
    if(a == aLast) return 0;
    using diff_t = typename std::iterator_traits<RandomIterator>::difference_type;
    diff_t n = aLast - a;
    return (n - 1) * log2Radix + bitLength(*--aLast);
}


template<typename RandomIterator, typename ull>
void shiftHigh_notModifyLength(RandomIterator first, RandomIterator last, ull b, ull log2Radix)
{
    using diff_t = typename std::iterator_traits<RandomIterator>::difference_type;
    auto n = last - first;
    for(diff_t i = n - 1; i >= 1; -- i)
    {
        first[i] = (first[i] << b) | (first[i - 1] >> (log2Radix - b));
    }
    first[0] <<= b;
}


template<typename RandomIterator, typename ull>
void shiftLow_notModifyLength(RandomIterator first, RandomIterator last, ull b, ull log2Radix)
{
    using diff_t = typename std::iterator_traits<RandomIterator>::difference_type;
    auto n = last - first;
    for(diff_t i = 0; i < n - 1; ++ i)
    {
        first[i] = (first[i] >> b) | (first[i + 1] << (log2Radix - b));
    }
    first[n - 1] >>= b;
}


/*

[a, aLast) << b

b表示移动b个bit位
log2Radix表示此大整数的进制为2**log2Radix
 */
template<typename RandomIterator, typename ull>
RandomIterator shiftHigh(RandomIterator first, RandomIterator last, ull b, ull log2Radix)
{
    using diff_t = typename std::iterator_traits<RandomIterator>::difference_type;
    diff_t n = last - first;

    ull topWordBits = bitLength(first[n - 1]);
    ull totalBits = topWordBits + log2Radix * (n - 1);
    ull newTotalBits = totalBits + b;
    diff_t newN = newTotalBits / log2Radix + bool(newTotalBits % log2Radix);
    ull newTopWordBits = newTotalBits % log2Radix;


    std::copy_backward(first, last, first + newN);
    std::fill(first, first + newN - n, 0);
    if(newTopWordBits >= topWordBits)
    {
        auto diffBits = newTopWordBits - topWordBits;
        shiftHigh_notModifyLength(first + newN - n, first + newN, diffBits, log2Radix);
    }
    else
    {
        auto diffBits = topWordBits - newTopWordBits;
        first[newN - n - 1] = first[newN - n] << (log2Radix - diffBits);
        shiftLow_notModifyLength(first + newN - n, first + newN, diffBits, log2Radix);
    }

    return first + newN;
}






/*

[a, aLast) >> b

b表示移动b个bit位
log2Radix表示此大整数的进制为2**log2Radix
 */
template<typename RandomIterator, typename ull>
RandomIterator shiftLow(RandomIterator first, RandomIterator last, ull b, ull log2Radix)
{
    using diff_t = typename std::iterator_traits<RandomIterator>::difference_type;
    diff_t n = last - first;

    ull topWordBits = bitLength(first[n - 1]);
    ull totalBits = topWordBits + log2Radix * (n - 1);

    ull newTotalBits = totalBits - b;
    diff_t newN = newTotalBits / log2Radix + bool(newTotalBits % log2Radix);
    ull newTopWordBits = newTotalBits % log2Radix;



    std::copy(last - newN, last, first);
    if(newTopWordBits >= topWordBits)
    {
        auto diffBits = newTopWordBits - topWordBits;
        shiftHigh_notModifyLength(first, first + newN, diffBits, log2Radix);
    }
    else
    {
        auto diffBits = topWordBits - newTopWordBits;
        shiftLow_notModifyLength(first, first + newN, diffBits, log2Radix);
    }

    return first + newN;
}






//
//template<typename RandomIterator1, typename RandomIterator2, typename OutputIterator, typename ull>
//void bitAnd(RandomIterator1 aFirst, RandomIterator1 aLast,
//        RandomIterator2 bFirst, RandomIterator2 bLast,
//        OutputIterator out, ull log2Radix)
//{
//    using diff_t = typename std::iterator_traits<RandomIterator1>::difference_type;
//    diff_t aSize = aLast - aFirst, bSize = bLast - bFirst;
//    for(diff_t i = 0; i < std::max(aSize, bSize); ++ i)
//    {
//        *out++ = aFirst[i] & bFirst[i];
//    }
//}







}} // namespace lz::BigIntegerPrivate





#endif /* LZ_BIG_INTEGER_BINARY_H_ */
