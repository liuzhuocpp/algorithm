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
//    StructName, // 表示Identfier 是一个struct name
    Unknown,
};



//
//struct NewType
//{
//    TypeCategory m_category;
//
//    // for Array
//    NewType *arrayBaseType;
//    std::vector<int> arrayDimensions;
//
//    // for StructName
//    std::vector<NewType> structNameVector;
//
//};
//













struct TypeTable;

struct TypeDescriptor
{
    friend TypeTable;
private:
    TypeCategory m_category = TypeCategory::Unknown;

    // index in arrayVector for Array;
    // index in attributes for Struct;
    // index in structNameVector for StructName;

    int index = -1;

public:
    TypeDescriptor() {}
    TypeDescriptor(TypeCategory category):m_category(category){}

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

    std::vector<std::pair<TypeDescriptor, std::vector<int>> > arrayVector;

    // every pair is identifier index and corresponding type
    std::vector<std::vector<std::pair<int, TypeDescriptor> > > structNameVector;



public:
    TypeDescriptor insert(TypeCategory category)
    {
        TypeDescriptor newType = TypeDescriptor(category);

        switch(category)
        {
        case TypeCategory::Array:
            newType.index = arrayVector.size();
            arrayVector.push_back({});
            break;
//        case TypeCategory::StructName:
//            newType.index = structNameVector.size();
//            structNameVector.push_back({});
//            break;
        default:
            break;

        }
        return newType;
    }

    bool typeEqual(TypeDescriptor a, TypeDescriptor b)
    {
        if(a.m_category != b.m_category) return false;
        auto category = a.m_category;
        if(category == TypeCategory::Array)
        {
            return typeEqual(arrayBaseType(a), arrayBaseType(b)) &&
                arrayDimensions(a) == arrayDimensions(b);
        }
//        else if(category == TypeCategory::StructName)
//        {
//            return false;
//        }
        else if(category == TypeCategory::Struct)
        {
            return a.index == b.index;
        }
        else // 其余是base type
        {
            return true;
        }




    }

    void clear()
    {
        arrayVector.clear();
        structNameVector.clear();
    }

    TypeDescriptor arrayBaseType(TypeDescriptor i) const
    {
        return arrayVector[i.index].first;
    }

    TypeDescriptor& arrayBaseType(TypeDescriptor i)
    {
        return arrayVector[i.index].first;
    }


    const std::vector<int>& arrayDimensions(TypeDescriptor i) const
    {
        assert(i.category() ==  TypeCategory::Array);

        return arrayVector[i.index].second;
    }

    std::vector<int>& arrayDimensions(TypeDescriptor i)
    {
        assert(i.category() ==  TypeCategory::Array);

        return arrayVector[i.index].second;
    }


    void addArrayDimension(TypeDescriptor i, int n)
    {
        arrayVector[i.index].second.push_back(n);
    }

//    const std::vector<std::pair<int, TypeDescriptor>>& structNameMembers(TypeDescriptor i) const
//    {
//        assert(i.category() == TypeCategory::StructName);
//
//        return structNameVector[i.index];
//    }


    void setArray(TypeDescriptor i, TypeDescriptor arrayBaseType, const std::vector<int>& arrayDimensions)
    {
        assert(i.category() ==  TypeCategory::Array);
        assert(i.index >= 0 && i.index < arrayVector.size());

        arrayVector[i.index] = {arrayBaseType, arrayDimensions};
    }

//    void setStructName(TypeDescriptor i, const std::vector<std::pair<int, TypeDescriptor> > & structNameMembers)
//    {
//        assert(i.category() == TypeCategory::StructName);
//        assert(i.index >= 0 && i.index < structNameVector.size());
//
//        structNameVector[i.index] = structNameMembers;
//    }



};


















} // namespace lz





#endif /* LZ_COMPILER_C_TYPE_H_ */
