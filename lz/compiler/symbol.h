/*
 * symbol.h
 *
 *  Created on: 2017年3月17日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_NEW_SYMBOL_H_
#define LZ_COMPILER_NEW_SYMBOL_H_



namespace lz{

using SymbolDescriptor = long;

constexpr SymbolDescriptor TerminalSymbolBegin = std::numeric_limits<SymbolDescriptor>::min();
constexpr SymbolDescriptor SemanticRuleSymbolBegin = std::numeric_limits<SymbolDescriptor>::min() >> 1;
constexpr SymbolDescriptor SemanticRuleSymbolEnd = -10;

constexpr SymbolDescriptor NullSymbol = -3;
constexpr SymbolDescriptor EmptyStringSymbol = -2;
constexpr SymbolDescriptor EndTagSymbol = -1;


SymbolDescriptor makeTerminal(std::ptrdiff_t terminalId)
{
    return terminalId + TerminalSymbolBegin;
}

SymbolDescriptor makeNonterminal(std::ptrdiff_t nonterminalId)
{
    return nonterminalId;
}


std::ptrdiff_t getTerminalId(SymbolDescriptor s)
{
    return s - TerminalSymbolBegin;
}


std::ptrdiff_t getNonterminalId(SymbolDescriptor s)
{
    return s;
}


bool isNonterminal(SymbolDescriptor s)
{
    return s >= 0;
}

bool isTerminal(SymbolDescriptor s)
{
    return s < SemanticRuleSymbolBegin;
}

bool isSemanticRule(SymbolDescriptor s)
{
    return s >= SemanticRuleSymbolBegin && s < SemanticRuleSymbolEnd;
}

bool isEmptyString(SymbolDescriptor s)
{
    return s == EmptyStringSymbol;
}

bool isEndTag(SymbolDescriptor s)
{
    return s == EndTagSymbol;
}


}// namesapce lz

#endif /* LZ_COMPILER_NEW_SYMBOL_H_ */
