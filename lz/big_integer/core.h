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

precondition:
aSize >= bSize;
[aFirst, aLast), [bFirst, bLast) 不能有前导0， 数字0使用长度为0的range表示

postcondition:
[aFirst, aLast) += [bFirst, bLast)
结果放置在[aFirst, aLast)中

return:
最终是否进位

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
}


/**

[aFirst, aLast) += b
结果放置在[aFirst, aLast)， 返回值为最高位进位的值
 */
template<typename RandomIterator, typename ull>
ull plusAssign(RandomIterator aFirst, RandomIterator aLast,
    typename std::iterator_traits<RandomIterator>::value_type b, ull radix)
{
    ull t = b;
    for(int i = 0; i < aLast - aFirst; ++ i)
    {
        if(t == 0) break;
        t += aFirst[i];
        aFirst[i] = t % radix;
        t /= radix;
    }
    return t;
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

[cFirst, cLast) = [aFirst, aLast) * [bFirst, bLast)
[cFirst, cLast), [aFirst, aLast) 不应重叠
[cFirst, cLast), [bFirst, bLast) 不应重叠

precondition:
    cSize >= aSize + bSize;
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
            if(j > 0)
            {
                t += cFirst[i + j];
            }
            cFirst[i + j] = t % radix;
            t /= radix;
        }
        cFirst[j + aSize] = t;
    }
    return removeLeadingZeros(cFirst, cend);
}


/**
T 是个正整数，将T 转化为radix 进制存入out中， 从低位开始输出到out中
 */
template<typename T, typename OutputIterator, typename RadixType>
OutputIterator integerRadixTransform(T t, OutputIterator out, RadixType radix)
{
    while(t > 0) *out++ = t % radix, t /= radix;
    return out;
}


/**

[aFirst, aLast) *= b

precondition:
b > 0;
aFirst与 out或者不指向同一块数据，或者aFirst 与out都指向相同的数据的相同的起始地址

postcondition:
[aFirst, aLast)内容改变为a*b之后的结果

return:
向最高位的进位值
 */
template<typename RandomIterator, typename OuputIterator, typename ull>
ull multiplySchool(RandomIterator aFirst, RandomIterator aLast,
    typename std::iterator_traits<RandomIterator>::value_type b, OuputIterator out, ull radix)
{
    using uint = typename std::iterator_traits<RandomIterator>::value_type;
    using diff_t = typename std::iterator_traits<RandomIterator>::difference_type;
    ull t = 0;
    for(diff_t i = 0; i < aLast - aFirst; ++ i)
    {
        t += ull(aFirst[i]) * b;
        *out ++ = t % radix;
//        aFirst[i] = t % radix;
        t /= radix;
    }
    return t;
}


/**

[aFirst, aLast) /= b

return:
    pair(x, y): [aFirst, x)表示 结果区间，y 表示remainder
precondition:
b > 0
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


/*

[first, last) content will change
 */
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

if [aFirst, aLast) < [bFirst, bLast): return -1
if [aFirst, aLast) == [bFirst, bLast): return 0
if [aFirst, aLast) > [bFirst, bLast): return 1
 */
template<typename RandomIterator1, typename RandomIterator2>
int compare(RandomIterator1 aFirst, RandomIterator1 aLast,
        RandomIterator2 bFirst, RandomIterator2 bLast)
{
    using diff_t = typename std::iterator_traits<RandomIterator1>::difference_type;
    diff_t asize = aLast - aFirst, bsize = bLast - bFirst;

    if(asize < bsize) return -1;
    if(asize > bsize) return 1;
    for(diff_t i = asize - 1; i >= 0; -- i)
    {
        if(aFirst[i] < bFirst[i]) return -1;
        if(aFirst[i] > bFirst[i]) return 1;
    }
    return 0;
}




template<typename RandomIterator1, typename RandomIterator2>
typename std::iterator_traits<RandomIterator1>::difference_type
calculateQuotientLength(RandomIterator1 a, RandomIterator1 aLast, RandomIterator2 b, RandomIterator2 bLast)
{
    using diff_t = typename std::iterator_traits<RandomIterator1>::difference_type;
    diff_t n = aLast - a, m = bLast - b;
    if(n < m) return 0;
    diff_t res = n - m;
    if(compare(a + n - m, a + n, b, bLast) >= 0) ++ res;
    return res;

}


/**
[out, outLast) = [aFirst, aLast) / [bFirst, bLast)

precondition:
[bFirst, bLast) > 0
*(bLast - 1) >= radix/2

postcondition:
余数将存储在[aFirst, aLast)
[bFirst, bLast) not change

 */
template<typename RandomIterator1, typename RandomIterator2, typename OutputIterator, typename ull>
std::pair<OutputIterator, RandomIterator1>
divideAndRemainderKnuthNormalized(RandomIterator1 a, RandomIterator1 aLast,
    RandomIterator2 b, RandomIterator2 bLast, OutputIterator qout, ull radix)
{
    using uint = typename std::iterator_traits<RandomIterator1>::value_type;
    using diff_t = typename std::iterator_traits<RandomIterator1>::difference_type;
    diff_t n = aLast - a, m = bLast - b;
    if(n < m)
    {
        return make_pair(qout, aLast);
    }

    uint* buffer = new uint[m + 1];
    for(diff_t i = n - m; i >= 0; -- i)
    {
        ull q_ = a[i + m - 1];
        if(i + m < n) q_ += a[i + m] * radix;
        q_ /= b[m - 1];
        q_ = std::min(q_, radix - 1);

        ull carry = multiplySchool(b, bLast, q_, buffer, radix);
        buffer[m] = carry;
        uint* bufferLast = removeLeadingZeros(buffer, buffer + m + 1);
        RandomIterator1 cntALast = a + i + m + 1;
        if(i + m == n) --cntALast;
        else
        {
            cntALast = removeLeadingZeros(a + i, cntALast);
        }

        while(compare(buffer, bufferLast, a + i, cntALast) > 0)
        {
            minusAssign(buffer, bufferLast, b, bLast, radix);
            --q_;
        }
        minusAssign(a + i, cntALast, buffer, bufferLast, radix);
        if(i == n - m && q_ == 0) continue;
        *qout ++ = q_;
    }

    delete[] buffer;
    return std::make_pair(qout, removeLeadingZeros(a, a + m));
}












//    uint bTopWord = *(bLast - 1);
//    uint d = radix / (bTopWord + 1) * bTopWord;
//
//    if(d > 1)
//    {
//        multiplyAssignSchool(aFirst, aLast, d, radix);
//        multiplyAssignSchool(bFirst, bLast, d, radix);
//    }























}} // namespace lz::BigIntegerPrivate





#endif /* LZ_BIG_INTEGER_CORE_H_ */
