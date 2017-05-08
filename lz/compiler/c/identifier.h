/*
 * identifier.h
 *
 *  Created on: 2017年4月22日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_C_IDENTIFIER_H_
#define LZ_COMPILER_C_IDENTIFIER_H_


#include <lz/compiler/c/type.h>
#include <lz/utility.h>

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
//        : public EqualityComparableFacade<Type>
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

//    friend bool operator==(const Type &a, const Type &b)
//    {
//        return a.m_category == b.m_category && a.index == b.index;
//    }


};



struct TypeTable
{
private:
//    std::vector<Type> typeVector;

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
//        typeVector.clear();
        structVector.clear();
        arrayVector.clear();
        structNameVector.clear();
    }

    int getWidth(Type t) const
    {
        int ans = 1; // 暂时先设为1

        for(auto x: arrayDimensions(t))
        {
            ans *= x;
        }
        return ans;

    }


//    Type type(int i) const
//    {
//        return typeVector[i];
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


struct IdentifierTable
{
    TypeTable typeTable;
private:

    std::map<std::string, int> identifierToId;
    std::vector<std::pair<std::string, Type>> identifierAndTypes;

public:
    void clear()
    {
        identifierToId.clear();
        identifierAndTypes.clear();

        typeTable.clear();
    }
    int insert(std::string identifierName, Type t)
    {
        if(identifierToId.count(identifierName))
        {
            assert(0);
            return -1;
        }
        assert(identifierToId.size() == identifierAndTypes.size());

        int newId = identifierToId.size();
        identifierToId[identifierName] = newId;

//        Type newType = typeTable.insert(category);

        identifierAndTypes.push_back(std::make_pair(identifierName, t));


        return newId;
    }

    int find(std::string identifierName) const
    {
        auto it = identifierToId.find(identifierName);
        int ans = it->second;

        if(it == identifierToId.end()) ans = -1;
        return ans;
    }

    int index(std::string identifier) const
    {
        assert(identifierToId.count(identifier));
        return identifierToId.at(identifier);
    }

private:
    TypeCategory category(int i) const
    {
        return type(i).category();
    }
public:

    std::string identifier(int i)
    {
        return identifierAndTypes[i].first;
    }

    const Type& type(int i) const
    {
        return identifierAndTypes[i].second;
    }

    Type& type(int i)
    {
        return identifierAndTypes[i].second;
    }

//    Type arrayBaseType(int i) const
//    {
//        assert(category(i) ==  TypeCategory::Array);
//        return typeTable.arrayBaseType(type(i));
//
//    }

//    const std::vector<int>& arrayDimensions(int i) const
//    {
//        assert(category(i) ==  TypeCategory::Array);
//
//        return typeTable.arrayDimensions(type(i));
////        return arrayVector[type(i).index].second;
//    }
//
//    const std::vector<std::pair<int, Type>>& structNameMembers(int i) const
//    {
//        assert(category(i) == TypeCategory::StructName);
//
//
//        return typeTable.structNameMembers(type(i));
////        return structNameVector[i];
//    }

//    // struct 对应的structName的index
//    int structIndex(int i)
//    {
//        assert(category(i) == TypeCategory::Struct);
//        typeTable.structIndex(type(i));
//        return type(i).index;
//    }
//
//    void setArray(int i, Type arrayBaseType, const std::vector<int>& arrayDimensions)
//    {
//        assert(category(i) ==  TypeCategory::Array);
//        assert(i >= 0 && i < arrayVector.size());
//
//        arrayVector[i] = {arrayBaseType, arrayDimensions};
//    }
//
//    void setStructName(int i, const std::vector<std::pair<int, Type> > & structNameMembers)
//    {
//        assert(category(i) == TypeCategory::StructName);
//        assert(i >= 0 && i < structNameVector.size());
//
//        structNameVector[type(i).index] = structNameMembers;
//    }
//
//    void setStruct(int i, const std::string& structName)
//    {
//        assert(category(i) == TypeCategory::Struct);
//        assert(identifierToId.count(structName));
//
//        int structNameId = identifierToId[structName];
//        type(i).index = structNameId;
//    }




};



//struct Identifier
//{
//
//
//
//
//
//    Type type;
//    std::string name; // 名字不可能相同，暂时不考虑作用域
//    friend bool operator<(const Identifier &a, const Identifier &b)
//    {
//        return a.name < b.name;
//    }
//
//    Identifier() = default;
//    Identifier(Type type, std::string name):
//        type(type), name(name) {}
//
//    Identifier(std::string name):
//            name(name)
//    {
//
//    }
//
//
//    template <class Char, class Traits>
//    friend std::basic_ostream<Char, Traits>&
//    operator<<(std::basic_ostream<Char, Traits>& os,
//               const Identifier&  id)
//    {
//
//        os << "Identifier("  << id.type << " " << id.name << ")";
//
//
//        return os;
//    }
//
//
//
//
//};

//struct IdentifierTable: std::map<Identifier, int>
//{
//
//    std::vector<Identifier> identifiers;
//    void clear()
//    {
//        this->clear();
//        identifiers.clear();
//    }
//
//    long queryIdentifierIndex(const std::string &identifier) const
//    {
//        auto it = this->find(identifier);
//        return it->second;
//    }
//
//
//
//
//
//
//
//    int insertIdentifier(Type type, std::string name)
//    {
//        if(count(name))
//        {
//            std::cout << "conflicting declaration:" << name << endl;
//            assert(0);
//            return -1;
//        }
//        else
//        {
//            int addr = size();
//
//            insert(std::make_pair( Identifier(type, name), addr ) );
//            return addr;
//        }
//
//    }
//
//};



} // namespace lz





#endif /* LZ_COMPILER_C_IDENTIFIER_H_ */
