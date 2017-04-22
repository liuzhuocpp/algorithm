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
    enum class Category
    {
        Int,
        Float,
        Double,
        Unknown,
    };

    Category category;
    Type(Category cat = Category::Unknown):
        category(cat){}

    operator Category()
    {
        return category;
    }
};


















} // namespace lz





#endif /* LZ_COMPILER_C_TYPE_H_ */
