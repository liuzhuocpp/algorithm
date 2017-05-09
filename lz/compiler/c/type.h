/*
 * type.h
 *
 *  Created on: 2017年4月22日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_C_TYPE_H_
#define LZ_COMPILER_C_TYPE_H_



namespace lz {


enum class TypeCategory
{
    Int,
    Bool,
    Float,
    Double,


    Array,
    Struct, // 表示Identifier 的类型是一个struct
    StructName, // 表示Identfier 是一个struct name
    Unknown,
};

struct TypeTable;

struct Type
{
    friend TypeTable;
private:
    TypeCategory m_category = TypeCategory::Unknown;

    // index in arrayVector for Array;
    // index in attributes for Struct;
    // index in structNameVector for StructName;

    int index = -1;

public:
    Type() {}
    Type(TypeCategory category):m_category(category){}

    TypeCategory category() const
    {
        return m_category;
    }

    int getStructIndex()
    {
        assert(m_category == TypeCategory::Struct);
        return index;
    }

    void setStructIndex(int i)
    {
        assert(m_category == TypeCategory::Struct);
        index = i;
    }
};


// only when category is Array, StructName, need this TypeTable
struct TypeTable
{
private:

    std::vector<std::pair<Type, std::vector<int>> > arrayVector;

    // every pair is identifier index and corresponding type
    std::vector<std::vector<std::pair<int, Type> > > structNameVector;



public:
    Type insert(TypeCategory category)
    {
        Type newType = Type(category);

        switch(category)
        {
        case TypeCategory::Array:
            newType.index = arrayVector.size();
            arrayVector.push_back({});
            break;
        case TypeCategory::StructName:
            newType.index = structNameVector.size();
            structNameVector.push_back({});
            break;
        default:
            break;

        }
        return newType;
    }

    void clear()
    {
        arrayVector.clear();
        structNameVector.clear();
    }





    Type arrayBaseType(Type i) const
    {
        return arrayVector[i.index].first;
    }

    Type& arrayBaseType(Type i)
    {
        return arrayVector[i.index].first;
    }


    const std::vector<int>& arrayDimensions(Type i) const
    {
        assert(i.category() ==  TypeCategory::Array);

        return arrayVector[i.index].second;
    }

    std::vector<int>& arrayDimensions(Type i)
    {
        assert(i.category() ==  TypeCategory::Array);

        return arrayVector[i.index].second;
    }


    void addArrayDimension(Type i, int n)
    {
        arrayVector[i.index].second.push_back(n);
    }

    const std::vector<std::pair<int, Type>>& structNameMembers(Type i) const
    {
        assert(i.category() == TypeCategory::StructName);

        return structNameVector[i.index];
    }


    void setArray(Type i, Type arrayBaseType, const std::vector<int>& arrayDimensions)
    {
        assert(i.category() ==  TypeCategory::Array);
        assert(i.index >= 0 && i.index < arrayVector.size());

        arrayVector[i.index] = {arrayBaseType, arrayDimensions};
    }

    void setStructName(Type i, const std::vector<std::pair<int, Type> > & structNameMembers)
    {
        assert(i.category() == TypeCategory::StructName);
        assert(i.index >= 0 && i.index < structNameVector.size());

        structNameVector[i.index] = structNameMembers;
    }



};


















} // namespace lz





#endif /* LZ_COMPILER_C_TYPE_H_ */
