/*
 * properties.h
 *
 *  Created on: 2017年4月22日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_C_PROPERTIES_H_
#define LZ_COMPILER_C_PROPERTIES_H_


//#include <lz/compiler/c/type.h>
#include <lz/compiler/c/identifier.h>
#include <lz/compiler/c/three_address_code.h>

namespace lz {



struct Properties
{

    bool isLvalue = false;

    int arrayId = -1;
    ThreeAddressInstructionArgument addr;


    TypeDescriptor type;

    std::list<int> trueList, falseList; // for condition expression
    std::list<int> nextList; // only for statement, not for statementList
    std::list<int> breakList, continueList; // for break continue 跳转 in statement and statementList

    int cntInstructionIndex = -1; // for conditionMark




    Properties() = default;
    std::string lexValue;
    Properties(LexicalSymbol t)
    {
        lexValue = t.value();

    }

};




} // namespace lz





#endif /* LZ_COMPILER_C_PROPERTIES_H_ */
