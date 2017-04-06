/*
 * lr_grammar.h
 *
 *  Created on: 2017年4月6日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_LR_GRAMMAR_H_
#define LZ_COMPILER_LR_GRAMMAR_H_



namespace lz {






template<typename InputIterator, typename Grammar, typename ActionTable,
    typename GotoTable,
    typename IndexToTerminalMap,
    typename IndexToNonterminalMap>
void parseSLR1Grammar(
        InputIterator first,
        InputIterator last,
        const Grammar &g,
        const ActionTable& actionTable,
        const GotoTable& gotoTable,
        IndexToTerminalMap indexToTerminalMap,
        IndexToNonterminalMap indexToNonterminalMap

        )
{
    using RuleSymbolIterator = typename Grammar::RuleSymbolIterator;
    using RuleDescriptor = typename Grammar::RuleDescriptor;
//    using Item = ItemDescriptor<Grammar>;
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


                std::cout << "             Rule is:" <<
                                            ruleOutput << std::endl;


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




} // namespace lz





#endif /* LZ_COMPILER_LR_GRAMMAR_H_ */
