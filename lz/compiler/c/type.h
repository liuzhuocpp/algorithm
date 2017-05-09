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

};



struct TypeTable
{
private:

    std::vector<std::pair<Type, std::vector<int>> > arrayVector;

    // every pair is identifier index and corresponding type
    std::vector<std::vector<std::pair<int, Type> > > structNameVector;

    std::vector<int> structVector;
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
        case TypeCategory::Struct:
            newType.index = structVector.size();
            structVector.push_back({});
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
        structVector.clear();
        arrayVector.clear();
        structNameVector.clear();
    }

//    int getWidth(Type t) const
//    {
//        int ans = 1; // 暂时先设为1
//
//        for(auto x: arrayDimensions(t))
//        {
//            ans *= x;
//        }
//        return ans;
//
//    }



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
//        assert(category(i) ==  TypeCategory::Array);

        return arrayVector[i.index].second;
    }

    std::vector<int>& arrayDimensions(Type i)
    {
//        assert(category(i) ==  TypeCategory::Array);

        return arrayVector[i.index].second;
    }


    void addArrayDimension(Type i, int n)
    {
        arrayVector[i.index].second.push_back(n);
    }

    const std::vector<std::pair<int, Type>>& structNameMembers(Type i) const
    {
//        assert(category(i) == TypeCategory::StructName);

        return structNameVector[i.index];
    }

    // struct 对应的structName的index
    int structIndex(Type i)
    {
//        assert(category(i) == TypeCategory::Struct);
        return structVector[i.index];
    }

    void setArray(Type i, Type arrayBaseType, const std::vector<int>& arrayDimensions)
    {
//        assert(category(i) ==  TypeCategory::Array);
//        assert(i >= 0 && i < arrayVector.size());

        arrayVector[i.index] = {arrayBaseType, arrayDimensions};
    }

    void setStructName(Type i, const std::vector<std::pair<int, Type> > & structNameMembers)
    {
//        assert(category(i) == TypeCategory::StructName);
//        assert(i >= 0 && i < structNameVector.size());

        structNameVector[i.index] = structNameMembers;
    }

    void setStruct(Type i, int structId)
    {
//        assert(category(i) == TypeCategory::Struct);
//        assert(identifierToId.count(structName));
        structVector[i.index] = structId;
    }

};

//struct Type
//{
//    enum class Category
//    {
//        Int,
//        Float,
//        Double,
//
//        Array,
////        Struct,
//        Unknown,
//    };
//
//
//private:
//    Category m_category;
//    std::vector<int> arrayDimensions;
//public:
//
//    Category getCategory() const
//    {
//        if(arrayDimensions.empty())
//        {
//            return m_category;
//        }
//        else
//        {
//            return Category::Array;
//        }
//    }
//    Category baseType() const
//    {
//        return m_category;
//    }
//
//
//
//
//
//    int getWidth() const
//    {
//        int ans = 1; // 暂时先设为1
//        for(auto x: arrayDimensions)
//        {
//            ans *= x;
//        }
//        return ans;
//
//    }
//
//    void addDimension(int x)
//    {
//        arrayDimensions.push_back(x);
//    }
//
//    Type subArray() const
//    {
//        Type ans(this->m_category);
//        ans.arrayDimensions.assign(this->arrayDimensions.begin() + 1, this->arrayDimensions.end());
//        return ans;
//    }
//
//
//
//    Type(Category cat = Category::Unknown):
//        m_category(cat){}
//
//
//
//
////    operator Category()
////    {
////        return m_category;
////    }
//
//    template <class Char, class Traits>
//    friend std::basic_ostream<Char, Traits>&
//    operator<<(std::basic_ostream<Char, Traits>& os,
//               const Type&  id)
//    {
//
//        os << "Type(";
//
//
//        if(id.m_category == Category::Float) os << "float" ;
//        else if(id.m_category == Category::Int) os << "int";
//        else if(id.m_category == Category::Double) os << "double";
//        else os << "unknown";
//
//        if(!id.arrayDimensions.empty())
//        {
//            auto cntRangeSplitter = currentRangeSplitter(os);
//            os <<commaRangeSplitter << id.arrayDimensions << cntRangeSplitter;
//
//        }
//        os << ")";
//
//
//        return os;
//    }
//
//
//
//};
//

















} // namespace lz





#endif /* LZ_COMPILER_C_TYPE_H_ */
