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

using Table = std::map<std::pair<SymbolDescriptor, SymbolDescriptor>, Grammar::RuleDescriptor>;


Table constructLL1Table(const Grammar& g)
{
    std::map<std::pair<SymbolDescriptor, SymbolDescriptor>, Grammar::RuleDescriptor> ans;

    auto firstSets = calculateFirstSets(g);
    auto followSets = calculateFollowSets(g, firstSets);

    auto addPair = [&](auto key, auto value){
        if(ans.count(key) && ans[key] != value) return 0;
        ans[key] =  value;
        return 1;
    };

    for(auto rule: g.rules())
    {
        auto head = g.ruleHead(rule);
        auto body = g.ruleBody(rule);



        for(auto firstS: calculateRuleBodyFirstSet(body.begin(), body.end(), firstSets))
        {
//            std::cout << "firstS: " << firstS << " " ;
            if(isEmptyString(firstS))
            {
                for(auto followS: followSets[head])
                {
                    if(!addPair(std::make_pair(head, followS), rule))
                    {
//                        std::cout << "SB" << std::endl;
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
//        std::cout << "----****" << std::endl;
    }

    return ans;
}

bool isLL1Grammar(const Grammar &g)
{
    return !constructLL1Table(g).empty();
}


template<typename InputIterator, typename P>
void parseLL1Grammar(
    InputIterator first,
    InputIterator last,
    const std::map<typename std::iterator_traits<InputIterator>::value_type, SymbolDescriptor> &translate,
    const Grammar& g,
    const std::vector<std::function< void(std::vector<P>, P&) > >& actions,
    const Table &table,
    SymbolDescriptor startSymbol = 0)
{

    using RuleSymbolDescriptor = Grammar::RuleSymbolDescriptor;
    using RuleDescriptor = Grammar::RuleDescriptor;


    std::vector<RuleSymbolDescriptor > symbolStack;
    std::vector<P> propertyStack;


//    symbolStack.push_back(RuleSymbolDescriptor(RuleDescriptor(-1, -1), -1)    );
    symbolStack.push_back( Grammar::nullRuleSymbol() );

    propertyStack.push_back(P());

    auto getRuleHeadAction = [&](RuleDescriptor rd) ->SymbolDescriptor
    {
        const RuleBody& body = g.ruleBody(rd);
        if(!body.empty() && isAction(body[0]))
        {
            return body[0];
        }
        else return EmptyStringSymbol;
    };

    auto getSymbol = [&](RuleSymbolDescriptor rsd) ->SymbolDescriptor
    {
        if(rsd.rule.head == -1) return startSymbol;
        else
        {
            const RuleBody& body = g.ruleBody(rsd.rule);
            return body[rsd.id];
        }
    };



    auto getSymbolAction = [&](RuleSymbolDescriptor rsd) ->SymbolDescriptor
    {
        if(rsd.rule.head == -1) return EmptyStringSymbol;
        else
        {

            const RuleBody& body = g.ruleBody(rsd.rule);
            if(rsd.id + 1 < body.size() && isAction(body[rsd.id + 1]))
            {
                return body[rsd.id + 1];
            }
            else return EmptyStringSymbol;
        }

    };


    while(!symbolStack.empty())
    {

        RuleSymbolDescriptor rsd = symbolStack.back();
        SymbolDescriptor s;
        if(rsd == Grammar::nullRuleSymbol()) s = startSymbol;
        else s = getSymbol(rsd);

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
                int inheritActionId = -1;
                if(rsd == Grammar::nullRuleSymbol())
                    inheritActionId = g.getActionId(rsd);
                P sp;

                if(inheritActionId != -1)
                {
                    const RuleBody&ruleBody = g.ruleBody(rsd.rule);

                    int nonterminalsNumber =
                        g.getNonterminalsNumber(ruleBody.begin(), ruleBody.begin() + rsd.id);
                    nonterminalsNumber ++;
                    std::vector<P> tmpStack(propertyStack.end() - nonterminalsNumber, propertyStack.end());
                    actions[inheritActionId](tmpStack, sp);
                }


                symbolStack.pop_back();

                // 加入综合属性
                propertyStack.push_back(sp);
                RuleDescriptor nextRd = table.at(std::make_pair(s, input));
                const RuleBody& nextRuleBody = g.ruleBody(nextRd);

                using RuleSymbolIterator = Grammar::RuleSymbolIterator;
                IteratorRange<RuleSymbolIterator> nextRule = g.ruleSymbols(nextRd);


                SymbolDescriptor synthesizeAction = getRuleHeadAction(nextRd);
                if(synthesizeAction != EmptyStringSymbol)
                {
                    symbolStack.push_back(RuleSymbolDescriptor(nextRd, 0));
                }
                else // will be error
                {

                }



                for(RuleSymbolIterator it = --nextRule.second; it != nextRule.first; -- it)
                {
                    symbolStack.push_back(*it);
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
            const RuleBody& ruleBody = g.ruleBody(rsd.rule);
            int nonterminalsNumber = g.getNonterminalsNumber(ruleBody.begin(), ruleBody.end());
            std::vector<P> tmpStack;
            while(nonterminalsNumber --)
            {
                tmpStack.push_back(propertyStack.back());
                propertyStack.pop_back();
            }
            if(s - ActionSymbolBegin < actions.size())
            {
                actions[s - ActionSymbolBegin](tmpStack, propertyStack.back());
            }
            symbolStack.pop_back();

        }

    }

    std::cout  << propertyStack.back() << " " << std::endl;
}














}// namesapce lz

#endif /* LZ_COMPILER_NEW_LL1_GRAMMAR_H_ */
