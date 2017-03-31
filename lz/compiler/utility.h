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


// 把每个terminal转化为一个[0,n)的整数，其中n是不同的终结符的数目
template<typename Iterator, typename TerminalIndexMap>
struct TerminalIndexIterator:
    public IteratorAdapter<TerminalIndexIterator< Iterator, TerminalIndexMap>,
            Iterator,
            typename std::iterator_traits<Iterator>::iterator_category,
            typename MapTraits<TerminalIndexMap>::ValueType >
{
    using Base = IteratorAdapter<TerminalIndexIterator< Iterator, TerminalIndexMap>,
            Iterator,
            typename std::iterator_traits<Iterator>::iterator_category,
            typename MapTraits<TerminalIndexMap>::ValueType >;

    TerminalIndexMap indexMap;

    TerminalIndexIterator()
    {}

    TerminalIndexIterator(Iterator it, TerminalIndexMap indexMap):
        Base(it), indexMap(indexMap)
    {}

    typename MapTraits<TerminalIndexMap>::ValueType operator*() const
    {
//        std::cout << "??" << std::endl;
        return indexMap[*this->base()];
    }

};





}// namesapce lz

#endif /* LZ_COMPILER_UTILITY_H_ */
