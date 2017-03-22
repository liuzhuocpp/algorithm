/*
 * slr1_grammar.h
 *
 *  Created on: 2017年3月18日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_NEW_SLR1_GRAMMAR_H_
#define LZ_COMPILER_NEW_SLR1_GRAMMAR_H_



namespace lz {


template<typename RuleDescriptor, typename RuleSymbolIterator>
struct ItemDescriptor
{
    RuleDescriptor ruleDescirptor() const
    {
        return rule;
    }
    RuleSymbolIterator ruleSymbolIterator() const
    {
        return ruleSymbol;
    }
    auto nextItem() const
    {
        ItemDescriptor res = *this;
        res.ruleSymbol++;
        res.pos++;
        return res;
    }
    ItemDescriptor(RuleDescriptor rule, RuleSymbolIterator ruleSymbol):
        rule(rule), ruleSymbol(ruleSymbol), pos(0)
    {

    }

    ItemDescriptor() = default;
    bool operator<(const ItemDescriptor& other ) const
    {
        if(rule != other.rule) return rule < other.rule;
        return pos < other.pos;
    }

private:
    int pos;
    RuleDescriptor rule;
    RuleSymbolIterator ruleSymbol;
};

template<typename Grammar>
std::vector<std::set<std::pair<typename Grammar::RuleDescriptor, typename Grammar::RuleSymbolIterator>>>
makeItemSets(const Grammar& g, Grammar::RuleDescriptor startRule)
{
    using RuleSymbolIterator = typename Grammar::RuleSymbolIterator;
    using RuleDescriptor = typename Grammar::RuleDescriptor;

    using Item = ItemDescriptor<RuleDescriptor, RuleSymbolIterator>;
    std::set<Item> startItem;
    auto symbolRange = g.ruleSymbols(startRule);
    startItem.insert({startRule, symbolRange.begin()});
}


template<typename Grammar>
auto calculateItemClosure(const Grammar &g,
    const std::set<std::pair<typename Grammar::RuleDescriptor, typename Grammar::RuleSymbolIterator>> & items )
{
    std::set<std::pair<typename Grammar::RuleDescriptor, typename Grammar::RuleSymbolIterator>>
        nonkernelItems;

    while(true)
    {
        auto oldSize = nonkernelItems.size();

        for(auto item: items)
        {
            auto symbolRange = g.ruleSymbols(item.first);
            ++item.second;
            if(item.second != symbolRange.end())
            {
                if(isNonterminal(*item.second))
                {
                    auto outRuleRange = g.outRules(*item.second);
                    for(auto outRule: outRuleRange)
                    {
                        nonkernelItems.insert({outRule, g.ruleSymbols(outRule).first});
                    }
                }
            }
        }
        if(oldSize == nonkernelItems.size())
        {
            break;
        }
    }
    return nonkernelItems;
}



template<typename Grammar>
SymbolDescriptor makeAugmentedGrammar(Grammar &g, SymbolDescriptor start)
{
    SymbolDescriptor newStart = g.addNonterminal();
    std::vector<SymbolDescriptor> newRule{newStart, start};
    g.addRule(newRule.begin(), newRule.end());
    return newStart;
}


//template<typename Grammar>
//struct AugmentedGrammar
//{
//    const Grammar& g;
//
//};



}// namesapce lz

#endif /* LZ_COMPILER_NEW_SLR1_GRAMMAR_H_ */
