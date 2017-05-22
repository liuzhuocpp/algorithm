/*
 * type.h
 *
 *  Created on: 2017年4月22日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_C_TYPE_H_
#define LZ_COMPILER_C_TYPE_H_


#include <lz/named_enum.h>

namespace lz {

//enum class TypeCategory
//{
//#define X(type) type,
//#include <lz/compiler/c/compound_type_list.def>
//#include <lz/compiler/c/base_type_list.def>
//
//#undef X
//};

#define  compound_type_list(X) \
    X(Array)\
    X(Struct)\
    X(Unknown)

#define  base_type_list(X) \
    X(Int)\
    X(Bool)\
    X(Float)\
    X(Double)



//enum class

#define allList(X) \
    compound_type_list(X)\
    base_type_list(X)

LZ_MAKE_NAMED_ENUM(TypeCategory, typeCategoryToName, allList)

#undef allList




struct TypeTable;

struct TypeDescriptor
{
    friend TypeTable;
private:
    TypeCategory m_category = TypeCategory::Unknown;

    // index in arrayVector for Array;
    int index = -1;

    // when m_category == Array, this field will be used.
    // 指明了当前的数组类型的维度是以arrayDimensionBeginId开始
    int arrayDimensionBeginId = 0;

public:
    TypeDescriptor() {}
    TypeDescriptor(TypeCategory category):m_category(category){}

    TypeCategory category() const
    {
        return m_category;
    }

};


bool isBaseType(TypeCategory category)
{
    auto range = LZ_GET_ENUM_LIST_RANGE(TypeCategory, base_type_list);
    return category >= range.first && category <= range.second;
}




// only when category is Array, need this TypeTable
struct TypeTable
{
private:



    std::vector<std::pair<TypeDescriptor, std::vector<int>> > arrayVector;


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
                arrayFullDimensionVector(a) == arrayFullDimensionVector(b);
        }
        else if(category == TypeCategory::Struct)
        {
            return a.index == b.index;
        }
        else // 其余是base type
        {
            return true;
        }
    }

    std::string typeToString(TypeDescriptor a)
    {
        if(isBaseType(a.category()))
        {
            return typeCategoryToName(a.category());
        }
        else if(a.category() == TypeCategory::Array)
        {
            std::string ans = typeToString(arrayBaseType(a));
            for(auto x: arrayDimensions(a))
            {
                ans += "[" + std::to_string(x) + "]";
            }
            return ans;
        }
        else
        {
            std::cout << typeCategoryToName(a.category()) << "+++" << std::endl;
            assert(0);
            return "";
        }
    }


    void clear()
    {
        arrayVector.clear();
    }

    // eg, if type i is : int[10][3][4], then the subarrayType is int[3][4]
    TypeDescriptor subarrayType(TypeDescriptor i) const
    {
        assert(i.category() == TypeCategory::Array);

        i.arrayDimensionBeginId ++;
        if(i.arrayDimensionBeginId == arrayFullDimensionVector(i).size())
        {
            return arrayBaseType(i);
        }
        return i;
    }

    TypeDescriptor arrayBaseType(TypeDescriptor i) const
    {
        return arrayVector[i.index].first;
    }

    TypeDescriptor& arrayBaseType(TypeDescriptor i)
    {
        return arrayVector[i.index].first;
    }

    //目前不允许外部修改array dimension 的值
    lz::IteratorRange<std::vector<int>::const_iterator>  arrayDimensions(TypeDescriptor i) const
    {
        assert(i.category() ==  TypeCategory::Array);

        const std::vector<int> & arrayDimensions = arrayVector[i.index].second;
        return lz::IteratorRange<std::vector<int>::const_iterator>(
                arrayDimensions.begin() + i.arrayDimensionBeginId,
                arrayDimensions.end()
                );
    }

    int getWidth(TypeDescriptor i) const
    {
        int ans = 1;
        if(isBaseType(i.category())) //目前不考虑类型的实际宽度
        {
            return ans;
        }

        for(auto x: arrayDimensions(i))
        {
            ans *= x;
        }
        return ans;


    }

    void addArrayDimension(TypeDescriptor i, int n)
    {
        arrayVector[i.index].second.push_back(n);
    }



    void setArray(TypeDescriptor i, TypeDescriptor arrayBaseType, const std::vector<int>& arrayDimensions)
    {
        assert(i.category() ==  TypeCategory::Array);
        assert(i.index >= 0 && i.index < arrayVector.size());

        arrayVector[i.index] = {arrayBaseType, arrayDimensions};
    }


private:
    const std::vector<int>& arrayFullDimensionVector(TypeDescriptor i) const
    {
        return arrayVector[i.index].second;
    }

};


















} // namespace lz





#endif /* LZ_COMPILER_C_TYPE_H_ */
