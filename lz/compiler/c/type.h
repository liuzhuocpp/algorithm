/*
 * type.h
 *
 *  Created on: 2017年4月22日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_C_TYPE_H_
#define LZ_COMPILER_C_TYPE_H_



namespace lz {



struct Type
{
//private:
    enum class Category
    {
        Int,
        Float,
        Double,

        Array,
//        Struct,
        Unknown,
    };

public:

    bool isInt() const
    {
        return category == Category::Int;
    }

    bool isFloat() const
    {
        return category == Category::Float;
    }

    bool isDouble() const
    {
        return category == Category::Double;
    }

    bool isArray() const
    {
        return !arrayDimensions.empty();
    }




    Category category;

    std::vector<int> arrayDimensions; // for Array Type;

    Type(Category cat = Category::Unknown):
        category(cat){}




    operator Category()
    {
        return category;
    }




};


















} // namespace lz





#endif /* LZ_COMPILER_C_TYPE_H_ */
