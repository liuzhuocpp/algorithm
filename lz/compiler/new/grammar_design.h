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


Set calculateRuleBodyFirstSet(RuleBody::iterator ruleBodyBegin, RuleBody::iterator ruleBodyEnd,
    const std::vector<Set>& nonterminalsFirstSet);


std::vector<Set> calculateFirstSets(const Grammar& g)
{
    std::vector<Set> ans(g.size());
    while(true)
    {
        bool hasNew = 0;
        for(auto A: lz::irange(g.size()) )
        {
            for(auto ruleBody: g[A])
            {
                auto ruleBodyFirstSet = calculateRuleBodyFirstSet(ruleBody.begin(), ruleBody.end(), ans);
                auto oldSize = ans[A].size();
                ans[A].insert(ruleBodyFirstSet.begin(), ruleBodyFirstSet.end());
                if(ans[A].size() > oldSize) hasNew = 1;
            }
        }
        if(!hasNew) break;
    }

    return ans;
}

Set calculateRuleBodyFirstSet(
    RuleBody::iterator ruleBodyBegin,
    RuleBody::iterator ruleBodyEnd,
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

        for(auto A: lz::irange(g.size()) )
        {
            for(auto ruleBody: g[A])
            {
                for(auto it = ruleBody.begin(); it != ruleBody.end(); ++ it)
                {
                    auto s = *it;
                    if(isNonterminal(s))
                    {
                        Set::size_type oldSize = ans[s].size();

                        auto backFirstSet = calculateRuleBodyFirstSet(it + 1, ruleBody.end(), firstSets);
                        ans[s].insert(backFirstSet.begin(), backFirstSet.end());
                        if(backFirstSet.count(EmptyStringSymbol))
                        {
                            ans[s].erase(EmptyStringSymbol);
                            ans[s].insert(ans[A].begin(), ans[A].end());
                        }
                        if(ans[s].size() > oldSize) hasNew = 1;
                    }
                }
            }
        }
        if(!hasNew) break;
    }

    return ans;
}

























} // namespace lz





#endif /* LZ_COMPILER_GRAMMAR_DESIGN_H_ */























