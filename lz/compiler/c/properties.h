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
namespace lz {



struct Properties
{
    // 若以a开头，后边的数字表示identifierTable中的下表；若是以t开头则表示临时变量；还有可能是从 LexicalSymbol 得到，
    // for expression
    std::string addr;

    TypeDescriptor type; // 类型， for declare and expression



    std::string arrayOffsetAddr; // 是一个地址，和addr表示的一样
    int cntArrayDimensionId = 0;

    std::list<int> trueList, falseList; // for condition expression
    std::list<int> nextList; // only for statement, not for statementList
    std::list<int> breakList, continueList; // for break continue 跳转 in statement and statementList

    int cntInstructionIndex = -1; // for conditionMark




    Properties() = default;
    Properties(LexicalSymbol t)
    {
        addr = t.value();

    }

};




} // namespace lz





#endif /* LZ_COMPILER_C_PROPERTIES_H_ */
