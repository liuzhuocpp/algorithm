/*
 * slr1_grammar.h
 *
 *  Created on: 2017年3月18日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_NEW_SLR1_GRAMMAR_H_
#define LZ_COMPILER_NEW_SLR1_GRAMMAR_H_


#include <lz/compiler/symbol.h>
#include <lz/compiler/grammar_design.h>
#include <lz/optional.h>

namespace lz {


template<typename Grammar>
struct ItemDescriptor: std::pair<typename Grammar::RuleDescriptor, typename Grammar::RuleSymbolIterator>
{
    using RuleDescriptor = typename Grammar::RuleDescriptor;
    using RuleSymbolIterator = typename Grammar::RuleSymbolIterator;

    using Base = std::pair<RuleDescriptor, RuleSymbolIterator>;
    const RuleDescriptor& ruleDescirptor() const
    {
        return Base::first;
    }
    const RuleSymbolIterator& ruleSymbolIterator() const
    {
        return Base::second;
    }

    RuleDescriptor& ruleDescirptor()
    {
        return Base::first;
    }
    RuleSymbolIterator& ruleSymbolIterator()
    {
        return Base::second;
    }

    ItemDescriptor(RuleDescriptor rule, RuleSymbolIterator ruleSymbol):
        Base(rule, ruleSymbol)
    {
    }

    ItemDescriptor() = default;

};

template<typename Grammar, typename NonterminalMap, typename TerminalMap>
struct ItemDescriptorForOutput
{
    using Item = ItemDescriptor<Grammar>;
    using SymbolDescriptor = typename Grammar::SymbolDescriptor;


    Item item;
    const Grammar& g;
    NonterminalMap nonterminalMap;
    TerminalMap terminalMap;



    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
        operator<<(std::basic_ostream<Char, Traits>& os, const ItemDescriptorForOutput& ido)
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
        return os;
    }


};





template<typename Grammar>
auto calculateNonkernelItemSetClosure(const Grammar &g,
    const std::vector<ItemDescriptor<Grammar>> & items )
{

    using RuleSymbolIterator = typename Grammar::RuleSymbolIterator;
    using RuleDescriptor = typename Grammar::RuleDescriptor;
    using Item = ItemDescriptor<Grammar>;

    std::vector<Item> nonkernelItems;
    std::set<SymbolDescriptor> vis;
    std::queue<SymbolDescriptor> q;

    for(Item item: items)
    {
        auto symbolRange = g.ruleSymbols(item.ruleDescirptor());
        if(item.ruleSymbolIterator() != symbolRange.end())
        {
            if(isNonterminal(*item.ruleSymbolIterator()))
            {
                q.push(*item.ruleSymbolIterator());
                vis.insert(*item.ruleSymbolIterator());
            }
        }
    }

    while(!q.empty())
    {
        auto u = q.front();
        q.pop();


        for(typename Grammar::RuleDescriptor outRule: g.outRules(u))
        {
            auto begin = ++ g.ruleSymbols(outRule).begin();

            nonkernelItems.push_back(Item(outRule, begin));


            if(begin != g.ruleSymbols(outRule).end() &&
                    isNonterminal(*begin) &&
                    !vis.count(*begin))
            {
                vis.insert(*begin);
                q.push(*begin);

            }
        }
    }


    return nonkernelItems;
}


template<typename Grammar>
auto calculateNextItemSet(const Grammar& g,
    const std::vector<ItemDescriptor<Grammar>> & itemSet,
    SymbolDescriptor target)
{
    auto nonkernelItems = calculateNonkernelItemSetClosure(g, itemSet);
    using Item = ItemDescriptor<Grammar>;
    std::vector<Item> nextItemSet;

    auto addItem = [&](Item item) {
        auto symbolIterator = item.ruleSymbolIterator();
        if(symbolIterator != g.ruleSymbols(item.ruleDescirptor()).end()
            && *symbolIterator == target)
        {
            item.ruleSymbolIterator() ++;
            nextItemSet.push_back(item);
        }
    };
    for(Item item: itemSet)
    {
        addItem(item);
    }
    for(Item item: nonkernelItems)
    {
        addItem(item);
    }
    std::sort(nextItemSet.begin(), nextItemSet.end());
    // 不能忘记unique！！
    auto newEnd = std::unique(nextItemSet.begin(), nextItemSet.end());
    nextItemSet.resize(newEnd - nextItemSet.begin());
    return nextItemSet;
}



template<typename Grammar>
auto makeItemSets(const Grammar& g, typename Grammar::RuleDescriptor startRule)
{
    using RuleSymbolIterator = typename Grammar::RuleSymbolIterator;
    using RuleDescriptor = typename Grammar::RuleDescriptor;
    using Item = ItemDescriptor<Grammar>;
    std::vector<Item> startItemSet(1, Item(startRule, ++g.ruleSymbols(startRule).begin()));
    std::vector<std::vector<Item>> itemSets;
    std::map<std::vector<Item>, int> itemSetToId; // itemSet -> int

    itemSets.push_back(startItemSet);
    itemSetToId[startItemSet] = 0;


    // key表示当前状态ID和当前遇到的符号，value表示下一个状态ID
    std::map<std::pair<int, SymbolDescriptor>, int> gotoFunction;

    // 存储状态ID
    std::queue<int> q;
    q.push(0);
    while(!q.empty())
    {
        std::vector<Item> cntItemSet = itemSets[q.front()];
        std::cout << "??" << " " << q.front() << " " << cntItemSet.size() <<  std::endl;

        q.pop();

        auto addSymbol = [&](SymbolDescriptor inputSymbol)
        {
            std::vector<Item> nextItemSet = calculateNextItemSet(g, cntItemSet, inputSymbol);

            if(nextItemSet.empty()) return ;
            if(!itemSetToId.count(nextItemSet))
            {
                itemSetToId[nextItemSet] = itemSets.size();
                q.push(itemSets.size());
                itemSets.push_back(nextItemSet);
            }
            gotoFunction[{itemSetToId[cntItemSet], inputSymbol}] = itemSetToId[nextItemSet];
        };


        for(SymbolDescriptor s: g.nonterminals())
        {

            addSymbol(s);
        }

        for(SymbolDescriptor inputSymbol : g.terminals())
        {
            addSymbol(inputSymbol);
        }
    }

    return std::make_tuple(itemSets, itemSetToId, gotoFunction);
}



template<typename Grammar>
auto makeAugmentedGrammar(Grammar &g, SymbolDescriptor start)
{
    SymbolDescriptor newStart = g.addNonterminal();
    std::vector<SymbolDescriptor> newRule{newStart, start};
    return g.addRule(newRule.begin(), newRule.end());
}
















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
};

template<typename Grammar, typename ItemSets>
auto
calculateSLR1ActionTable(
    const Grammar& g,
    typename Grammar::RuleDescriptor startRule,
    const ItemSets& itemSets,
    const std::map<std::pair<int, SymbolDescriptor>, int>& gotoFunction )
{

    using RuleSymbolIterator = typename Grammar::RuleSymbolIterator;
    using RuleDescriptor = typename Grammar::RuleDescriptor;
    using Item = ItemDescriptor<Grammar>;
    using Action = ActionValue<RuleDescriptor>;



    std::vector<std::set<SymbolDescriptor>> firstSets = calculateFirstSets(g);
    std::vector<std::set<SymbolDescriptor>> followSets =
        calculateFollowSets(g, firstSets, *g.ruleSymbols(startRule).begin() );


    std::map<std::pair<int, SymbolDescriptor>, Action > actionTable;
    using ActionTableOptinal = Optional<std::map<std::pair<int, SymbolDescriptor>, Action >>;
    ActionTableOptinal emptyOption = {};

    auto addAction = [&](auto i, SymbolDescriptor j, Action newAction) {

        if(actionTable.count({i, j}))
        {
            Action oldAction = actionTable.at({i, j});

            if(oldAction.type == ActionType::Reduce && newAction.type == ActionType::Shift)
            {
            }
            else if(oldAction.type == ActionType::Shift && newAction.type == ActionType::Reduce)
            {
                std::swap(oldAction, newAction);
            }
            else return false;
            // oldAction 是reduce， newAction是shift

            if(g.getPriority(oldAction.rule, j) > 0)
            {
                actionTable[{i, j}] = oldAction;
            }
            else if(g.getPriority(oldAction.rule, j) < 0)
            {
                actionTable[{i, j}] = newAction;
            }
            else return false;
            return true;

        }
        actionTable[std::make_pair(i, j)] = newAction;
        return true;
    };

    for(int i = 0; i < itemSets.size(); ++ i)
    {
        auto cntItemSet = itemSets[i];

        auto nonkernelItemSet = calculateNonkernelItemSetClosure(g, cntItemSet);

        cntItemSet.insert(cntItemSet.end(), nonkernelItemSet.begin(), nonkernelItemSet.end());

        for(Item item: cntItemSet)
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
                    SymbolDescriptor cntSymbol = *g.ruleSymbols(item.ruleDescirptor()).begin();


                    for(SymbolDescriptor followSymbol: followSets[cntSymbol])
                    {
                        if(!addAction(i, followSymbol, Action(item.ruleDescirptor())))
                        {
                            return emptyOption;
                        }
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



template<typename InputIterator, typename Grammar, typename ActionTable,
    typename GotoTable, typename IndexToTerminalMap>
void parseSLR1Grammar(
        InputIterator first,
        InputIterator last,
        const Grammar &g,
        const ActionTable& actionTable,
        const GotoTable& gotoTable,
        IndexToTerminalMap indexToTerminalMap

        )
{
    using RuleSymbolIterator = typename Grammar::RuleSymbolIterator;
    using RuleDescriptor = typename Grammar::RuleDescriptor;
    using Item = ItemDescriptor<Grammar>;
    using Action = ActionValue<RuleDescriptor>;
    using P = typename Grammar::NodeProperties;


    std::vector<int> stateStack;
    stateStack.push_back(0);
    while(!stateStack.empty())
    {

        SymbolDescriptor input = EndTagSymbol;
        if(first != last)
        {
            input = lz::makeTerminal(*first);
        }


        int u = stateStack.back();

        if(actionTable.count({u, input}) )
        {
            Action cnt = actionTable.at({u, input});
            if(cnt.type == ActionType::Accept)
            {
                std::cout << "Accept" << std::endl;
                break;
            }
            else if(cnt.type == ActionType::Shift)
            {
                stateStack.push_back(cnt.itemSetId);

                if(first == last)
                {
                    std::cout << "Shift: " << u << " " << "$" << " " << cnt.itemSetId << std::endl;
                }
                else
                {
                    std::cout << "Shift: " << u << " " << indexToTerminalMap[*first] << " " << cnt.itemSetId << std::endl;
                }

                first ++;
            }
            else if(cnt.type == ActionType::Reduce)
            {
                auto symbolRange = g.ruleSymbols(cnt.rule);
                int r = std::distance(++symbolRange.begin(), symbolRange.end());
                while(r --)
                {
                    stateStack.pop_back();
                }
                stateStack.push_back(gotoTable.at({stateStack.back(), *symbolRange.begin()}   ) );


                if(first == last)
                {
                    std::cout << "Reduce: " << u << " " << "$" << std::endl;
                }
                else
                {
                    std::cout << "Reduce: " << u << " " << indexToTerminalMap[*first] << std::endl;
                }

            }
            else
            {
                std::cout << "unkonwn ERROR" << std::endl;
            }


        }
        else
        {
            std::cout << "ERROR" << std::endl;
            return ;
        }
    }




}






















}// namesapce lz

#endif /* LZ_COMPILER_NEW_SLR1_GRAMMAR_H_ */
