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


#include <lz/compiler/c/lexical_analyze.h>
#include <lz/compiler/c/identifier.h>
#include <lz/compiler/c/properties.h>

namespace lz {



std::string getTemporaryVariableName()
{
    static int tempId = 0;

    return "$" + std::to_string(tempId++);
}
std::string getVariableName(int i)
{
    for(auto it : identifierTable)
    {
        if(it.second == i) return it.first.name; // 目前先返回变量的真实的identifier，便于debug

    }
    assert(0);
    return "";

//    return "a" + std::to_string(i);
}





template<typename InputIterator>
void grammarAnalyze(InputIterator first, InputIterator last)
{

    std::ofstream outText ("out.txt", std::ofstream::out);
    std::ofstream errorOfstream ("error.txt", std::ofstream::out);

    auto generateCode = [&](std::string op, std::string arg1, std::string arg2, std::string res)
    {
        outText << op << " " << arg1 << " " << arg2 << " " << res << std::endl;;
    };



    identifierTable.clear();

    using P = Properties;
    using T = LexicalSymbol;


    auto solveArithmeticOperator = [&](std::string op, auto v, P &o) {
        o.addr = getTemporaryVariableName();
        generateCode(op, v[1].addr, v[3].addr, o.addr);
    };

    auto checkVariableDeclare = [&](std::string variable) {
        auto it = identifierTable.find(variable);
        if(it == identifierTable.end())
        {
            errorOfstream << "\"" << variable << "\" was not declare \n";
            return -1;
        }
        else
            return it->second;
    };








    NonterminalProxy<T, P>
        LZ_NONTERMINAL_PROXY(program),
        LZ_NONTERMINAL_PROXY(declare),
        LZ_NONTERMINAL_PROXY(statement),
        LZ_NONTERMINAL_PROXY(expression),
//        LZ_NONTERMINAL_PROXY(subexpression),
        LZ_NONTERMINAL_PROXY(baseType),
        LZ_NONTERMINAL_PROXY(operateExpression);



    GrammarFactory<T, P> gf(program);

    program = declare >> program;
    program = statement >> program;
    program = declare;
    program = statement;

    declare = baseType >> LexicalSymbol::Type::Identifier >> ";" >>
        [&](auto v, P& o) {
            insertIdentifierTable(v[1].type, v[2].addr);
        };

    baseType = "int" >>
        [&](auto v, P&o) {
            o.type = Type::Category::Int;
        };

    baseType = "float" >>
        [&](auto v, P&o) {
            o.type = Type::Category::Float;
        };

    statement = expression >> ";";


//    expression = subexpression >> ";";

    expression = eps >> LexicalSymbol::Type::Identifier >> "=" >> expression >>
        [&](auto v, P&o) {

            if(auto checkId = checkVariableDeclare(v[1].addr); checkId != -1)
            {
                generateCode("=", getVariableName(checkId), v[3].addr, "");
            }

        };

    expression = operateExpression >>
        [&](auto v, P&o) {
            o.addr = v[1].addr;
        };


    operateExpression = operateExpression >> "+" >> operateExpression >>
        [&](auto v, P&o) {

            solveArithmeticOperator("+", v, o);
        } > "+" > "-" < "*" < "/";

    operateExpression = operateExpression >> "-" >> operateExpression >>
        [&](auto v, P&o) {
            solveArithmeticOperator("-", v, o);
        } > "+" > "-" < "*" < "/";

    operateExpression = operateExpression >> "*" >> operateExpression >>
        [&](auto v, P&o) {
            solveArithmeticOperator("*", v, o);
        } > "+" > "-" > "*" > "/";

    operateExpression = operateExpression >> "/" >> operateExpression >>
        [&](auto v, P&o) {
            solveArithmeticOperator("/", v, o);
        } > "+" > "-" > "*" > "/";

    operateExpression = "+" >> operateExpression  >>
        [&](auto v, P&o) {
            o.addr = getTemporaryVariableName();
            generateCode("plus", v[2].addr, "", o.addr);

        } > "+" > "-" > "*" > "/";

    operateExpression = "-" >> operateExpression >>
        [&](auto v, P&o) {
            o.addr = getTemporaryVariableName();
            generateCode("minus", v[2].addr, "", o.addr);
        } > "+" > "-" > "*" > "/";

    operateExpression = "(" >> operateExpression >> ")"  >>
        [&](auto v, P&o) {
            o.addr = v[2].addr;
        };


    operateExpression = LexicalSymbol::Type::Integer >>
        [&](auto v, P&o) {
            o.addr = v[1].addr;
        };

    operateExpression = LexicalSymbol::Type::Identifier >>
        [&](auto v, P&o) {
            std::string name = v[1].addr;

            if(auto checkId = checkVariableDeclare(v[1].addr); checkId != -1)
            {
                o.addr = getVariableName(checkId);
            }
        };






    Grammar<P> g = gf.g;
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






    outText.clear();
    errorOfstream.clear();

}

} // namespace lz





#endif /* LZ_COMPILER_C_GRAMMAR_ANALYZE_H_ */
