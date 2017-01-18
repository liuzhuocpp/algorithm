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

Use iterator range represent a unsigned BigInteger.
数字的进制权重按照iterator range 递增的顺序递增
即对于iterator range [a, b), 进制为radix，  表示的数字是
a[0] * radix^0 + a[1] * radix^1 + a[2] * radix ^2 + ... + a[b - a - 1] * radix ^(b - a - 1)
（其中radix^x 这里表示radix的x次幂）
同时仅仅当 长度为0的iterator range 表示大整数0
不能有(b - a == 1 && a[0] == 0) 这种情况

 */





template<typename RandomIterator>
RandomIterator removeLeadingZeros(RandomIterator first, RandomIterator last)
{
    while(last - first > 0 && *(last - 1) == 0)
        last --;
    return last;
}


/**

Plus the contents of the WordVector a and b. And put the result into a, namely a += b.
a , b should not have leading zeors.
If x is the max value of uint, the ull type must can hold x * x + x

不能有前导0， 数字0使用长度为0的range表示
a += b结果放置在a中
 * @param  a an add value.
 * @param  b value to be added to a.
 * @return 返回值表示 最终是否进位，其余的相加结果存放在[afirst, afirst + max(aSize, bSize) )
 *
 */
template<typename RandomIterator1, typename RandomIterator2, typename ull>
bool
plusAssign(RandomIterator1 aFirst, RandomIterator1 aLast, RandomIterator2 bFirst, RandomIterator2 bLast, ull radix)
{
    using uint = typename std::iterator_traits<RandomIterator1>::value_type;
    using diff_t = typename std::iterator_traits<RandomIterator1>::difference_type;
    diff_t aSize = aLast - aFirst, bSize = bLast - bFirst;
    diff_t minSize = std::min(aSize, bSize);
    ull t = 0;
    for(diff_t i = 0; i < minSize; ++ i)
    {
        t += aFirst[i];
        t += bFirst[i];
        aFirst[i] = t % radix;
        t /= radix;
    }
    if(aSize < bSize)
    {
        for(diff_t i = aSize; i < bSize; ++ i)
        {
            t += bFirst[i];
            *aLast++ = t % radix;
            t /= radix;
        }
    }
    else
    {
        for(diff_t i = bSize; t > 0 && i < aSize; ++ i)
        {
            t += aFirst[i];
            aFirst[i] = t % radix;
            t /= radix;
        }
    }
    return t;
//    while(t > 0) *aend++ = t % radix, t /= radix;
//    return aend;
}


/**
 * Minus the UintSeq b from a, and put the result into a, namely a -= b.
 * @param  a value to be minuend by b.
 * @param  b the minus value.
 */
template<typename RandomIterator1, typename RandomIterator2, typename ull>
RandomIterator1 minusAssign(RandomIterator1 aFirst, RandomIterator1 aLast,
    RandomIterator2 bFirst, RandomIterator2 bLast, ull radix)
{
    using uint = typename std::iterator_traits<RandomIterator1>::value_type;
    using diff_t = typename std::iterator_traits<RandomIterator1>::difference_type;
    diff_t bSize = bLast - bFirst, aSize = aLast - aFirst;
    ull t = 0;
    for(diff_t i = 0; i < bSize; ++ i)
    {
        t += bFirst[i];
        if(aFirst[i] < t)
            aFirst[i] = radix + aFirst[i] - t, t = 1;
        else
            aFirst[i] -= t, t = 0;
    }
    for(diff_t i = bSize; t > 0 && i < aSize; ++ i)
    {
        if(aFirst[i] < t)
            aFirst[i] = radix + aFirst[i] - t, t = 1;
        else
            aFirst[i] -= t, t = 0;
    }
    return removeLeadingZeros(aFirst, aLast);
}


/**
 * Multiply the contents of the UintSeq a and b using school method..
 * c must be all zeros.
 * And put the result into c, namely c = a * b.
 * @param  a the multiply value.
 * @param  b value to be multiplied to a.
 * @return  c the result will be stored.


a 和b所指向数据不能有重叠
assert(cSize >= aSize + bSize)
c的起始值不必全是0
 */
template<typename RandomIterator1, typename RandomIterator2, typename RandomIterator3, typename ull>
RandomIterator3 multiplySchool(RandomIterator1 aFirst, RandomIterator1 aLast,
        RandomIterator2 bFirst, RandomIterator2 bLast,
        RandomIterator3 cFirst, ull radix)
{
    using uint = typename std::iterator_traits<RandomIterator1>::value_type;
    using diff_t = typename std::iterator_traits<RandomIterator1>::difference_type;
    diff_t aSize = aLast - aFirst, bSize = bLast - bFirst;
    RandomIterator3 cend = cFirst + aSize + bSize;

    for(diff_t j = 0; j < bSize; ++ j)
    {
        ull t = 0;
        for(diff_t i = 0; i < aSize; ++ i)
        {
            t += ull(bFirst[j]) * aFirst[i];
            if(j > 0 && i < aSize)
            {
                t += cFirst[i + j];
            }
            cFirst[i + j] = t % radix;
            t /= radix;
        }
        cFirst[j + aSize] = t;
    }

//    for(diff_t i = 0; i < bSize; ++ i)
//    {
//        ull t = 0;
//        for(diff_t j = 0; j < aSize; ++ j)
//        {
//            t += ull(bFirst[i]) * aFirst[j] + cFirst[i + j];
//            cFirst[i + j] = t % radix;
//            t /= radix;
//        }
//        if(t > 0)
//        {
//            cFirst[i + aSize] = t;
//        }
//    }
    return removeLeadingZeros(cFirst, cend);
}


/**

T 是个正数，将T 转化为radix 进制存入out中
 */

template<typename T, typename OutputIterator, typename RadixType>
OutputIterator integerRadixTransform(T t, OutputIterator out, RadixType radix)
{
    while(t > 0) *out++ = t % radix, t /= radix;
    return out;
}


/**
 * Multiply the contents of the UintSeq a and uint b using school method.
 * And put the result into a, namely a *= b.
 * @param  a the multiply value.
 * @param  b value to be multiplied to a.

precondition:
b > 0;

postcondition:
[aFirst, aLast)内容改变为a*b之后的结果

return:
向最高位的进位值




 */
template<typename RandomIterator, typename ull>
ull multiplyAssignSchool(RandomIterator aFirst, RandomIterator aLast,
    typename std::iterator_traits<RandomIterator>::value_type b, ull radix)
{
    using uint = typename std::iterator_traits<RandomIterator>::value_type;
    using diff_t = typename std::iterator_traits<RandomIterator>::difference_type;
    ull t = 0;
    for(diff_t i = 0; i < aLast - aFirst; ++ i)
    {
        t += ull(aFirst[i]) * b;
        aFirst[i] = t % radix;
        t /= radix;
    }
    return t;
//    while(t > 0) *aLast ++ = t % radix, t /= radix;
//    return aLast;
}


/**
 * Adds the contents of the UintSeq a and uint b.
 * And put the result into a, namely a += b.
 * @param  a add value.
 * @param  b value to be added to a.
 */
template<typename RandomIterator, typename ull>
ull plusAssign(RandomIterator afirst, RandomIterator alast,
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
    return t;
//    while(t > 0) *alast ++ = t % radix, t /= radix;
//    return alast;
}



/**
 * Divide uint b from UintSeq a using school method.
 * And put the result into a, namely a /= b.
 * @param  a the divided value by b.
 * @param  b the divide value.
 * @return the remainder a % b.
 */
template<typename RandomIterator, typename buint, typename ull>
std::pair<RandomIterator, ull> divideAndRemainderSchool(RandomIterator aFirst, RandomIterator aLast,
    buint b, ull radix)
{
    using uint = typename std::iterator_traits<RandomIterator>::value_type;
    using diff_t = typename std::iterator_traits<RandomIterator>::difference_type;
    diff_t asize = aLast - aFirst;
    ull t = 0;
    for(diff_t i = asize - 1; i >= 0; -- i)
    {
        t = t * radix + aFirst[i];
        aFirst[i] = t / b;
        t %= b;
    }
    return std::make_pair(removeLeadingZeros(aFirst, aLast), t);
}


template<typename RandomIterator, typename ull, typename OutputIterator, typename NewRadix>
OutputIterator radixTransform(RandomIterator first, RandomIterator last, ull radix,
        OutputIterator out, NewRadix newRadix )
{
    using uint = typename std::iterator_traits<RandomIterator>::value_type;
    while(last - first > 0)
    {
        uint remainder;
        tie(last, remainder) = divideAndRemainderSchool(first, last, newRadix, radix);
        *out++ = remainder;
    }
    return out;
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
