/*
 * global_identifier_table_pointer.h
 *
 *  Created on: 2017年5月28日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_C_GLOBAL_IDENTIFIER_TABLE_POINTER_H_
#define LZ_COMPILER_C_GLOBAL_IDENTIFIER_TABLE_POINTER_H_


#include <lz/compiler/c/identifier.h>


namespace lz {

// 指向grammarInputData.m_identifierTable
IdentifierTable *globalIdentifierTablePointer;


} // namespace lz





#endif /* LZ_COMPILER_C_GLOBAL_IDENTIFIER_TABLE_POINTER_H_ */
