/*
 * optional.h
 *
 *  Created on: 2017年3月12日
 *      Author: LZ
 */

#ifndef LZ_OPTIONAL_H_
#define LZ_OPTIONAL_H_



namespace lz{





template<typename T>
struct Optional
{
    Optional() {}

    Optional(const T& t)
    {
        *this = t;
    }

    Optional& operator=(const T& t)
    {
        if(val == nullptr) val = new T(t);
        else *val = t;
        return *this;
    }

    Optional(const Optional& other)
    {
        *this = other;
    }

    Optional& operator=(const Optional& other)
    {
        if(other.val != nullptr)
        {
            if(val == nullptr) val = new T(*other.val);
            else *val = *other.val;
        }
        else
        {
            if(val != nullptr)
            {
                delete val;
                val = nullptr;
            }
        }
        return *this;
    }

    Optional(Optional && other)
    {
        *this = std::move(other);
    }

    Optional& operator=(Optional && other)
    {
        if(other.val != nullptr)
        {
            if(val == nullptr) val = new T(std::move(*other.val));
            else *val = std::move(*other.val);
        }
        else
        {
            if(val != nullptr)
            {
                delete val;
                val = nullptr;
            }
        }
        return *this;
    }

    bool hasValue() const
    {
        return val != nullptr;
    }

    const T& value() const
    {
        return *val;
    }

    T& value()
    {
        return *val;
    }

    ~Optional()
    {
        if(val != nullptr) delete val;
    }
private:
    T* val = nullptr;
};











}// namesapce lz

#endif /* LZ_OPTIONAL_H_ */
