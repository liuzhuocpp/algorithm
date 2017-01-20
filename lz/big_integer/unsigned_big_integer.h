/*
 * unsigned_big_integer.h
 *
 *  Created on: 2017年1月16日
 *      Author: LZ
 */

#ifndef LZ_BIG_INTEGER_UNSIGNED_BIG_INTEGER_H_
#define LZ_BIG_INTEGER_UNSIGNED_BIG_INTEGER_H_

#include <lz/big_integer/vector.h>
#include <algorithm>
#include <vector>
namespace lz {



class UnsignedBigInteger;


    namespace BigIntegerPrivate {





    UnsignedBigInteger parseLiteral(const char*);






    }


class UnsignedBigInteger
{
    using uint = std::uint_fast32_t;
    using ull = std::uint_fast64_t;
    static constexpr ull Radix = (1ull << 32);
    std::vector<uint> mag;
    using U = UnsignedBigInteger;
    using Mag = std::vector<uint>;
protected:
    UnsignedBigInteger(const std::vector<uint>& _mag): mag(_mag) {}
    UnsignedBigInteger(std::vector<uint>&& _mag): mag(std::move(_mag)) {}

public:

    UnsignedBigInteger() = default;
    UnsignedBigInteger(const char* _a):
        UnsignedBigInteger(std::string(_a)) { }

    UnsignedBigInteger(const std::string& _a)
    {
        if(_a.size() == 0 || (_a.size() == 1 && _a[0] == 0)) return ;
        mag.assign(_a.rbegin(), _a.rend());
        for(auto& x: mag)
        {
            x -= '0';
        }
        BigIntegerPrivate::radixTransform(mag, 10ULL, Radix);
    }
    UnsignedBigInteger(uint x)
    {
        if(x == 0) return ;
        mag.assign(1, x);
    }

    UnsignedBigInteger(int_fast32_t x)
    {
        if(x == 0) return;
        if(x < 0)
        {
            assert(0);
            return ;
        }
        mag.assign(1, x);
    }

    UnsignedBigInteger(ull x)
    {
        if(x == 0) return;
        mag.resize(2);
        mag[0] = x % Radix;
        mag[1] = x >> 32;
    }

    UnsignedBigInteger(int_fast64_t x)
    {
        if(x == 0) return;
        if(x < 0)
        {
            assert(0);
            return ;
        }
        mag.resize(2);
        mag[0] = x % Radix;
        mag[1] = x >> 32;
    }

    std::string toString(ull newRadix = 10ULL) const
    {
        auto mutableMag = mag;
        BigIntegerPrivate::radixTransform(mutableMag, Radix, newRadix);

        std::string ans = std::string(mutableMag.rbegin(), mutableMag.rend());
        for(auto& x: ans)
        {
            x += '0';
        }
        if(ans.size() == 0) ans.push_back('0');

        return ans;
    }

    friend U operator+(const U& a, const U& b)
    {
        Mag ans(a.mag);
        BigIntegerPrivate::plusAssign(ans, b.mag, Radix);
        return ans;
    }

    friend U operator-(const U& a, const U& b)
    {
        Mag ans(a.mag);
        BigIntegerPrivate::minusAssign(ans, b.mag, Radix);
        return ans;
    }

    friend U operator*(const U& a, const U& b)
    {
        Mag ans = BigIntegerPrivate::multiplySchool(a.mag, b.mag, Radix);
        return ans;
    }

    friend U operator/(const U& a, const U& b)
    {
        Mag mutableA = a.mag;
        Mag ans = BigIntegerPrivate::divideAndRemainderKnuth(mutableA, b.mag, Radix);
        return ans;
    }

    friend U operator%(const U& a, const U& b)
    {
        Mag mutableA = a.mag;
        BigIntegerPrivate::divideAndRemainderKnuth(mutableA, b.mag, Radix);
        return mutableA;
    }

    U& operator+=(const U& b)
    {
        BigIntegerPrivate::plusAssign(mag, b.mag, Radix);
        return *this;
    }

    U& operator-=(const U& b)
    {
        BigIntegerPrivate::minusAssign(mag, b.mag, Radix);
        return *this;
    }

    U& operator*=(const U& b)
    {
        *this = *this * b;
        return *this;
    }

    U& operator/=(const U& b)
    {
        *this = *this / b;
        return *this;
    }
    U& operator%=(const U& b)
    {
        BigIntegerPrivate::divideAndRemainderKnuth(mag, b.mag, Radix);
        return *this;
    }


    U& operator^=(const U& b)
    {
        BigIntegerPrivate::bitOperateAssign(mag, b.mag, std::bit_xor<uint>());
        return *this;
    }

    U& operator|=(const U& b)
    {
        BigIntegerPrivate::bitOperateAssign(mag, b.mag, std::bit_or<uint>());
        return *this;
    }

    U& operator&=(const U& b)
    {
        BigIntegerPrivate::bitOperateAssign(mag, b.mag, std::bit_and<uint>());
        return *this;
    }


    friend U operator^(const U& a, const U& b)
    {
        U ans = a;
        ans ^= b;
        return ans;
    }

    friend U operator|(const U& a, const U& b)
    {
        U ans = a;
        ans |= b;
        return ans;
    }

    friend U operator&(const U& a, const U& b)
    {
        U ans = a;
        ans &= b;
        return ans;
    }

    /**
            有待于完善
     */
//    friend U operator~(const U&a)
//    {
//        U ans = a;
//        BigIntegerPrivate::bitNot(ans.mag);
//        return ans;
//    }











    friend bool operator== (const U& a, const U& b)
    {
        return BigIntegerPrivate::compare(a.mag, b.mag) == 0;
    }
    friend bool operator!= (const U& a, const U& b)
    {
        return BigIntegerPrivate::compare(a.mag, b.mag) != 0;
    }

    friend bool operator< (const U& a, const U& b)
    {
        return BigIntegerPrivate::compare(a.mag, b.mag) < 0;
    }

    friend bool operator<= (const U& a, const U& b)
    {
        return BigIntegerPrivate::compare(a.mag, b.mag) <= 0;
    }

    friend bool operator> (const U& a, const U& b)
    {
        return BigIntegerPrivate::compare(a.mag, b.mag) > 0;
    }
    friend bool operator>= (const U& a, const U& b)
    {
        return BigIntegerPrivate::compare(a.mag, b.mag) >= 0;
    }







    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
    operator<<(std::basic_ostream<Char, Traits>& os,
               const U& u)
    {
        return os << u.toString();
    }


    friend UnsignedBigInteger BigIntegerPrivate::parseLiteral(const char*);
};


    namespace BigIntegerPrivate {

    UnsignedBigInteger parseLiteral(const char*literal)
    {
        using U = UnsignedBigInteger;
        using uint = uint_fast32_t;
        if(literal[0] == '0' && (literal[1] == 'x' || literal[1] == 'X'))
        {
            std::vector<uint> a;
            std::ptrdiff_t i = 2;
            for(;literal[i] == '0'; ++ i);

            for(;literal[i]; ++ i)
            {
                char ch = literal[i];
                uint x;
                if(ch >= '0' && ch <= '9') x = ch - '0';
                else if(ch >= 'a' && ch <= 'f') x = ch - 'a' + 10;
                else x = ch - 'A' + 10;
                a.push_back(x);
            }
            std::reverse(a.begin(), a.end());
            BigIntegerPrivate::radixTransform(a, 16ULL, (1ULL << 32));
            return a;

        }
        else if(literal[0] == '0')
        {
            std::vector<uint> a;
            std::ptrdiff_t i = 1;
            for(;literal[i] == '0'; ++ i);

            for(;literal[i]; ++ i)
            {
                a.push_back(literal[i] - '0');
            }
            std::reverse(a.begin(), a.end());
            BigIntegerPrivate::radixTransform(a, 8ULL, (1ULL << 32));
            return a;

        }
        else
        {
            return U(literal);
        }

    }

    }


UnsignedBigInteger operator"" _UB(const char* literal)
{

    return BigIntegerPrivate::parseLiteral(literal);

}

UnsignedBigInteger operator"" _ub(const char* literal)
{
    return operator"" _UB(literal);
}

UnsignedBigInteger operator"" _Ub(const char* literal)
{
    return operator"" _UB(literal);
}

UnsignedBigInteger operator"" _uB(const char* literal)
{
    return operator"" _UB(literal);
}





} // namespace lz





#endif /* LZ_BIG_INTEGER_UNSIGNED_BIG_INTEGER_H_ */
