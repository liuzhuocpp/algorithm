/*
 * grammar_design.h
 *
 *  Created on: 2017年2月28日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_GRAMMAR_DESIGN_H_
#define LZ_COMPILER_GRAMMAR_DESIGN_H_

/*
 *
 包含了一些对文法重新设计、改造的算法，包括 计算first集、follow集，消除左递归，提取左公共因子
 */

#include <lz/compiler/new/grammar.h>

namespace lz {


using Set = std::set<SymbolDescriptor>;


template<typename InputIterator>
Set calculateRuleBodyFirstSet(InputIterator ruleBodyBegin, InputIterator ruleBodyEnd,
    const std::vector<Set>& nonterminalsFirstSet);


template<typename Grammar>
std::vector<Set> calculateFirstSets(const Grammar& g)
{
    std::vector<Set> ans(g.size());
    while(true)
    {
        bool hasNew = 0;
        for(auto rd: g.rules())
        {
            auto ruleSymbolRange = g.ruleSymbols(rd);
            auto head = *ruleSymbolRange.first;
            auto ruleBodyFirstSet = calculateRuleBodyFirstSet(++ruleSymbolRange.first, ruleSymbolRange.second, ans);
            auto oldSize = ans[head].size();
            ans[head].insert(ruleBodyFirstSet.begin(), ruleBodyFirstSet.end());
            if(ans[head].size() > oldSize) hasNew = 1;
        }
        if(!hasNew) break;
    }

    return ans;
}

template<typename Iterator>
Set calculateRuleBodyFirstSet(
    Iterator ruleBodyBegin,
    Iterator ruleBodyEnd,
    const std::vector<Set>& firstSets)
{
    Set ans;

    bool allHasEmptyString = 1;
    for(auto s: lz::makeIteratorRange(ruleBodyBegin, ruleBodyEnd))
    {
        if(isNonterminal(s))
        {
            ans.insert(firstSets[s].begin(), firstSets[s].end());
            if(!firstSets[s].count(EmptyStringSymbol))
            {
                allHasEmptyString = 0;
                break;
            }
            else ans.erase(EmptyStringSymbol);
        }
        else if(isTerminal(s))
        {
            ans.insert(s);
            allHasEmptyString = 0;
            break;
        }

        else // error
        {}
    }
    if(allHasEmptyString) ans.insert(EmptyStringSymbol);

    return ans;


}

template<typename Grammar>
std::vector<Set> calculateFollowSets(
    const Grammar& g,
    const std::vector<Set>& firstSets,
    SymbolDescriptor startSymbol = 0)
{
    std::vector<Set> ans(g.size());

    ans[startSymbol].insert(EndTagSymbol);

    while(true)
    {
        bool hasNew = 0;

        for(auto rd: g.rules())
        {
            auto ruleSymbolRange = g.ruleSymbols(rd);
            auto head = *ruleSymbolRange.first ++;

            for(auto it = ruleSymbolRange.first ; it != ruleSymbolRange.second; ++ it)
            {
                auto s = *it;
                if(isNonterminal(s))
                {
                    Set::size_type oldSize = ans[s].size();
                    auto nextIt = it;

                    auto backFirstSet = calculateRuleBodyFirstSet(++nextIt, ruleSymbolRange.second, firstSets);
                    ans[s].insert(backFirstSet.begin(), backFirstSet.end());
                    if(backFirstSet.count(EmptyStringSymbol))
                    {
                        ans[s].erase(EmptyStringSymbol);
                        ans[s].insert(ans[head].begin(), ans[head].end());
                    }
                    if(ans[s].size() > oldSize) hasNew = 1;
                }

            }

        }
        if(!hasNew) break;
    }

    return ans;
}

























} // namespace lz





#endif /* LZ_COMPILER_GRAMMAR_DESIGN_H_ */























