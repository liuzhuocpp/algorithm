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


namespace lz {

std::string getTemporaryVariableName()
{
    static int tempId = 0;

    return "$" + std::to_string(tempId++);
}


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
        LZ_NONTERMINAL_PROXY(arrayDeclare),
        LZ_NONTERMINAL_PROXY(baseTypeDeclare),

        LZ_NONTERMINAL_PROXY(statement),

        LZ_NONTERMINAL_PROXY(expression),
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
        program = declare;
        program = statement;

        declare = typeDeclare >> Lex::Identifier >> ";" >>
            [&](PIT v, P& o) {
                identifierTable.insertIdentifier(v[1].type, v[2].addr);
            };
        typeDeclare = baseTypeDeclare >> arrayDeclare >>
            [&](PIT v, P &o)
            {
                o.type.category = v[1].type.category;
                o.type.arrayDimensions = v[2].arrayDimensions;
            };

        arrayDeclare = eps >> "[" >> Lex::Integer >> "]" >> arrayDeclare >>
            [&](PIT v, P& o) {
                o.arrayDimensions.push_back(std::stoi(v[2].addr));
                o.arrayDimensions.insert(o.arrayDimensions.end(), v[4].arrayDimensions.begin(), v[4].arrayDimensions.end());
            };

        arrayDeclare = eps >>
            [&](PIT v, P &o) {
                o.arrayDimensions = {};
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


        expression = eps >> Lex::Identifier >> "=" >> expression >>
            [&](PIT v, P&o) {

                if(auto checkId = checkVariableDeclare(v[1].addr); checkId != -1)
                {
                    generateCode("=", v[3].addr, "", getVariableName(checkId));
                }

            } < "+" < "-" < "*" < "/";


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

                if(auto checkId = checkVariableDeclare(v[1].addr); checkId != -1)
                {
                    o.addr = getVariableName(checkId);
                }
            };



        expression = arrayExpression >>
            [&](PIT v, P &o){

                std::string tmp = getTemporaryVariableName();
                generateCode("=[]", v[1].addr, v[1].arrayOffsetAddr, tmp);
                o.addr = tmp;
            };

        arrayExpression = Lex::Identifier >> "[" >> expression >> "]" >>
            [&](PIT v, P &o){
                std::string arrayName = v[1].addr;
                o.addr = arrayName; // 数组名称


                auto it = identifierTable.find(arrayName);
                if(it == identifierTable.end())
                {
                    errorOfstream << "\"" << arrayName << "\" was not declare \n";
                    return ;
                }

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

    int checkVariableDeclare (std::string variable) {
        auto it = identifierTable.find(variable);
        if(it == identifierTable.end())
        {
            errorOfstream << "\"" << variable << "\" was not declare \n";
            return -1;
        }
        else
            return it->second;
    };


    std::string getVariableName (int i)
    {
        for(auto it : identifierTable)
        {
            if(it.second == i) return it.first.name; // 目前先返回变量的真实的identifier，便于debug
        }
        assert(0);
        return "";
    };





};








} // namespace lz





#endif /* LZ_COMPILER_C_GRAMMAR_INPUT_H_ */
