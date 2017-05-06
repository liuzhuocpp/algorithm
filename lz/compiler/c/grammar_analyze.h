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
#include <lz/compiler/c/three_address_instruction.h>
//
//
//#include <lz/compiler/c/lexical_analyze.h>
//#include <lz/compiler/c/identifier.h>

namespace lz {


//template<typename InputIterator, typename ErrorStream>
//auto constructGrammar(IdentifierTable& identifierTable, IRTable & codeTable, ErrorStream &errorOfstream)
//{
//    auto gf = GrammarInput<decltype(codeTable), decltype(errorOfstream)>
//        (identifierTable, codeTable, errorOfstream).gf;
//
//
//    auto g = gf.g;
//    auto indexToNonterminalMap = gf.getIndexToNonterminalMap();
//    auto indexToTerminalMap =  gf.getIndexToTerminalMap();
//    auto terminalToIndexMap =  gf.getTerminalToIndexMap();
//
//
//
//
//
//
//
////    std::cout << GrammerForOutput<decltype(g), decltype(indexToNonterminalMap), decltype(indexToTerminalMap)>
////        {g, indexToNonterminalMap, indexToTerminalMap} ;
//
//
//   auto ans = extendGrammarAndConstructActionGotoMark(g, indexToNonterminalMap, indexToTerminalMap, terminalToIndexMap);
//   return std::tuple_cat( ans,
//           terminalToIndexMap,
//           indexToTerminalMap,
//           indexToNonterminalMap);
//
//
//}
//
//template<typename InputIterator,
//    typename G,
//    typename ActionTableOptional,
//    typename Goto,
//    typename MarkNonterminalsMap,
//    typename TerminalToIndexMap,
//    typename IndexToTerminalMap,
//    typename IndexToNonterminalMap
//
//    >
//
//void grammarAnalyze(
//    InputIterator first,
//    InputIterator last,
//    G g,
//    ActionTableOptional actionTableOption,
//    Goto gotoFunction,
//    MarkNonterminalsMap markNonterminalsMap,
//    TerminalToIndexMap terminalToIndexMap,
//    IndexToTerminalMap indexToTerminalMap,
//    IndexToNonterminalMap indexToNonterminalMap   )
//{
//    cout << std::string(2, '\n') << "Begin parsing...\n\n";
//    auto ans = parseLRGrammar(
//            first,
//            last,
//            g,
//            actionTableOption.value(),
//            gotoFunction,
//            markNonterminalsMap,
//            terminalToIndexMap,
//            indexToTerminalMap,
//            indexToNonterminalMap);
//
//}

template<typename OutStream, typename ErrorStream>
struct GrammarParser
{

    OutStream& outStream;
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
//         gf = GrammarInput<ErrorStream>
//            (identifierTable, codeTable, errorOfstream).gf;
         gf = makeGrammarFactory(identifierTable, codeTable, errorOfstream);

//        auto tmp_g = gf.g;
        indexToNonterminalMap = gf.getIndexToNonterminalMap();
        indexToTerminalMap =  gf.getIndexToTerminalMap();
        terminalToIndexMap =  gf.getTerminalToIndexMap();

//        std::cout << "IIIIIIII" << terminalToIndexMap[LexicalSymbol::Category::Int] << std::endl;

        auto ansTuple = extendGrammarAndConstructActionGotoMark(
            gf.g,
            indexToNonterminalMap,
            indexToTerminalMap,
            terminalToIndexMap);

//        std::cout << "yyyyyyyyyyyyy" << terminalToIndexMap[LexicalSymbol::Category::Int] << std::endl;


        g = std::get<0>(ansTuple);
        actionTableOption = std::get<1>(ansTuple);
        gotoFunction = std::get<2>(ansTuple);
        markNonterminalsMap = std::get<3>(ansTuple);

//        std::cout << "iooooooooooooooo" << terminalToIndexMap[LexicalSymbol::Category::Int] << std::endl;

    }

    template<typename Iterator>
    void parse(Iterator first, Iterator last)
    {


//        std::cout << "IIIIIIII" << terminalToIndexMap[LexicalSymbol::Category::Int] << std::endl;


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



        std::cout << "identifierTable: \n" << newLineRangeSplitter  << identifierTable << std::endl;

////        std::cout << "code Table: " << codeTable << std::endl;
//
//
//        std::cout << "code in grammar parasr : " << codeTable.size() <<
//                " " <<
//                codeTable <<  " " << (std::size_t)&codeTable <<  std::endl;
////        outStream << "JJJooooooooooooJ" << std::endl;

        outStream << codeTable;


    }




    //    auto [newG, actionTableOption, gotoFunction, markNonterminalsMap]
    //           = extendGrammarAndConstructActionGotoMark(g, indexToNonterminalMap, indexToTerminalMap, terminalToIndexMap);

private:


};


//template<typename InputIterator, typename OutStream, typename ErrorStream>
//void grammarAnalyze(InputIterator first, InputIterator last, OutStream& outText, ErrorStream &errorOfstream )
//{
//
//    TemporaryVariableNumberGenerator::reset();
//
//    IRTable codeTable;
//
//
//    IdentifierTable identifierTable;
//
//    auto gf = GrammarInput<decltype(codeTable), decltype(errorOfstream)>
//        (identifierTable, codeTable, errorOfstream).gf;
//
//
//    auto g = gf.g;
//    auto indexToNonterminalMap = gf.getIndexToNonterminalMap();
//    auto indexToTerminalMap =  gf.getIndexToTerminalMap();
//    auto terminalToIndexMap =  gf.getTerminalToIndexMap();
//
//
//
//
//
//
//
////    std::cout << GrammerForOutput<decltype(g), decltype(indexToNonterminalMap), decltype(indexToTerminalMap)>
////        {g, indexToNonterminalMap, indexToTerminalMap} ;
//
//
//    auto [newG, actionTableOption, gotoFunction, markNonterminalsMap]
//           = extendGrammarAndConstructActionGotoMark(g, indexToNonterminalMap, indexToTerminalMap, terminalToIndexMap);
//
//
//
//
//
//    cout << std::string(2, '\n') << "Begin parsing...\n\n";
//    auto ans = parseLRGrammar(
//            first,
//            last,
//            newG,
//            actionTableOption.value(),
//            gotoFunction,
//            markNonterminalsMap,
//            terminalToIndexMap,
//            indexToTerminalMap,
//            indexToNonterminalMap);
//
//
//
//    std::cout << "identifierTable: \n" << newLineRangeSplitter  << identifierTable << std::endl;
//
//
////    std::ofstream outText ("out.txt", std::ofstream::out);
//    outText << codeTable;
//
////    outText.clear();
////    errorOfstream.clear();
//
//}

} // namespace lz





#endif /* LZ_COMPILER_C_GRAMMAR_ANALYZE_H_ */
