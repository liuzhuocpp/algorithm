/*
 * properties.h
 *
 *  Created on: 2017年4月22日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_C_PROPERTIES_H_
#define LZ_COMPILER_C_PROPERTIES_H_


#include <lz/compiler/c/type.h>
namespace lz {



struct Properties
{
    // 若以a开头，后边的数字表示identifierTable中的下表；若是以t开头则表示临时变量；还有可能是从 LexicalSymbol 得到，
    // for expression
    std::string addr;
    Type type; // 类型， for declare and arrayExpression
    std::string arrayOffsetAddr; // 是一个地址，和addr表示的一样




    Properties() = default;
    Properties(LexicalSymbol t)
    {
        addr = t.value;

    }

};




} // namespace lz





#endif /* LZ_COMPILER_C_PROPERTIES_H_ */
