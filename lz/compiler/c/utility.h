/*
 * utility.h
 *
 *  Created on: 2017年4月28日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_C_UTILITY_H_
#define LZ_COMPILER_C_UTILITY_H_



#include <lz/compiler/c/identifier.h>
#include <lz/compiler/c/properties.h>
#include <lz/compiler/c/three_address_code.h>
#include <lz/compiler/c/global_identifier_table_pointer.h>

namespace lz {


struct GrammarInputData
{
    std::ofstream outStream, errorStream;

    IdentifierTable m_identifierTable;
    ThreeAddressCode m_threeAddressCode;
    TypeTable m_typeTable;



    //起始值设为非法的大负整数
    int offset = -0x7fffffff;



    GrammarInputData() {
        globalIdentifierTablePointer = &this->m_identifierTable;
    }
};



GrammarInputData grammarInputData;



struct TemporaryVariableNumberGenerator
{
    static int get()
    {
        return tempId++;
    }
    static void reset()
    {
        tempId = 0;
    }

private:
    static int tempId;
};

int TemporaryVariableNumberGenerator::tempId = 0;



unsigned getTemporaryVariableId()
{

    return TemporaryVariableNumberGenerator::get();
}



} // namespace lz





#endif /* LZ_COMPILER_C_UTILITY_H_ */
