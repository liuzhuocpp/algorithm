/*
 * lr1_grammar.h
 *
 *  Created on: 2017年4月6日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_LR1_GRAMMAR_H_
#define LZ_COMPILER_LR1_GRAMMAR_H_

#include <lz/compiler/grammar_design.h>

namespace lz {


template<typename Grammar>
struct ExtendItemDescriptor: std::tuple<
    typename Grammar::RuleDescriptor,
    typename Grammar::RuleSymbolIterator,
    typename Grammar::SymbolDescriptor >
{
    using RuleDescriptor = typename Grammar::RuleDescriptor;
    using RuleSymbolIterator = typename Grammar::RuleSymbolIterator;
    using SymbolDescriptor = typename Grammar::SymbolDescriptor;

    using Base = std::tuple<
            typename Grammar::RuleDescriptor,
            typename Grammar::RuleSymbolIterator,
            typename Grammar::SymbolDescriptor >;

    const RuleDescriptor& ruleDescirptor() const
    { return std::get<0>(*this); }
    const RuleSymbolIterator& ruleSymbolIterator() const
    { return std::get<1>(*this); }
    const RuleSymbolIterator& terminal() const
    { return std::get<2>(*this); }


    RuleDescriptor& ruleDescirptor()
    { return std::get<0>(*this); }
    RuleSymbolIterator& ruleSymbolIterator()
    { return std::get<1>(*this); }
    RuleSymbolIterator& terminal()
    { return std::get<2>(*this); }


    ExtendItemDescriptor(RuleDescriptor rule, RuleSymbolIterator ruleSymbol, SymbolDescriptor s):
        Base(rule, ruleSymbol, s)
    { }

    ExtendItemDescriptor() = default;
};

// 计算核心项集的闭包
template<typename Grammar>
void calculateKernelExtendItemSetClosure(
    const Grammar &g,
    std::vector<ExtendItemDescriptor<Grammar>> & itemSet)
{

    using ExtendItem = ExtendItemDescriptor<Grammar>;
    using SymbolDescriptor = typename Grammar::SymbolDescriptor;
    using RuleDescriptor = typename Grammar::RuleDescriptor;

    auto firstSets = calculateFirstSets(g);

    std::set<ExtendItem> ans;
    ans.insert(itemSet.begin(), itemSet.end());

    while(1)
    {
        auto oldSize = ans.size();
        for(ExtendItem item: itemSet)
        {
            if(item.ruleSymbolIterator() != g.ruleSymbols(item.ruleDescirptor()).end() &&
                isNonterminal(*item.ruleSymbolIterator()))
            {
                SymbolDescriptor B = *item.ruleSymbolIterator();
                for(RuleDescriptor rule: g.outRules(B))
                {
                    std::set<SymbolDescriptor> firstSet =
                        calculateRuleBodyFirstSet(
                            ++item.ruleSymbolIterator(),
                            g.ruleSymbols(item.ruleDescirptor()).end() ),
                            firstSets);
                    if(firstSets.count(EmptyStringSymbol))
                    {
                        firstSets.erase(EmptyStringSymbol);
                        firstSets.insert(item.terminal());
                    }
                    for(SymbolDescriptor firstSymbol: firstSets)
                    {
                        ans.insert(ExtendItem(rule, ++item.ruleSymbolIterator(), firstSymbol));
                    }
                }
            }
        }
        if(oldSize == ans.size()) break;
    }

    itemSet.assign(ans.begin(), ans.end());
}

template<typename Grammar>
auto calculateNextExtendItemSet(
    const Grammar& g,
    const std::vector<ExtendItemDescriptor<Grammar>> & I,
    typename Grammar::SymbolDescriptor X)
{

    using ExtendItem = ExtendItemDescriptor<Grammar>;
    using SymbolDescriptor = typename Grammar::SymbolDescriptor;
    using RuleDescriptor = typename Grammar::RuleDescriptor;

    std::set<ExtendItemDescriptor<Grammar>> J;
    for(ExtendItem item: I)
    {
        if(item.ruleSymbolIterator() != g.ruleSymbols(item.ruleDescirptor()).end() )
        {
            if(*item.ruleSymbolIterator() == X)
            {
                J.insert(ExtendItem(item.ruleDescirptor(), ++item.ruleSymbolIterator().end(), X));
            }
        }
    }
    std::vector<ExtendItemDescriptor<Grammar>> ans(J.begin(), J.end());
    calculateKernelExtendItemSetClosure(g, ans);
    return ans;
}

template<typename Grammar>
auto makeExtendItemSets(const Grammar& g, typename Grammar::RuleDescriptor startRule)
{
    using ExtendItem = ExtendItemDescriptor<Grammar>;
    using SymbolDescriptor = typename Grammar::SymbolDescriptor;
    using RuleDescriptor = typename Grammar::RuleDescriptor;

    std::vector<std::vector<ExtendItem > > ans(1);
    ans[0].push_back(ExtendItem(startRule, ++g.ruleSymbols(startRule).begin(), EndTagSymbol));
    calculateKernelExtendItemSetClosure(g, ans[0]);

    std::map<std::vector<ExtendItem>, int > flag;
    std::map<std::pair<int, SymbolDescriptor>, int> gotoTable;
    flag[ans[0]] = 0;


    while(1)
    {
        auto oldSize = ans.size();
        for(int i = int(ans.size()) - 1; i >= 0; -- i)
        {
            auto addSymbol = [&](SymbolDescriptor s)
            {
                auto next = calculateNextExtendItemSet(g, ans[i], s);
                if(!next.empty() && !flag.count(next))
                {
                    flag[next] = ans.size();
                    ans.push_back(next);

                }
                gotoTable[{flag[ans[i]], s}] = flag[next];
            }

            for(SymbolDescriptor terminal: g.terminals())
            {
                addSymbol(terminal);
            }

            for(SymbolDescriptor nonterminal: g.terminals())
            {
                addSymbol(nonterminal);
            }
        }
        if(oldSize == ans.size()) break;
    }

    return std::make_pair(ans, gotoTable);



}




} // namespace lz





#endif /* LZ_COMPILER_LR1_GRAMMAR_H_ */
