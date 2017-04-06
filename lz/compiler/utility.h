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

enum class ActionType
{
   Shift, Reduce, Accept,
};

template<typename RuleDescriptor>
struct ActionValue
{
    ActionType type;

    int itemSetId; // 移入
    RuleDescriptor rule; // 规约
    ActionValue(){}
    ActionValue(int i):type(ActionType::Shift), itemSetId(i){}
    ActionValue(RuleDescriptor rule):type(ActionType::Reduce), rule(rule){}
    ActionValue(ActionType type):type(type){}

    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
        operator<<(std::basic_ostream<Char, Traits>& os, const ActionValue& av)
    {
        if(av.type == ActionType::Accept)
        {
            os << "Accept";
        }
        else if(av.type == ActionType::Reduce)
        {
            os << "Reduce";
        }
        else if(av.type == ActionType::Shift)
        {
            os << "Shift";
        }
        else os << "error action type";



        return os;
    }


};


template<typename Grammar>
auto makeAugmentedGrammar(Grammar &g, SymbolDescriptor start)
{
    SymbolDescriptor newStart = g.addNonterminal();
    std::vector<SymbolDescriptor> newRule{newStart, start};
    return g.addRule(newRule.begin(), newRule.end());
}



}// namesapce lz

#endif /* LZ_COMPILER_UTILITY_H_ */
