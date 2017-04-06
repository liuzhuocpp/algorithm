/*
 * lr1_grammar.h
 *
 *  Created on: 2017年4月6日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_LR1_GRAMMAR_H_
#define LZ_COMPILER_LR1_GRAMMAR_H_

#include <lz/compiler/grammar_design.h>
#include <lz/compiler/grammar.h>
#include <lz/optional.h>
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
    const SymbolDescriptor& terminal() const
    { return std::get<2>(*this); }


    RuleDescriptor& ruleDescirptor()
    { return std::get<0>(*this); }
    RuleSymbolIterator& ruleSymbolIterator()
    { return std::get<1>(*this); }
    SymbolDescriptor& terminal()
    { return std::get<2>(*this); }


    ExtendItemDescriptor(RuleDescriptor rule, RuleSymbolIterator ruleSymbol, SymbolDescriptor s):
        Base(rule, ruleSymbol, s)
    { }

    ExtendItemDescriptor() = default;
};


template<typename Grammar, typename NonterminalMap, typename TerminalMap>
struct ExtendItemDescriptorForOutput
{
    using Item = ExtendItemDescriptor<Grammar>;
    using SymbolDescriptor = typename Grammar::SymbolDescriptor;


    Item item;
    const Grammar& g;
    NonterminalMap nonterminalMap;
    TerminalMap terminalMap;



    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
        operator<<(std::basic_ostream<Char, Traits>& os, const ExtendItemDescriptorForOutput& ido)
    {
        auto ruleSymbols = ido.g.ruleSymbols(ido.item.ruleDescirptor());

        SymbolForOutput<SymbolDescriptor, NonterminalMap, TerminalMap>
            head {*ruleSymbols.begin(), ido.nonterminalMap, ido.terminalMap};

        os << head <<"->";

        for(auto it = ++ruleSymbols.begin(); it != ruleSymbols.end(); ++ it)
        {
            if(ido.item.ruleSymbolIterator() == it)
            {
                os << ".";
                os << " " ;
            }
            os << SymbolForOutput<SymbolDescriptor, NonterminalMap, TerminalMap>
                {*it, ido.nonterminalMap, ido.terminalMap};
            os << " " ;
        }
        if(ido.item.ruleSymbolIterator() == ruleSymbols.end())
        {
            os << ".";
        }



        os << ", " <<
                SymbolForOutput<SymbolDescriptor, NonterminalMap, TerminalMap>
                    {ido.item.terminal(), ido.nonterminalMap, ido.terminalMap};

        return os;
    }


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
                    auto nextRuleSymbolIterator = item.ruleSymbolIterator();
                    ++nextRuleSymbolIterator;

                    std::set<SymbolDescriptor> firstSet =
                        calculateRuleBodyFirstSet(
                            nextRuleSymbolIterator,
                            g.ruleSymbols(item.ruleDescirptor()).end(),
                            firstSets);
                    if(firstSet.count(EmptyStringSymbol))
                    {
                        firstSet.erase(EmptyStringSymbol);
                        firstSet.insert(item.terminal());
                    }
                    for(SymbolDescriptor firstSymbol: firstSet)
                    {
                        ans.insert(ExtendItem(rule, ++g.ruleSymbols(rule).begin(), firstSymbol));
                    }
                }
            }
        }
        if(oldSize == ans.size()) break;
        itemSet.assign(ans.begin(), ans.end());
    }


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

    std::set<ExtendItem> J;
    for(ExtendItem item: I)
    {
        if(item.ruleSymbolIterator() != g.ruleSymbols(item.ruleDescirptor()).end() )
        {
            if(*item.ruleSymbolIterator() == X)
            {
                ++item.ruleSymbolIterator();
                J.insert(item);
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
            auto addSymbol = [&](SymbolDescriptor s) {

                auto next = calculateNextExtendItemSet(g, ans[i], s);
                if(!next.empty())
                {
                    if(!flag.count(next))
                    {
                        flag[next] = ans.size();
                        ans.push_back(next);
                    }
                    gotoTable[{flag[ans[i]], s}] = flag[next];

                }
            };

            for(SymbolDescriptor terminal: g.terminals())
            {
                addSymbol(terminal);
            }

            for(SymbolDescriptor nonterminal: g.nonterminals())
            {
                addSymbol(nonterminal);
            }
        }
        if(oldSize == ans.size()) break;
    }

    return std::make_pair(ans, gotoTable);
}

template<typename Grammar, typename ItemSets>
auto
calculateLR1ActionTable(
    const Grammar& g,
    typename Grammar::RuleDescriptor startRule,
    const ItemSets& itemSets,
    const std::map<std::pair<int, SymbolDescriptor>, int>& gotoFunction )
{

    using RuleSymbolIterator = typename Grammar::RuleSymbolIterator;
    using RuleDescriptor = typename Grammar::RuleDescriptor;
    using ExtendItem = ExtendItemDescriptor<Grammar>;
    using Action = ActionValue<RuleDescriptor>;


    std::map<std::pair<int, SymbolDescriptor>, Action > actionTable;
    using ActionTableOptinal = Optional<std::map<std::pair<int, SymbolDescriptor>, Action >>;
    ActionTableOptinal emptyOption = {};

    auto addAction = [&](auto i, SymbolDescriptor j, Action newAction) {



        if(actionTable.count({i, j}))
        {
            Action oldAction = actionTable.at({i, j});

            if(oldAction.type == ActionType::Shift && newAction.type == ActionType::Shift)
            {
                return true;
            }




            bool ans = true;

            if(oldAction.type == ActionType::Reduce && newAction.type == ActionType::Shift)
            {
            }
            else if(oldAction.type == ActionType::Shift && newAction.type == ActionType::Reduce)
            {
                std::swap(oldAction, newAction);
            }
            else ans = false;
            // oldAction 是reduce， newAction是shift
            if(ans)
            {
                if(g.getPriority(oldAction.rule, j) > 0)
                {
                    actionTable[{i, j}] = oldAction;
                }
                else if(g.getPriority(oldAction.rule, j) < 0)
                {
                    actionTable[{i, j}] = newAction;
                }
                else ans = false;
            }
            if(!ans)
            {
                std::cout << "Action1: " << oldAction << std::endl;
                std::cout << "Action2: " << newAction << std::endl;
                std::cout << "info: " << i << " " << j << std::endl;
                if(i == 5)
                {
                    actionTable[{i, j}] = newAction;
                    ans = true;
                }
            }


            return ans;

        }
        actionTable[std::make_pair(i, j)] = newAction;
        return true;
    };

    for(int i = 0; i < itemSets.size(); ++ i)
    {
        auto cntItemSet = itemSets[i];


        for(ExtendItem item: cntItemSet)
        {
            if(item.ruleSymbolIterator() == g.ruleSymbols(item.ruleDescirptor()).end())
            {
                if(item.ruleDescirptor() == startRule)
                {
                    if(!addAction(i, EndTagSymbol, ActionType::Accept))
                    {
                        return emptyOption;
                    }
                }
                else
                {
                    if(!addAction(i, item.terminal(),  Action(item.ruleDescirptor())))
                    {
                        return emptyOption;
                    }


                }
            }
            else if(isTerminal(*item.ruleSymbolIterator()))
            {
                if(!addAction(i, *item.ruleSymbolIterator(), Action(gotoFunction.at({i, *item.ruleSymbolIterator()}))))
                {
                    return emptyOption;
                }
            }
        }
    }


    return ActionTableOptinal(actionTable);

}









} // namespace lz





#endif /* LZ_COMPILER_LR1_GRAMMAR_H_ */
