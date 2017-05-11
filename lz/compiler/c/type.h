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
#define X(type) type,
#include <lz/compiler/c/compound_type_list.def>
#include <lz/compiler/c/base_type_list.def>

#undef X
};






struct TypeTable;

struct TypeDescriptor
{
    friend TypeTable;
private:
    TypeCategory m_category = TypeCategory::Unknown;

    // index in arrayVector for Array;
    // index in identifierTable for Struct;

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


// only when category is Array, need this TypeTable
struct TypeTable
{
private:

    std::string typeCategoryToName(TypeCategory c)
    {
        std::string ans = "";

#define X(type) if(TypeCategory::type == c) ans = std::string(#type);
#include <lz/compiler/c/compound_type_list.def>
#include <lz/compiler/c/base_type_list.def>

#undef X
        if(ans.empty())
        {
            assert(0);
            return "";
        }
        else
        {
            ans[0] = ::tolower(ans[0]);
            return ans;
        }
    }

    static bool isBaseType(TypeCategory category)
    {
        switch(category)
        {
        case TypeCategory::Int:
        case TypeCategory::Double:
        case TypeCategory::Float:
        case TypeCategory::Bool:
            return true;
        default:
            return false;

        }
    }




    std::vector<std::pair<TypeDescriptor, std::vector<int>> > arrayVector;

    // every pair is identifier index and corresponding type
//    std::vector<std::vector<std::pair<int, TypeDescriptor> > > structNameVector;



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
                arrayDimensions(a) == arrayDimensions(b);
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
//        structNameVector.clear();
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



    void setArray(TypeDescriptor i, TypeDescriptor arrayBaseType, const std::vector<int>& arrayDimensions)
    {
        assert(i.category() ==  TypeCategory::Array);
        assert(i.index >= 0 && i.index < arrayVector.size());

        arrayVector[i.index] = {arrayBaseType, arrayDimensions};
    }

};


















} // namespace lz





#endif /* LZ_COMPILER_C_TYPE_H_ */
