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

#include <lz/compiler/c/make_grammar_factory.h>
#include <lz/compiler/c/three_address_code.h>


namespace lz {


template<typename OutStream, typename ErrorStream>
struct GrammarParser
{
    GrammarParser(OutStream &outStream, ErrorStream &errorOfstream):
        outStream(outStream), errorOfstream(errorOfstream){}

    OutStream &outStream;

    ErrorStream &errorOfstream;

    using P = Properties;
    using T = LexicalSymbol;

    using GF = GrammarFactory<T, P>;
    decltype(GF()) gf;
    decltype(GF().g) g;
    decltype(GF().getIndexToNonterminalMap()) indexToNonterminalMap;
    decltype(GF().getIndexToTerminalMap()) indexToTerminalMap;
    decltype(GF().getTerminalToIndexMap()) terminalToIndexMap;


    using ExtendedTuple =
        decltype(extendGrammarAndConstructActionGotoMark(g, indexToNonterminalMap, indexToTerminalMap, terminalToIndexMap));

    std::tuple_element_t<1, ExtendedTuple> actionTableOption;
    std::tuple_element_t<2, ExtendedTuple> gotoFunction;
    std::tuple_element_t<3, ExtendedTuple> markNonterminalsMap;

    IdentifierTable identifierTable;
    ThreeAddressCode codeTable;
    void construct()
    {
        gf = makeGrammarFactory(identifierTable, codeTable, errorOfstream);

        indexToNonterminalMap = gf.getIndexToNonterminalMap();
        indexToTerminalMap =  gf.getIndexToTerminalMap();
        terminalToIndexMap =  gf.getTerminalToIndexMap();


        auto ansTuple = extendGrammarAndConstructActionGotoMark(
            gf.g,
            indexToNonterminalMap,
            indexToTerminalMap,
            terminalToIndexMap);
        g = std::get<0>(ansTuple);
        actionTableOption = std::get<1>(ansTuple);
        gotoFunction = std::get<2>(ansTuple);
        markNonterminalsMap = std::get<3>(ansTuple);


    }

    template<typename Iterator>
    void parse(Iterator first, Iterator last)
    {




        TemporaryVariableNumberGenerator::reset();
        identifierTable.clear();
        codeTable.clear();
        cout << std::string(2, '\n') << "Begin parsing...\n\n";


        auto ans = parseLRGrammar(
                first,
                last,
                g,
                actionTableOption.value(),
                gotoFunction,
                markNonterminalsMap,
                terminalToIndexMap,
                indexToTerminalMap,
                indexToNonterminalMap);




        outStream << codeTable;


    }


private:


};










} // namespace lz





#endif /* LZ_COMPILER_C_GRAMMAR_ANALYZE_H_ */
