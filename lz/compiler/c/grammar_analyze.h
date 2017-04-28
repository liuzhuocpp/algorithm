/*
 * grammar_analyze.h
 *
 *  Created on: 2017年4月18日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_C_GRAMMAR_ANALYZE_H_
#define LZ_COMPILER_C_GRAMMAR_ANALYZE_H_



#include <lz/compiler/c/grammar_input.h>
#include <lz/utility.h>


#include <lz/compiler/noterminal_proxy.h>
#include <lz/compiler/grammar.h>
#include <lz/compiler/slr1_grammar.h>
#include <lz/compiler/lr_grammar.h>
//
//
//#include <lz/compiler/c/lexical_analyze.h>
//#include <lz/compiler/c/identifier.h>

namespace lz {






template<typename InputIterator>
void grammarAnalyze(InputIterator first, InputIterator last)
{

    std::ofstream outText ("out.txt", std::ofstream::out);
    std::ofstream errorOfstream ("error.txt", std::ofstream::out);

    auto generateCode = [&](std::string op, std::string arg1, std::string arg2, std::string res)
    {
        outText << op << " " << arg1 << " " << arg2 << " " << res << std::endl;;
    };


    IdentifierTable identifierTable;
//    identifierTable.clear();
    auto gf = GrammarInput<decltype(generateCode), decltype(errorOfstream)>
        (identifierTable, generateCode, errorOfstream).gf;

//    auto gf = makeGrammarFactory(identifierTable, generateCode, errorOfstream);

//    auto gf = cgf.gf;






//    return ;


    auto g = gf.g;
    auto indexToNonterminalMap = gf.getIndexToNonterminalMap();
    auto indexToTerminalMap =  gf.getIndexToTerminalMap();
    auto terminalToIndexMap =  gf.getTerminalToIndexMap();







//    std::cout << GrammerForOutput<decltype(g), decltype(indexToNonterminalMap), decltype(indexToTerminalMap)>
//        {g, indexToNonterminalMap, indexToTerminalMap} ;


    auto [newG, actionTableOption, gotoFunction, markNonterminalsMap]
           = extendGrammarAndConstructActionGotoMark(g, indexToNonterminalMap, indexToTerminalMap, terminalToIndexMap);






    cout << std::string(2, '\n') << "Begin parsing...\n\n";
    auto ans = parseLRGrammar(
            first,
            last,
            newG,
            actionTableOption.value(),
            gotoFunction,
            markNonterminalsMap,
            terminalToIndexMap,
            indexToTerminalMap,
            indexToNonterminalMap);



    std::cout << "identifierTable: \n" << newLineRangeSplitter  << identifierTable << std::endl;



    outText.clear();
    errorOfstream.clear();

}

} // namespace lz





#endif /* LZ_COMPILER_C_GRAMMAR_ANALYZE_H_ */
