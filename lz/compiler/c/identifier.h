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


struct IdentifierTable
{
private:

    std::map<std::string, int> identifierToId;
    std::vector<std::pair<std::string, TypeDescriptor>> identifierAndTypes;

public:
    void clear()
    {
        identifierToId.clear();
        identifierAndTypes.clear();

    }
    int insert(std::string identifierName, TypeDescriptor t)
    {
        if(identifierToId.count(identifierName))
        {
            assert(0);
            return -1;
        }
        assert(identifierToId.size() == identifierAndTypes.size());

        int newId = identifierToId.size();
        identifierToId[identifierName] = newId;


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

    const TypeDescriptor& type(int i) const
    {
        return identifierAndTypes[i].second;
    }

    TypeDescriptor& type(int i)
    {
        return identifierAndTypes[i].second;
    }



};


struct StructNameTable
{
private:

    std::map<std::string, int> structNameToId;
    std::vector<std::pair<std::map<std::string, int>, std::vector<TypeDescriptor>  >  >   structMembers;


public:
    void clear()
    {
        structNameToId.clear();
        structMembers.clear();

    }
    int insert(std::string structName)
    {
        if(structNameToId.count(structName))
        {
            assert(0);
            return -1;
        }

        assert(structNameToId.size() == structMembers.size());

        int newId = structNameToId.size();
        structNameToId[structName] = newId;


        structMembers.push_back({});
        return newId;
    }

    void addStructMember(int structId, std::string memberName, TypeDescriptor memberType)
    {
        structMembers[structId].first[memberName] = structMembers[structId].first.size();
        structMembers[structId].second.push_back(memberType);
    }

    int structMemberIndex(int structId, std::string memberName) const
    {
        return structMembers[structId].first.at(memberName);
    }

    int structMemberNumber(int structId) const
    {
        return structMembers[structId].first.size();
    }

    TypeDescriptor structMemberType(int structId, int memberIndex) const
    {
        return structMembers[structId].second[memberIndex];
    }

    TypeDescriptor structMemberType(int structId, std::string memberName) const
    {
        int memberIndex = structMemberIndex(structId, memberName);
        return structMembers[structId].second[memberIndex];
    }



    int find(std::string identifierName) const
    {
        auto it = structNameToId.find(identifierName);
        int ans = it->second;

        if(it == structNameToId.end()) ans = -1;
        return ans;
    }




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
