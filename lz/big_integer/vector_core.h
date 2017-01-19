/*
 * vector_core.h
 *
 *  Created on: 2017年1月18日
 *      Author: LZ
 */

#ifndef LZ_BIG_INTEGER_VECTOR_CORE_H_
#define LZ_BIG_INTEGER_VECTOR_CORE_H_

#include <lz/big_integer/core.h>


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
        multiplySchool(a.begin(), a.end(), factor, a.begin(), radix);
        multiplySchool(b.begin(), b.end(), factor, b.begin(), radix);
    }

    std::vector<uint> q(calculateQuotientLength(a.begin(), a.end(), b.begin(), b.end()));
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




} } // namespace lz::BigIntegerPrivate





#endif /* LZ_BIG_INTEGER_VECTOR_CORE_H_ */
