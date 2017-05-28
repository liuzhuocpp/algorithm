/*
 * grammar_analyze.h
 *
 *  Created on: 2017年4月18日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_C_GRAMMAR_PARSER_H_
#define LZ_COMPILER_C_GRAMMAR_PARSER_H_




#include <lz/utility.h>


#include <lz/compiler/noterminal_proxy.h>
#include <lz/compiler/grammar.h>
#include <lz/compiler/slr1_grammar.h>
#include <lz/compiler/lr_grammar.h>

#include <lz/compiler/c/make_grammar_factory.h>
#include <lz/compiler/c/three_address_code.h>


namespace lz {


//template<typename OutStream, typename ErrorStream>
struct GrammarParser
{
//    GrammarParser(OutStream &outStream, ErrorStream &errorOfstream):
//        m_outStream(outStream), m_errorOfstream(errorOfstream){}

//    OutStream& outStream()
//    {
//        return m_outStream;
//    }
//
//    ErrorStream& errorOfstream()
//    {
//        return m_errorOfstream;
//    }
//
//    IdentifierTable& identifierTable()
//    {
//        return m_identifierTable;
//    }
//    ThreeAddressCode& codeTable()
//    {
//        return m_codeTable;
//    }
//    TypeTable& typeTable()
//    {
//        return m_typeTable;
//    }


private:
//    OutStream &m_outStream;
//    ErrorStream &m_errorOfstream;

//    IdentifierTable m_identifierTable;
//    ThreeAddressCode m_codeTable;
//    TypeTable m_typeTable;
//

//    outStream.open(outFileName, std::ofstream::out);
//    errorOfstream.open(errorFileName, std::ofstream::out);


    using P = Properties;
    using T = LexicalSymbol;

    using GF = GrammarFactory<T, P>;
    decltype(GF().g) g;
    decltype(GF().getIndexToNonterminalMap()) indexToNonterminalMap;
    decltype(GF().getIndexToTerminalMap()) indexToTerminalMap;
    decltype(GF().getTerminalToIndexMap()) terminalToIndexMap;


    using ExtendedTuple =
        decltype(extendGrammarAndConstructActionGotoMark(g, indexToNonterminalMap, indexToTerminalMap, terminalToIndexMap));

    std::tuple_element_t<1, ExtendedTuple> actionTableOption;
    std::tuple_element_t<2, ExtendedTuple> gotoFunction;
    std::tuple_element_t<3, ExtendedTuple> markNonterminalsMap;

public:

    GrammarParser()
    {
//        grammarInputData.outStream.open(outFileName, std::ofstream::out);
//        grammarInputData.errorStream.open(errorFileName, std::ofstream::out);

        GF gf = GrammarInput().gf;

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

//    void construct()
//    {
//
//
////        outStream.open(outFileName, std::ofstream::out);
////        errorOfstream.open(errorFileName, std::ofstream::out);
//
//
//
//
//    }

    template<typename Iterator>
    void parse(Iterator first, Iterator last, const std::string& outFileName, const std::string & errorFileName)
    {

        grammarInputData.outStream.open(outFileName, std::ofstream::out);
        grammarInputData.errorStream.open(errorFileName, std::ofstream::out);

        TemporaryVariableNumberGenerator::reset();
        grammarInputData.m_identifierTable.clear();
        grammarInputData.m_codeTable.clear();
        grammarInputData.m_typeTable.clear();

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

        std::cout << "Parse finished" << std::endl;


        grammarInputData.outStream << grammarInputData.m_codeTable;


    }


private:


};










} // namespace lz





#endif /* LZ_COMPILER_C_GRAMMAR_PARSER_H_ */
