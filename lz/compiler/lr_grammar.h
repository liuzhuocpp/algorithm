/*
 * lr_grammar.h
 *
 *  Created on: 2017年4月6日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_LR_GRAMMAR_H_
#define LZ_COMPILER_LR_GRAMMAR_H_



namespace lz {




template<typename Grammar>
auto makeAugmentedGrammar(Grammar &g, SymbolDescriptor start)
{
    SymbolDescriptor newStart = g.addNonterminal();
    std::vector<SymbolDescriptor> newRule{newStart, start};
    return g.addRule(newRule.begin(), newRule.end());
}

template<typename Grammar>
auto transformInteritSemanticRuleGrammar(const Grammar& g)
{
    using RuleDescriptor = typename Grammar::RuleDescriptor;
    Grammar ans;
    for(auto nonterminal: g.nonterminals())
    {
        ans.addNonterminal(nonterminal);
    }

    for(auto terminal: g.nonterminals())
    {
        ans.addTerminal(terminal);
    }
    for(auto semanticRule: g.semanticRules())
    {
        ans.addSemanticRuleFunc(g.getSemanticRuleFunc(semanticRule));
    }


    for(RuleDescriptor rule: g.rules())
    {
        auto symbolRange = g.ruleSymbols(rule);

        std::vector<SymbolDescriptor> newRule;
        newRule.push_back(*symbolRange.begin());
        SymbolDescriptor semanticRule = g.calculateSemanticRule(rule, symbolRange.begin());
        if(semanticRule != NullSymbol)
        {
            newRule.push_back(semanticRule);
        }

        for(auto it = ++symbolRange.begin(); it != symbolRange.end(); ++ it)
        {
            semanticRule = g.calculateSemanticRule(rule, it);
            if(semanticRule != NullSymbol)
            {
                SymbolDescriptor M[2];
                M[0] = ans.addNonterminal();
                M[1] = semanticRule;
                ans.addRule(M, M + 2);
                newRule.push_back(M[0]);
            }
            newRule.push_back(*it);
        }
        ans.addRule(newRule.begin(), newRule.end());
    }
    return ans;
}



template<typename InputIterator, typename Grammar, typename ActionTable,
    typename GotoTable,
    typename IndexToTerminalMap,
    typename IndexToNonterminalMap>
auto parseLRGrammar(
        InputIterator first,
        InputIterator last,
        const Grammar &g,
        const ActionTable& actionTable,
        const GotoTable& gotoTable,
//        bool hasInheritSemanticRule,
        IndexToTerminalMap indexToTerminalMap,
        IndexToNonterminalMap indexToNonterminalMap

        )
{
    using RuleSymbolIterator = typename Grammar::RuleSymbolIterator;
    using RuleDescriptor = typename Grammar::RuleDescriptor;
    using Action = ActionValue<RuleDescriptor>;
    using P = typename Grammar::NodeProperties;

    std::vector<int> stateStack;
    stateStack.push_back(0);
    std::vector<P> propertyStack;
    propertyStack.push_back(P());

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
                int r = 0;
                std::vector<P> tmpStack;
                auto semanticRule = g.calculateSemanticRule(cnt.rule, symbolRange.begin());
                typename Grammar::SemanticRuleFunc semanticFunc;
                if(semanticRule != NullSymbol)
                {
                    semanticFunc = g.getSemanticRuleFunc(semanticRule);
                }
                for(auto it = ++symbolRange.begin(); it != symbolRange.end(); ++ it)
                {
                    stateStack.pop_back();
                    if(isNonterminal(*it))
                    {
                        tmpStack.push_back(propertyStack.back());
                        propertyStack.pop_back();
                    }
                    r++;
                }
                stateStack.push_back(gotoTable.at({stateStack.back(), *symbolRange.begin()}) );

                if(semanticFunc)
                {

//                    if(r == 0)
//                    {
//                        P ans;
//
//                    }
//                    else
                    {
                        P ans;
                        std::reverse(tmpStack.begin(), tmpStack.end());
                        semanticFunc(tmpStack, ans);
                        propertyStack.push_back(ans);
                    }


                }


                auto ruleSymbolRange = g.ruleSymbols(cnt.rule);
                auto ruleOutput = RuleForOutput<decltype(ruleSymbolRange.begin()), IndexToNonterminalMap, IndexToTerminalMap>
                    {ruleSymbolRange.begin(), ruleSymbolRange.end(), indexToNonterminalMap, indexToTerminalMap };

                if(first == last)
                {
                    std::cout << "Reduce: " << u << " " << "$" ;
                }
                else
                {
                    std::cout << "Reduce: " << u << " " << indexToTerminalMap[*first];
                }
                std::cout << "             Rule is:" << ruleOutput << std::endl;
            }
            else
            {
                std::cout << "unkonwn ERROR" << std::endl;
            }


        }
        else
        {
            std::cout << "ERROR" << std::endl;
            break;
        }
    }

    return propertyStack.back();



}




} // namespace lz





#endif /* LZ_COMPILER_LR_GRAMMAR_H_ */
