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



#define  compound_type_list(X) \
    X(Array)\
    X(Struct)\
    X(Unknown)

#define  base_type_list(X) \
    X(Void)\
    X(Int)\
    X(Bool)\
    X(Float)\
    X(Double)

#define other_type_list(X) \
    X(Function)



//enum class

#define allList(X) \
    compound_type_list(X)\
    base_type_list(X) \
    other_type_list(X)

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
    TypeDescriptor(TypeCategory category):m_category(category)
    {
        if(category == TypeCategory::Array)
        {
            assert(0);
        }
    }

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

    // 对类型i扩展一维数组,类型i作为基类型，extent作为新数组的第一维
    TypeDescriptor makeArray(TypeDescriptor i, int extent)
    {
        if(i.category() == TypeCategory::Array)
        {
            arrayFullDimensionVector(i).push_back(extent);
            return i;
        }
        else
        {


            TypeDescriptor newType;
            newType.m_category = TypeCategory::Array;

            newType.index = arrayVector.size();
            arrayVector.push_back({});
            arrayBaseType(newType).m_category = i.category();
            arrayFullDimensionVector(newType).push_back(extent);

            return newType;
        }

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
        int ans = -1;
        switch(i.category())
        {
        case TypeCategory::Array:
            ans = getWidth(arrayBaseType(i));
            for(int d: arrayDimensions(i)) ans *= d;
            return ans;
        case TypeCategory::Bool:
            return 1;
        case TypeCategory::Int:
            return 4;
        case TypeCategory::Float:
            return 4;
        case TypeCategory::Double:
            return 8;
        default:
            std::cout << typeCategoryToName(i.category()) << std::endl;
            assert(0);
        }

        return -1;

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

    std::vector<int>& arrayFullDimensionVector(TypeDescriptor i)
    {
        return arrayVector[i.index].second;
    }

};


















} // namespace lz





#endif /* LZ_COMPILER_C_TYPE_H_ */
