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
#include <lz/compiler/grammar.h>

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
//        std::cout << "??" << " " << q.front() << " " << cntItemSet.size() <<  std::endl;

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



















template<typename Grammar, typename ItemSets, typename IndexToNonterminalMap, typename IndexToTerminalMap>

auto
calculateSLR1ActionTable(
    const Grammar& g,
    typename Grammar::RuleDescriptor startRule,
    const ItemSets& itemSets,
    const std::map<std::pair<int, SymbolDescriptor>, int>& gotoFunction,
    IndexToNonterminalMap indexToNonterminalMap,
    IndexToTerminalMap indexToTerminalMap)
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

            bool ans = true;

            if(oldAction.type == ActionType::Reduce && newAction.type == ActionType::Shift)
            {
            }
            else if(oldAction.type == ActionType::Shift && newAction.type == ActionType::Reduce)
            {
                std::swap(oldAction, newAction);
            }
            else if(oldAction.type == ActionType::Shift && newAction.type == ActionType::Shift)
            {
                return true;
            }

            else
            {
                if(oldAction.rule == newAction.rule) return true;
                ans = false;
            }
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
                std::cout << "confict type: " << oldAction << " " << newAction <<  std::endl;

                std::cout << "confict content: " << std::endl;
                std::cout << GrammarRuleForOutput<Grammar,IndexToNonterminalMap, IndexToTerminalMap > {
                    g, oldAction.rule, indexToNonterminalMap, indexToTerminalMap } << std::endl;

                if(newAction.type == ActionType::Shift) // 移入规约冲突
                {
                    std::cout << "terminal: " << indexToTerminalMap[lz::getTerminalId(j)];
                }
                if(newAction.type == ActionType::Reduce) // 移入规约冲突
                {
                    std::cout << GrammarRuleForOutput<Grammar,IndexToNonterminalMap, IndexToTerminalMap > {
                        g, newAction.rule, indexToNonterminalMap, indexToTerminalMap } << std::endl;
                }

                std::cout << "\n\n";

//                std::cout << "info: " << i << " " << j << std::endl;
                assert(0);

            }





            return ans;

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




template<typename Grammar, typename IndexToNonterminalMap, typename IndexToTerminalMap, typename TerminalToIndexMap>
auto extendGrammarAndConstructActionGotoMark(
    Grammar g,
    IndexToNonterminalMap indexToNonterminalMap,
    IndexToTerminalMap indexToTerminalMap,
    TerminalToIndexMap terminalToIndexMap,
    typename Grammar::SymbolDescriptor startSymbol = 0,
    bool outputNonkernelItem = true )
{
    using P = typename Grammar::NodeProperties;
    using SymbolDescriptor = typename Grammar::SymbolDescriptor;

    auto startRule = makeAugmentedGrammar(g, startSymbol);
    SymbolDescriptor newStartSymbol = *g.ruleSymbols(startRule).begin();

    indexToNonterminalMap[getNonterminalId(newStartSymbol)] = indexToNonterminalMap[getNonterminalId(startSymbol)] + "'";

    auto transformAns = transformInteritSemanticRuleGrammar(g);
    g = std::get<0>(transformAns);
    auto markNonterminalsMap = std::get<1>(transformAns);

    int markCounter = 0;
    for(auto mark: markNonterminalsMap)
    {
        indexToNonterminalMap[getNonterminalId(mark.first)] ="M" + std::to_string(markCounter ++);
    }






    std::cout << GrammerForOutput<decltype(g), decltype(indexToNonterminalMap), decltype(indexToTerminalMap)>
        {g, indexToNonterminalMap, indexToTerminalMap} ;


    auto result = makeItemSets(g, startRule);

    auto itemSets = std::get<0>(result);
    auto gotoFunction = std::get<2>(result);

    std::cout << std::string(100, '*') << std::endl;
    std::cout << "Item set number: " << itemSets.size() << "\n\n";
    int counter = 0;
    for(auto itemSet: itemSets)
    {
        std::cout << "I" << counter ++ << ": " << std::endl;
        for(auto item: itemSet)
        {
            std::cout << ItemDescriptorForOutput<Grammar, decltype(indexToNonterminalMap), decltype(indexToTerminalMap)>
                {item, g, indexToNonterminalMap, indexToTerminalMap} << std::endl;
        }

        if(outputNonkernelItem)
        {
            std::cout << std::string(10, '~') << std::endl;
            for(auto item: calculateNonkernelItemSetClosure(g, itemSet))
            {
                std::cout << ItemDescriptorForOutput<Grammar, decltype(indexToNonterminalMap), decltype(indexToTerminalMap) >
                    {item, g, indexToNonterminalMap, indexToTerminalMap} << std::endl;
            }

        }
        std::cout << std::string(20, '-') << std::string(3, '\n');
    }


    std::cout << std::string(100, '*') << "gotoFunction" << std::endl;

    for(auto _pair: gotoFunction)
    {
        std::cout << _pair.first.first
            << " "
            << SymbolForOutput<SymbolDescriptor, decltype(indexToNonterminalMap), decltype(indexToTerminalMap)>
                {_pair.first.second, indexToNonterminalMap, indexToTerminalMap} << " "
            << _pair.second << std::endl;
    }


    auto actionTableOption = calculateSLR1ActionTable(g, startRule, itemSets, gotoFunction,

            indexToNonterminalMap,
            indexToTerminalMap);

    std::cout << "Is SLR(1) grammar? " << std::boolalpha << actionTableOption.hasValue() << std::endl;
    if(actionTableOption.hasValue())
    {
        std::cout << std::string(100, '*') << " actionTable" << std::endl;
        for(auto _pair: actionTableOption.value())
        {

            std::cout << _pair.first.first << " " <<
                    SymbolForOutput<SymbolDescriptor, decltype(indexToNonterminalMap), decltype(indexToTerminalMap)>
                        {_pair.first.second, indexToNonterminalMap, indexToTerminalMap}
                    << " " << std::endl;
        }
    }


    return std::make_tuple(g, actionTableOption, gotoFunction, markNonterminalsMap);

}



















}// namesapce lz

#endif /* LZ_COMPILER_NEW_SLR1_GRAMMAR_H_ */
