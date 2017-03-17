/*
 * ll1_grammar.h
 *
 *  Created on: 2017年3月14日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_NEW_LL1_GRAMMAR_H_
#define LZ_COMPILER_NEW_LL1_GRAMMAR_H_


#include <lz/compiler/new/grammar.h>
#include <lz/compiler/new/grammar_design.h>
//#include <lz/new>
namespace lz{

template<typename Grammar>
auto constructLL1Table(const Grammar& g)
{
    std::map<std::pair<SymbolDescriptor, SymbolDescriptor>, typename Grammar::RuleDescriptor> ans;

    auto firstSets = calculateFirstSets(g);
    auto followSets = calculateFollowSets(g, firstSets);

    auto addPair = [&](auto key, auto value){
        if(ans.count(key) && ans[key] != value) return 0;
        ans[key] =  value;
        return 1;
    };

    for(auto rule: g.rules())
    {
        auto ruleSymbolRange = g.ruleSymbols(rule);
        auto head = *ruleSymbolRange.first++;


        for(auto firstS: calculateRuleBodyFirstSet(ruleSymbolRange.first, ruleSymbolRange.second, firstSets))
        {
            if(isEmptyString(firstS))
            {
                for(auto followS: followSets[head])
                {
                    if(!addPair(std::make_pair(head, followS), rule))
                    {
                        ans.clear();
                        return ans;
                    }
                }
            }
            else
            {
                if(!addPair(std::make_pair(head, firstS), rule))
                {
                    ans.clear();
                    return ans;
                }
            }
        }
    }

    return ans;
}

template<typename Grammar>
bool isLL1Grammar(const Grammar &g)
{
    return !constructLL1Table(g).empty();
}


template<typename InputIterator, typename Grammar>
void parseLL1Grammar(
    InputIterator first,
    InputIterator last,
    const std::map<typename std::iterator_traits<InputIterator>::value_type, SymbolDescriptor> &translate,
    const Grammar& g,
    const std::map<std::pair<SymbolDescriptor, SymbolDescriptor>, typename Grammar::RuleDescriptor> &table,
    SymbolDescriptor startSymbol = 0)
{

    using P = typename Grammar::NonterminalProperties;
    using RuleDescriptor = typename Grammar::RuleDescriptor;
    using RuleSymbolIterator = typename Grammar::RuleSymbolIterator;

    std::vector<std::tuple<SymbolDescriptor, int, SymbolDescriptor> > symbolStack;
    std::vector<P> propertyStack;


    symbolStack.push_back( std::make_tuple(startSymbol, 0, NullSymbol) );

    propertyStack.push_back(P());


    while(!symbolStack.empty())
    {
        int nonterminalsNumber;
        SymbolDescriptor s, sInheritActoin;
        std::tie(s, nonterminalsNumber, sInheritActoin) = symbolStack.back();

        SymbolDescriptor input = EndTagSymbol;

        if(first != last)
        input = translate.at(*first);

        if(isTerminal(s))
        {
            if(s == input)
            {
                symbolStack.pop_back();
                if(first == last)
                {
                    std::cout << "match finish" << std::endl;
                }
                else
                {
                    std::cout << "match: " << *first << " " << std::endl;
                    ++ first;
                }
            }
            else
            {
                if(first != last)
                {
                    std::cout << "error: stack top symbol is terminal "
                            << ", input character is "
                            << *first
                            << std::endl;
                }
                else
                {
                    std::cout << "error: stack top symbol is terminal"
                            << ", input character is end tag"
                            <<  std::endl;
                }
                return ;
            }
        }
        else if(isNonterminal(s))
        {
            if(table.count(std::make_pair(s, input)))
            {
                P sp;

                if(sInheritActoin != NullSymbol)
                {
                    std::vector<P> tmpStack(propertyStack.end() - nonterminalsNumber, propertyStack.end());
                    g.getActionFunc(sInheritActoin)(tmpStack, sp);
                }
                symbolStack.pop_back(); // 开始进行非终结符展开


                // 加入综合属性
                propertyStack.push_back(sp);
                RuleDescriptor nextRd = table.at(std::make_pair(s, input));

                IteratorRange<RuleSymbolIterator> nextRule = g.ruleSymbols(nextRd);


                SymbolDescriptor synthesizeAction = g.calculateAction(nextRd, g.ruleSymbols(nextRd).first);
                int nextNonterminalsNumber = 1;
                int nextRuleBodyBeginInSymbolStack = symbolStack.size();
                if(synthesizeAction != NullSymbol)
                {
                    symbolStack.push_back(std::make_tuple(synthesizeAction, 0, NullSymbol));
                    nextRuleBodyBeginInSymbolStack++;
                }
                else // will be error
                {

                }
                for(RuleSymbolIterator it = ++nextRule.first; it != nextRule.second; ++ it)
                {
                    symbolStack.push_back(std::make_tuple(*it, nextNonterminalsNumber, g.calculateAction(nextRd, it) ));
                    if(isNonterminal(*it)) nextNonterminalsNumber++;
                }

                std::reverse(symbolStack.begin() + nextRuleBodyBeginInSymbolStack, symbolStack.end());
                if(synthesizeAction != NullSymbol)
                {
                    std::get<1>(symbolStack[nextRuleBodyBeginInSymbolStack - 1]) = nextNonterminalsNumber - 1;
                }
            }
            else
            {
                if(first != last)
                {
                    std::cout << "error: stack top symbol is nonterminal "
                            << ", input character is "
                            << *first
                            << std::endl;
                }
                else
                {
                    std::cout << "error: stack top symbol is nonterminal"
                            << ", input character is end tag"
                            <<  std::endl;
                }

                return ;
            }
        }
        else if(isAction(s))
        {

            std::vector<P> tmpStack;
            while(nonterminalsNumber --)
            {
                tmpStack.push_back(propertyStack.back());
                propertyStack.pop_back();
            }

            g.getActionFunc(s)(tmpStack, propertyStack.back());
            symbolStack.pop_back();
        }

    }

    std::cout  << propertyStack.back() << " " << std::endl;
}














}// namesapce lz

#endif /* LZ_COMPILER_NEW_LL1_GRAMMAR_H_ */
