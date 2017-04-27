/*
 * make_grammar_factory.h
 *
 *  Created on: 2017年4月27日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_C_MAKE_GRAMMAR_FACTORY_H_
#define LZ_COMPILER_C_MAKE_GRAMMAR_FACTORY_H_

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



template<typename GenerateCode, typename ErrorOfstream>
auto makeGrammarFactory(const GenerateCode& generateCode, ErrorOfstream& errorOfstream)
{
    using P = Properties;
    using T = LexicalSymbol;
    using PIt = std::vector<P>::iterator;


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
        LZ_NONTERMINAL_PROXY(typeDeclare),
        LZ_NONTERMINAL_PROXY(arrayDeclare),
        LZ_NONTERMINAL_PROXY(baseTypeDeclare),

        LZ_NONTERMINAL_PROXY(statement),

        LZ_NONTERMINAL_PROXY(expression)
//        LZ_NONTERMINAL_PROXY(operateExpression)
        ;



    GrammarFactory<T, P> gf(program);

    program = declare >> program;
    program = statement >> program;
    program = declare;
    program = statement;

    declare = typeDeclare >> LexicalSymbol::Type::Identifier >> ";" >>
        [&](auto v, P& o) {
            insertIdentifierTable(v[1].type, v[2].addr);
        };
    typeDeclare = baseTypeDeclare >> arrayDeclare >>
        [&](PIt v, P &o)
        {
            o.type.category = v[1].type.category;
            o.type.arrayDimensions = v[2].arrayDimensions;
        };
    arrayDeclare = eps >> "[" >> LexicalSymbol::Type::Integer >> "]" >> arrayDeclare >>
        [&](PIt v, P& o) {
            o.arrayDimensions = v[4].arrayDimensions;
            o.arrayDimensions.push_back(std::stoi(v[1].addr));
        };

    arrayDeclare = eps >>
        [&](PIt v, P &o) {
            o.arrayDimensions = {};
        };

    baseTypeDeclare = "int" >>
        [&](PIt v, P&o) {
            o.type = Type::Category::Int;
        };

    baseTypeDeclare = "float" >>
        [&](auto v, P&o) {
            o.type = Type::Category::Float;
        };

    statement = expression >> ";";

    expression = eps >> LexicalSymbol::Type::Identifier >> "=" >> expression >>
        [=](auto v, P&o) {

            if(auto checkId = checkVariableDeclare(v[1].addr); checkId != -1)
            {
                generateCode("=", v[3].addr, "", getVariableName(checkId));
            }

        } < "+" < "-" < "*" < "/";


    expression = expression >> "+" >> expression >>
        [=](auto v, P&o) { // 因为使用了solveArithmeticOperator 是局部变量

            solveArithmeticOperator("+", v, o);
        } > "+" > "-" < "*" < "/";

    expression = expression >> "-" >> expression >>
        [=](auto v, P&o) {
            solveArithmeticOperator("-", v, o);
        } > "+" > "-" < "*" < "/";

    expression = expression >> "*" >> expression >>
        [=](auto v, P&o) {
            solveArithmeticOperator("*", v, o);
        } > "+" > "-" > "*" > "/";

    expression = expression >> "/" >> expression >>
        [=](auto v, P&o) {
            solveArithmeticOperator("/", v, o);
        } > "+" > "-" > "*" > "/";

    expression = "+" >> expression  >>
        [=](auto v, P&o) {
            o.addr = getTemporaryVariableName();
            generateCode("plus", v[2].addr, "", o.addr);

        } > "+" > "-" > "*" > "/";

    expression = "-" >> expression >>
        [&](auto v, P&o) {
            o.addr = getTemporaryVariableName();
            generateCode("minus", v[2].addr, "", o.addr);
        } > "+" > "-" > "*" > "/";

    expression = "(" >> expression >> ")"  >>
        [&](auto v, P&o) {
            o.addr = v[2].addr;
        };


    expression = LexicalSymbol::Type::Integer >>
        [&](auto v, P&o) {
            o.addr = v[1].addr;
        };

    expression = LexicalSymbol::Type::Identifier >>
        [=](auto v, P&o) {
            std::string name = v[1].addr;

            if(auto checkId = checkVariableDeclare(v[1].addr); checkId != -1)
            {
                o.addr = getVariableName(checkId);
            }
        };

    return gf;

}





} // namespace lz





#endif /* LZ_COMPILER_C_MAKE_GRAMMAR_FACTORY_H_ */
