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


    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
    operator<<(std::basic_ostream<Char, Traits>& os,
               const Identifier&  id)
    {

        os << "Identifier("  << id.type << " " << id.name << ")";


        return os;
    }




};

struct IdentifierTable: std::map<Identifier, int>
{
    int insertIdentifier(Type type, std::string name)
    {
        if(count(name))
        {
            std::cout << "conflicting declaration:" << name << endl;
            assert(0);
            return -1;
        }
        else
        {
            int addr = size();

            insert(std::make_pair(
                Identifier(type, name),
                addr ) );
            return addr;
        }

    }

};



} // namespace lz





#endif /* LZ_COMPILER_C_IDENTIFIER_H_ */
