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
    Optional(const Optional& other)
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

    }
    Optional& operator=(const Optional& other)
    {
        Optional::Optional(other);
        return *this;
    }

    Optional(Optional && other)
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
    }


    Optional& operator=(Optional && other)
    {
        Optional::Optional(std::move(other));
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
