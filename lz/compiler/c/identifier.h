/*
 * identifier.h
 *
 *  Created on: 2017年4月22日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_C_IDENTIFIER_H_
#define LZ_COMPILER_C_IDENTIFIER_H_


#include <lz/compiler/c/type.h>

namespace lz {


struct Identifier
{
//    enum class Type
//    {
//        Int,
//        Float,
//        Double,
//        Unknown,
//    };

    Type type; // int, float, double, ... 等基础类型
    std::string name; // 名字不可能相同，暂时不考虑作用域
    friend bool operator<(const Identifier &a, const Identifier &b)
    {
        return a.name < b.name;
    }

    Identifier() = default;
    Identifier(Type type, std::string name):
        type(type), name(name) {}

    Identifier(std::string name):
            name(name)
    {

    }



};


std::map<Identifier, int> identifierTable;

int insertIdentifierTable(Type type, std::string name)
{
    if(identifierTable.count(name))
    {
        std::cout << "conflicting declaration:" << name << endl;
        assert(0);
        return -1;
    }
    else
    {
        int addr = identifierTable.size();

        identifierTable.insert(std::make_pair(
            Identifier(Type::Category::Int, name),
            addr ) );
        return addr;
    }

}

} // namespace lz





#endif /* LZ_COMPILER_C_IDENTIFIER_H_ */
