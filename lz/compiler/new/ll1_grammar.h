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

using Table = std::map<std::pair<Symbol, Symbol>, Grammar::RuleDescriptor>;


Table constructLL1Table(const Grammar& g)
{
    std::map<std::pair<Symbol, Symbol>, Grammar::RuleDescriptor> ans;

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
    const std::map<typename std::iterator_traits<InputIterator>::value_type, Symbol> &translate,
    const Grammar& g,
    const std::vector<std::function< void(std::vector<P>, P&) > >& actions,
    const Table &table,
    Symbol startSymbol = 0)
{

    using RuleSymbolDescriptor = Grammar::RuleSymbolDescriptor;
    using RuleDescriptor = Grammar::RuleDescriptor;


    std::vector<RuleSymbolDescriptor > symbolStack;
    std::vector<P> propertyStack;

    symbolStack.push_back(RuleSymbolDescriptor(RuleDescriptor(-1, -1), -1)    );

    propertyStack.push_back(P());

    auto getRuleHeadAction = [&](RuleDescriptor rd) ->Symbol
    {
        const RuleBody& body = g.ruleBody(rd);
        if(!body.empty() && isAction(body[0]))
        {
            return body[0];
        }
        else return EmptyStringSymbol;
    };

    auto getSymbol = [&](RuleSymbolDescriptor rsd) ->Symbol
    {
        if(rsd.ruleDescriptor.A == -1) return startSymbol;
        else
        {
            const RuleBody& body = g.ruleBody(rsd.ruleDescriptor);
            return body[rsd.pos];
        }
    };

    auto getSymbolAction = [&](RuleSymbolDescriptor rsd) ->Symbol
    {
        if(rsd.ruleDescriptor.A == -1) return EmptyStringSymbol;
        else
        {

            const RuleBody& body = g.ruleBody(rsd.ruleDescriptor);
            if(rsd.pos + 1 < body.size() && isAction(body[rsd.pos + 1]))
            {
                return body[rsd.pos + 1];
            }
            else return EmptyStringSymbol;
        }

    };


    while(!symbolStack.empty())
    {

        RuleSymbolDescriptor rsd = symbolStack.back();
        Symbol s;
        s = getSymbol(rsd);

        Symbol terminal = EndTagSymbol;

        if(first != last)
        terminal = translate.at(*first);

        if(isTerminal(s))
        {
            if(s == terminal)
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
            if(table.count(std::make_pair(s, terminal)))
            {

                Symbol inheritAction = getSymbolAction(rsd);
                P sp;
                if(isAction(inheritAction))
                {
                    const RuleBody&ruleBody = g.ruleBody(rsd.ruleDescriptor);

                    int nonterminalsNumber =
                        g.getNonterminalsNumber(ruleBody.begin(), ruleBody.begin() + rsd.pos);
                    nonterminalsNumber ++;
                    std::vector<P> tmpStack(propertyStack.end() - nonterminalsNumber, propertyStack.end());
                    actions[inheritAction - ActionSymbolBegin](tmpStack, sp);
                }


                symbolStack.pop_back();

                // 加入综合属性
                propertyStack.push_back(sp);
                RuleDescriptor nextRd = table.at(std::make_pair(s, terminal));
                const RuleBody& nextRuleBody = g.ruleBody(nextRd);


                Symbol synthesizeAction = getRuleHeadAction(nextRd);

//                std::cout << "CO2222" << std::endl;

                if(synthesizeAction != EmptyStringSymbol)
                {
//                    std::cout << "CO-----" << std::endl;
                    symbolStack.push_back(RuleSymbolDescriptor(nextRd, 0));
                }
                else // will be error
                {

                }
//                std::cout << "CO233333" << std::endl;
//                for(auto it = nextRuleBody.end() - 1; it >= nextRuleBody.begin(); -- it)
                for(int i = int(nextRuleBody.size()) - 1; i >= 0; -- i)
                {
//                    std::cout << "***8888888888888" << std::endl;

//                    std::cout << nextRuleBody.size() << "sucker " << std::endl;

                    auto s = nextRuleBody[i];
//                    std::cout << "$$$$$" << std::endl;
                    if(!isAction(s))
                    {
//                        std::cout << "66666%%%%%%" << std::endl;
                        symbolStack.push_back(RuleSymbolDescriptor(nextRd, i));
                    }
//                    std::cout << "***9999999" << std::endl;
                }
//                std::cout << "CO-----" << std::endl;

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
//            std::cout << "jjj '" << std::endl;;
            const RuleBody& ruleBody = g.ruleBody(rsd.ruleDescriptor);
//            std::cout << "jjj '" << std::endl;;;
            int nonterminalsNumber = g.getNonterminalsNumber(ruleBody.begin(), ruleBody.end());
//            std::cout << "jjj ---'" << std::endl;;
            std::vector<P> tmpStack;
            while(nonterminalsNumber --)
            {
                tmpStack.push_back(propertyStack.back());
                propertyStack.pop_back();
            }

//            std::cout << "actions sizse: " << actions.size() << std::endl;
            if(s - ActionSymbolBegin < actions.size())
            {
//                std::cout << "HH" << std::endl;;
                actions[s - ActionSymbolBegin](tmpStack, propertyStack.back());
            }
            symbolStack.pop_back();

        }



    }

    std::cout  << propertyStack.back() << " " << std::endl;
}














}// namesapce lz

#endif /* LZ_COMPILER_NEW_LL1_GRAMMAR_H_ */
