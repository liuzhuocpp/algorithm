/*
 * binary_core.h
 *
 *  Created on: 2017年1月19日
 *      Author: LZ
 */

#ifndef LZ_BIG_INTEGER_BINARY_H_
#define LZ_BIG_INTEGER_BINARY_H_


#include <lz/big_integer/basic.h>


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
template<typename RandomIterator1, typename RandomIterator2, typename RandomIterator3, typename BitOperator>
RandomIterator3 bitOperate(RandomIterator1 aFirst, RandomIterator1 aLast,
        RandomIterator2 bFirst, RandomIterator2 bLast,
        RandomIterator3 out, BitOperator bitOperator)
{
    using diff_t = typename std::iterator_traits<RandomIterator1>::difference_type;
    using uint = typename std::iterator_traits<RandomIterator1>::value_type;
    diff_t aSize = aLast - aFirst, bSize = bLast - bFirst;
    auto outEnd = out;
    for(diff_t i = 0; i < std::max(aSize, bSize); ++ i)
    {
        uint aValue = 0, bValue = 0;
        if(i < aSize) aValue = aFirst[i];
        if(i < bSize) bValue = bFirst[i];
        *outEnd++ = bitOperator(aValue, bValue);
    }

    return removeLeadingZeros(out, outEnd);



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


/*

计算 [aFirst, aLast) 的位运算非
可以有out == aFirst相等

~0 == 0
!!!有待于完善
 */
template<typename RandomIterator1, typename RandomIterator2>
RandomIterator2 bitNot(RandomIterator1 first, RandomIterator1 last,
		RandomIterator2 out)
{
    if(first == last)
    {
    	*out ++ = 1;
    	return out;
    }
    using uint = typename std::iterator_traits<RandomIterator1>::value_type;
    RandomIterator2 outEnd = out;
    for(const auto& x: makeIteratorRange(first, last - 1))
    {
        *outEnd++ = ~x;
    }
    uint lastValue = *(last - 1);
	uint newLastValue = 0;
	uint lastValueBits = bitLength(lastValue);
    for(uint i = 0; i < lastValueBits; ++ i)
    {
    	if(!((lastValue >> i) & 1)) newLastValue |= uint(1) << i;
    }
    *outEnd++ = newLastValue;
    return removeLeadingZeros(out, outEnd);
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
//    	first[i] = (first[i] >> b) | (((ull(1) << b) - 1) );
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
    ull topWordBits;
    ull totalBits;
    if(n == 0) topWordBits = totalBits = 0;
    else
	{
    	topWordBits = bitLength(first[n - 1]),
		totalBits = (topWordBits + log2Radix * (n - 1)) ;
	}
    ull newTotalBits = totalBits + b;
    diff_t newN = newTotalBits / log2Radix + bool(newTotalBits % log2Radix);
    ull newTopWordBits = newTotalBits % log2Radix;
    if(newTopWordBits == 0) newTopWordBits = log2Radix;


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

    ull topWordBits;
    ull totalBits;
    if(n == 0) topWordBits = totalBits = 0;
    else
	{
    	topWordBits = bitLength(first[n - 1]),
		totalBits = (topWordBits + log2Radix * (n - 1)) ;
	}


    if(totalBits <= b)
    {
    	return first;
    }
    ull newTotalBits = totalBits - b;
    diff_t newN = newTotalBits / log2Radix + bool(newTotalBits % log2Radix);
    ull newTopWordBits = newTotalBits % log2Radix;
    if(newTopWordBits == 0) newTopWordBits = log2Radix;

    if(newTopWordBits >= topWordBits)
    {
        auto diffBits = newTopWordBits - topWordBits;
        shiftHigh_notModifyLength(first, last, diffBits, log2Radix);
    }
    else
    {
        auto diffBits = topWordBits - newTopWordBits;
        shiftLow_notModifyLength(first, last, diffBits, log2Radix);
    }
    std::copy(last - newN, last, first);
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
