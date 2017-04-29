/*
 * make_grammar_factory.h
 *
 *  Created on: 2017年4月27日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_C_GRAMMAR_INPUT_H_
#define LZ_COMPILER_C_GRAMMAR_INPUT_H_

#include <lz/compiler/noterminal_proxy.h>
#include <lz/compiler/grammar.h>
#include <lz/compiler/slr1_grammar.h>
#include <lz/compiler/lr_grammar.h>


#include <lz/compiler/c/lexical_analyze.h>
#include <lz/compiler/c/identifier.h>
#include <lz/compiler/c/properties.h>
#include <lz/compiler/c/utility.h>


namespace lz {



template<typename GenerateCode, typename ErrorOfstream>
struct GrammarInput
{
    using P = Properties;
    using T = LexicalSymbol;
    using PIT = std::vector<P>::iterator;

    NonterminalProxy<T, P>
        LZ_NONTERMINAL_PROXY(program),
        LZ_NONTERMINAL_PROXY(declare),
        LZ_NONTERMINAL_PROXY(typeDeclare),
        LZ_NONTERMINAL_PROXY(baseTypeDeclare),

        LZ_NONTERMINAL_PROXY(statement),
        LZ_NONTERMINAL_PROXY(statementList),

        LZ_NONTERMINAL_PROXY(expression),
        LZ_NONTERMINAL_PROXY(condition),
        LZ_NONTERMINAL_PROXY(arrayExpression)


        ;

    GrammarFactory<T, P> gf;


    IdentifierTable &identifierTable; // must be reference , otherwise error!
    GenerateCode&generateCode;// must be reference , otherwise error!
    ErrorOfstream& errorOfstream;// must be reference , otherwise error!


    GrammarInput(IdentifierTable &identifierTable, GenerateCode& generateCode, ErrorOfstream& errorOfstream):
        identifierTable(identifierTable),generateCode(generateCode), errorOfstream(errorOfstream),
        gf(program)
    {


        using Lex = LexicalSymbol::Category;

        program = declare >> program;
        program = statement >> program;
        program = eps;
//        program = declare;
//        program = statement;

        declare = typeDeclare >> Lex::Identifier >> ";" >>
            [&](PIT v, P& o) {
                identifierTable.insertIdentifier(v[1].type, v[2].addr);
            };

        typeDeclare = baseTypeDeclare >>
            [&](PIT v, P& o) {
                o.type.category = v[1].type.category;
            };

        typeDeclare = typeDeclare >> "[" >> Lex::Integer >> "]" >>
            [&](PIT v, P& o) {
                o.type = v[1].type;
                o.type.arrayDimensions.push_back(std::stoi(v[3].addr));
            };

        baseTypeDeclare = Lex::Int >>
            [&](PIT v, P&o) {
                o.type = Type::Category::Int;
            };

        baseTypeDeclare = Lex::Float >>
            [&](PIT v, P&o) {
                o.type = Type::Category::Float;
            };

        statement = expression >> ";";


        statement = "{" >> statementList >> "}";
        statement = eps >> Lex::If >> "(" >> condition >> ")" >> statement  > Lex::Else;
        statement = eps >> Lex::If >> "(" >> condition >> ")" >> statement >> Lex::Else >> statement ;
        statementList = statement >> statementList;
        statementList = eps;
        condition = expression >> ">" >> expression;
        condition = expression >> ">=" >> expression;
        condition = expression >> "<" >> expression;
        condition = expression >> "<=" >> expression;
        condition = expression >> "==" >> expression;
        condition = expression >> "!=" >> expression;
        condition = condition >> "||" >> condition < "&&" > "||";
        condition = condition >> "&&" >> condition > "&&" > "||";
        condition = "!" >>  condition                   > "&&" > "||";
        condition = Lex::True;
        condition = Lex::False;


        expression = expression >> "+" >> expression >>
            [&](PIT v, P&o) { // 因为使用了solveArithmeticOperator 是局部变量

                solveArithmeticOperator("+", v, o);
            } > "+" > "-" < "*" < "/";

        expression = expression >> "-" >> expression >>
            [&](PIT v, P&o) {
                solveArithmeticOperator("-", v, o);
            } > "+" > "-" < "*" < "/";

        expression = expression >> "*" >> expression >>
            [&](PIT v, P&o) {
                solveArithmeticOperator("*", v, o);
            } > "+" > "-" > "*" > "/";

        expression = expression >> "/" >> expression >>
            [&](PIT v, P&o) {
                solveArithmeticOperator("/", v, o);
            } > "+" > "-" > "*" > "/";

        expression = "+" >> expression  >>
            [&](PIT v, P&o) {
                o.addr = getTemporaryVariableName();
                generateCode("plus", v[2].addr, "", o.addr);

            } > "+" > "-" > "*" > "/";

        expression = "-" >> expression >>
            [&](PIT v, P&o) {
                o.addr = getTemporaryVariableName();
                generateCode("minus", v[2].addr, "", o.addr);
            } > "+" > "-" > "*" > "/";

        expression = "(" >> expression >> ")"  >>
            [&](PIT v, P&o) {
                o.addr = v[2].addr;
            };


        expression = Lex::Integer >>
            [&](PIT v, P&o) {
                o.addr = v[1].addr;
            };

        expression = Lex::Identifier >>
            [&](PIT v, P&o) {
                std::string name = v[1].addr;

                if(auto checkIt = checkVariableDeclare(v[1].addr); checkIt != identifierTable.end())
                {
                    o.addr = getVariableName(checkIt);
                }
            };


        expression = eps >> Lex::Identifier >> "=" >> expression >>
            [&](PIT v, P&o) {

                if(auto checkIt = checkVariableDeclare(v[1].addr); checkIt != identifierTable.end())
                {
                    generateCode("=", v[3].addr, "", getVariableName(checkIt));
                }

            } < "+" < "-" < "*" < "/";

        expression = arrayExpression >>
            [&](PIT v, P &o){

                std::string tmp = getTemporaryVariableName();
                generateCode("=[]", v[1].addr, v[1].arrayOffsetAddr, tmp);
                o.addr = tmp;
            };


        expression = arrayExpression >> "=" >> expression >>
            [&](PIT v, P&o) {

                generateCode("[]=", v[3].addr, v[1].arrayOffsetAddr, v[1].addr);

                std::string tmp = getTemporaryVariableName();
                generateCode("=[]", v[1].addr, v[1].arrayOffsetAddr, tmp);

                o.addr = tmp;

            }< "+" < "-" < "*" < "/";


        arrayExpression = Lex::Identifier >> "[" >> expression >> "]" >>
            [&](PIT v, P &o){
                std::string arrayName = v[1].addr;
                o.addr = arrayName; // 数组名称

                auto it = checkVariableDeclare(arrayName);
                if(it == identifierTable.end()) return ;

                std::string tmp = getTemporaryVariableName();
                o.type = it->first.type.subArray();
                generateCode("*", v[3].addr, std::to_string(o.type.getWidth()), tmp);
                o.arrayOffsetAddr = tmp;

            };

        arrayExpression = arrayExpression >> "[" >> expression >> "]" >>
            [&](PIT v, P &o) {
                o.type = v[1].type.subArray();
                o.addr = v[1].addr;
                std::string tmp1 = getTemporaryVariableName();
                std::string tmp2 = getTemporaryVariableName();
                generateCode("*", v[3].addr, std::to_string(o.type.getWidth()) , tmp1);
                generateCode("+", v[1].arrayOffsetAddr, tmp1 , tmp2);
                o.arrayOffsetAddr = tmp2;

            };


    }






    void solveArithmeticOperator (std::string op, PIT v, P &o) {
        o.addr = getTemporaryVariableName();
        generateCode(op, v[1].addr, v[3].addr, o.addr);
    };

    auto checkVariableDeclare (std::string variable) {
        auto it = identifierTable.find(variable);
        if(it == identifierTable.end())
        {
            errorOfstream << "\"" << variable << "\" was not declare \n";
            return identifierTable.end();
        }
        else
            return it;
    };


    std::string getVariableName (IdentifierTable::iterator it)
    {
        return it->first.name; // 目前先返回变量的真实的identifier，便于debug
//        for(auto it : identifierTable)
//        {
//            if(it.second == i) return it.first.name; // 目前先返回变量的真实的identifier，便于debug
//        }
//        assert(0);
//        return "";
    };





};








} // namespace lz





#endif /* LZ_COMPILER_C_GRAMMAR_INPUT_H_ */
