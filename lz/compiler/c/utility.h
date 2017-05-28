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


namespace lz {


struct GrammarInputData
{
    std::ofstream outStream, errorStream;

    IdentifierTable m_identifierTable;
    ThreeAddressCode m_codeTable;
    TypeTable m_typeTable;
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

//std::string getTemporaryVariableName()
//{
//
//    return "$" + std::to_string(TemporaryVariableNumberGenerator::get());
//}


unsigned getTemporaryVariableId()
{

    return TemporaryVariableNumberGenerator::get();
}



} // namespace lz





#endif /* LZ_COMPILER_C_UTILITY_H_ */
