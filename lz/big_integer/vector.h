/*
 * vector_core.h
 *
 *  Created on: 2017年1月18日
 *      Author: LZ
 */

#ifndef LZ_BIG_INTEGER_VECTOR_H_
#define LZ_BIG_INTEGER_VECTOR_H_

#include <lz/big_integer/basic.h>
#include <lz/big_integer/binary.h>

namespace lz { namespace BigIntegerPrivate {



template<typename uint, typename ull>
void plusAssign(std::vector<uint> &a, const std::vector<uint>& b, ull radix)
{
    using diff_t = typename std::vector<uint>::difference_type;
    diff_t aOldSize = a.size();
    if(a.size() < b.size()) a.resize(b.size());
    bool carry = plusAssign(a.begin(), a.begin() + aOldSize, b.begin(), b.end(), radix);
    if(carry) a.push_back(1);
}

template<typename uint, typename ull>
void minusAssign(std::vector<uint>&a, const std::vector<uint>&b, ull radix)
{
    using diff_t = typename std::vector<uint>::difference_type;
    auto aend = minusAssign(a.begin(), a.end(), b.begin(), b.end(), radix);
    a.resize(aend - a.begin());
}

template<typename uint, typename ull>
std::vector<uint> multiplySchool(const std::vector<uint>& a, const std::vector<uint>& b, ull radix)
{
    if(a.size() == 0 || b.size() == 0) return {};
    std::vector<uint> c(a.size() + b.size());
    auto cend = multiplySchool(a.begin(), a.end(), b.begin(), b.end(), c.begin(), radix);
    c.resize(cend - c.begin());
    return c;
}


template<typename uint, typename ull>
std::vector<uint> divideAndRemainderKnuth(std::vector<uint>& a, std::vector<uint> b, ull radix)
{
    uint factor = calculateNormalizedFactor(b.begin(), b.end(), radix);
    if(factor > 1)
    {
//        std::cout << "ENDTER:: " << std::endl;
        ull carry = multiplySchool(a.begin(), a.end(), factor, a.begin(), radix);
        if(carry > 0) a.push_back(carry);
        multiplySchool(b.begin(), b.end(), factor, b.begin(), radix);
    }

//    std::cout << "AB: " << a << "  " << b << std::endl;
    std::vector<uint> q(calculateQuotientLength(a.begin(), a.end(), b.begin(), b.end()));

//    std::cout << "Q:: " << q << std::endl;
    typename std::vector<uint>::iterator aend;

    std::tie(std::ignore, aend) = divideAndRemainderKnuthNormalized(
            a.begin(), a.end(), b.begin(), b.end(), q.rbegin(), radix);

    if(factor > 1)
    {
        std::tie(aend, std::ignore) = divideAndRemainderSchool(a.begin(), aend, factor, radix);
    }
    a.resize(aend - a.begin());
    return q;
}

template<typename uint, typename ull>
void radixTransform(std::vector<uint>& a, ull radix, ull newRadix)
{
    std::vector<uint> ans;
    radixTransform(a.begin(), a.end(), radix, std::back_inserter(ans), newRadix);
    a.swap(ans);
}




template<typename uint, typename BitOperator>
void bitOperateAssign(std::vector<uint>& a, const std::vector<uint>& b, BitOperator bitOperator)
{
    auto aOldSize = a.size();
    a.resize(std::max(a.size(), b.size()));
    auto aend = bitOperate(a.begin(), a.begin() + aOldSize, b.begin(), b.end(), a.begin(), bitOperator);
    a.resize(aend - a.begin());
}


/**
有待于完善
 */

//template<typename uint>
//void bitNot(std::vector<uint>& a)
//{
//    bitNot(a.begin(), a.end(), a.begin());
//}

template<typename uint, typename ull>
void shiftHigh(std::vector<uint>& a, ull b, ull log2Radix)
{
    shiftHigh(a.begin(), a.end(), b, log2Radix);
}


template<typename uint, typename ull>
void shiftLow(std::vector<uint>& a, ull b, ull log2Radix)
{
    shiftLow(a.begin(), a.end(), b, log2Radix);
}

template<typename uint>
int compare(const std::vector<uint>& a, const std::vector<uint>& b)
{
    return compare(a.begin(), a.end(), b.begin(), b.end());
}






} } // namespace lz::BigIntegerPrivate





#endif /* LZ_BIG_INTEGER_VECTOR_H_ */
