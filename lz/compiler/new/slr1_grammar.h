/*
 * slr1_grammar.h
 *
 *  Created on: 2017年3月18日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_NEW_SLR1_GRAMMAR_H_
#define LZ_COMPILER_NEW_SLR1_GRAMMAR_H_


#include <lz/compiler/new/symbol.h>
namespace lz {


template<typename RuleDescriptor, typename RuleSymbolIterator>
struct ItemDescriptor:std::pair<RuleDescriptor, RuleSymbolIterator>
{
    using Base = std::pair<RuleDescriptor, RuleSymbolIterator>;
    RuleDescriptor ruleDescirptor() const
    {
        return Base::first;
    }
    RuleSymbolIterator ruleSymbolIterator() const
    {
        return Base::second;
    }

    ItemDescriptor(RuleDescriptor rule, RuleSymbolIterator ruleSymbol):
        Base(rule, ruleSymbol)
    {
    }

    ItemDescriptor() = default;

};

template<typename Grammar, typename T>
struct ItemDescriptorForOutput
{
    using Item = ItemDescriptor<typename Grammar::RuleDescriptor, typename Grammar::RuleSymbolIterator>;

    const Grammar& g;
    Item item;
    const std::vector<std::string>& nonterminalNames;
    const std::map<SymbolDescriptor, T>& ternimalNames;



    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
        operator<<(std::basic_ostream<Char, Traits>& os, const ItemDescriptorForOutput& ido)
    {
        auto ruleSymbols = ido.g.ruleSymbols(ido.item.ruleDescirptor());

        SymbolForOutput<T> head {*ruleSymbols.begin(), ido.nonterminalNames, ido.ternimalNames};

        os << head <<"->";

        for(auto it = ++ruleSymbols.begin(); it != ruleSymbols.end(); ++ it)
        {
            if(ido.item.ruleSymbolIterator() == it)
            {
                os << ".";
                os << " " ;
            }
            os << SymbolForOutput<T>{*it, ido.nonterminalNames, ido.ternimalNames};
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
    const std::vector<ItemDescriptor<typename Grammar::RuleDescriptor, typename Grammar::RuleSymbolIterator>> & items )
{

    using RuleSymbolIterator = typename Grammar::RuleSymbolIterator;
    using RuleDescriptor = typename Grammar::RuleDescriptor;
    using Item = ItemDescriptor<RuleDescriptor, RuleSymbolIterator>;

    std::vector<Item> nonkernelItems;
    std::set<SymbolDescriptor> vis;
    std::queue<SymbolDescriptor> q;

    for(auto item: items)
    {
        auto symbolRange = g.ruleSymbols(item.first);
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
    const std::vector<ItemDescriptor<typename Grammar::RuleDescriptor, typename Grammar::RuleSymbolIterator>> & itemSet,
    SymbolDescriptor target)
{


    auto nonkernelItems = calculateNonkernelItemSetClosure(g, itemSet);



    using Item = ItemDescriptor<typename Grammar::RuleDescriptor, typename Grammar::RuleSymbolIterator>;

    std::vector<Item> nextItemSet;

    auto addItem = [&](Item item) {
        auto symbolIterator = item.ruleSymbolIterator();
        if(symbolIterator != g.ruleSymbols(item.ruleDescirptor()).end()
            && *symbolIterator == target)
        {
            item.second ++;
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

    return nextItemSet;



}



template<typename Grammar>
auto
makeItemSets(const Grammar& g, typename Grammar::RuleDescriptor startRule)
{
    using RuleSymbolIterator = typename Grammar::RuleSymbolIterator;
    using RuleDescriptor = typename Grammar::RuleDescriptor;
    using Item = ItemDescriptor<RuleDescriptor, RuleSymbolIterator>;


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
















//enum ActionType
//{
//
//};
//struct Action
//{
//
//
//};

template<typename Grammar>
auto calculateSlr1ActionAndGoto(
    const Grammar& g,
    typename Grammar::RuleDescriptor startRule,
    const std::vector<ItemDescriptor< typename Grammar::RuleDescriptor, typename Grammar::RuleSymbolIterator>>& itemSet,
    const std::map<std::pair<int, SymbolDescriptor>, int> gotoFunction )
{

}


























}// namesapce lz

#endif /* LZ_COMPILER_NEW_SLR1_GRAMMAR_H_ */
