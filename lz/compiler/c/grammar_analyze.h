/*
 * grammar_analyze.h
 *
 *  Created on: 2017年4月18日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_C_GRAMMAR_ANALYZE_H_
#define LZ_COMPILER_C_GRAMMAR_ANALYZE_H_




#include <lz/utility.h>


#include <lz/compiler/noterminal_proxy.h>
#include <lz/compiler/grammar.h>
#include <lz/compiler/slr1_grammar.h>
#include <lz/compiler/lr_grammar.h>

#include <lz/compiler/c/grammar_input.h>
#include <lz/compiler/c/intermediate_representation.h>
//
//
//#include <lz/compiler/c/lexical_analyze.h>
//#include <lz/compiler/c/identifier.h>

namespace lz {






template<typename InputIterator>
void grammarAnalyze(InputIterator first, InputIterator last)
{

    std::ofstream errorOfstream ("error.txt", std::ofstream::out);

    IRTable codeTable;
    auto generateCode = [&](std::string op, std::string arg1, std::string arg2, std::string res)
    {
        codeTable.push_back({op, arg1, arg2, res});
    };


    IdentifierTable identifierTable;
    auto gf = GrammarInput<decltype(generateCode), decltype(errorOfstream)>
        (identifierTable, generateCode, errorOfstream).gf;


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


    std::ofstream outText ("out.txt", std::ofstream::out);
    outText << codeTable;

//    outText.clear();
//    errorOfstream.clear();

}

} // namespace lz





#endif /* LZ_COMPILER_C_GRAMMAR_ANALYZE_H_ */
