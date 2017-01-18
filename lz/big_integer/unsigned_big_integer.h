/*
 * unsigned_big_integer.h
 *
 *  Created on: 2017年1月16日
 *      Author: LZ
 */

#ifndef LZ_BIG_INTEGER_UNSIGNED_BIG_INTEGER_H_
#define LZ_BIG_INTEGER_UNSIGNED_BIG_INTEGER_H_

#include <lz/big_integer/core.h>
#include <algorithm>

namespace lz {


    namespace BigIntegerPrivate {

    }


class UnsignedBigInteger
{
    using uint = std::uint_fast32_t;
    using ull = std::uint_fast64_t;
    static constexpr ull Radix = (1ull << 32);
    std::vector<uint> mag;
    using U = UnsignedBigInteger;
public:

    UnsignedBigInteger(): mag(1, 0)
    {

    }
    friend U operator+(const U& a, const U& b)
    {
        U r;
        r.mag.resize(std::max(a.mag.size(), b.mag.size()) + 1u);
        std::copy(a.mag.begin(), a.mag.end(), r.mag.begin());
        BigIntegerPrivate::plusAssign(r.mag.begin(), r.mag.end(), b.mag.begin(), b.mag.end(), Radix);
        auto end = removeLeadingZeros(r.mag.begin(), r.mag.end());
        r.mag.resize(end - r.mag.begin());

    }


};



} // namespace lz





#endif /* LZ_BIG_INTEGER_UNSIGNED_BIG_INTEGER_H_ */
