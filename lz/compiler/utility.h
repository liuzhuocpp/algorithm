/*
 * utility.h
 *
 *  Created on: 2017年3月30日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_UTILITY_H_
#define LZ_COMPILER_UTILITY_H_



namespace lz{

template<typename P>
using SemanticRuleType = std::function<void(const std::vector<P>&, P&)>;




template<typename Grammar>
struct GrammarTraits
{
    using NodeProperties = typename Grammar::NodeProperties;
    using SizeType = typename Grammar::SizeType;
    using RuleDescriptor = typename Grammar::RuleDescriptor;


    using RuleIterator = typename Grammar::RuleIterator;
    using RuleSymbolIterator = typename Grammar::RuleSymbolIterator;
    using NonterminalIterator = typename Grammar::NonterminalIterator;
    using TerminalIterator = typename Grammar::NonterminalIterator;
    using OutRuleIterator = typename Grammar::OutRuleIterator;

};





}// namesapce lz

#endif /* LZ_COMPILER_UTILITY_H_ */
