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
#include <lz/compiler/c/utility.h>
#include <lz/compiler/c/intermediate_representation.h>


namespace lz {


// this is private

template<typename ErrorOfstream>
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

        LZ_NONTERMINAL_PROXY(conditionMark),
        LZ_NONTERMINAL_PROXY(elseSymbol),


        LZ_NONTERMINAL_PROXY(expression),
        LZ_NONTERMINAL_PROXY(condition),
        LZ_NONTERMINAL_PROXY(arrayExpression)


        ;

    GrammarFactory<T, P> gf;
    // I think use the global variable is very ugly.
    static IdentifierTable* global_identifierTable;
    static IRTable* global_codeTable;
    static ErrorOfstream* global_errorOfstream;


    GrammarInput(IdentifierTable &identifierTable, IRTable& codeTable, ErrorOfstream& errorOfstream):

        gf(program)
    {

        using Lex = LexicalSymbol::Category;

        program = declare >> program >>
                [&](PIT v, P& o) {
                o.nextList = v[2].nextList;
            };

        program = statement >> conditionMark >>  program >>
            [&](PIT v, P& o) {
                backPatch(v[1].nextList, v[2].cntLabel);
                o.nextList = v[3].nextList;

            };

        program = eps;

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
        statement = ";";
        statement = "{" >> statementList >> "}";

        statement = eps >> Lex::If >> "(" >> condition >> ")" >> conditionMark >> statement >>
            [&](PIT v, P&o) {
                backPatch(v[3].trueList, v[5].cntLabel);
                o.nextList = merge(v[3].falseList, v[6].nextList);


            } < Lex::Else;

        statement = eps >> Lex::If >> "(" >> condition >> ")" >> conditionMark >> statement >> elseSymbol >> conditionMark >> statement >>
            [&](PIT v, P&o) {
                backPatch(v[3].trueList, v[5].cntLabel);
                backPatch(v[3].falseList, v[8].cntLabel);

                splice(o.nextList, v[6].nextList);
                splice(o.nextList, v[7].nextList);
                splice(o.nextList, v[9].nextList);


            };
        statementList =  statement >> conditionMark >>  statementList >>
            [&](PIT v, P&o) {
                backPatch(v[1].nextList, v[2].cntLabel);
                o.nextList = v[3].nextList;
            };

        statementList =  eps >>
            [&](PIT v, P&o) {
                o.nextList =  {};
            };


        condition = expression >> "<" >> expression >> solveRelationalOperator;
        condition = expression >> ">" >> expression >> solveRelationalOperator;

        condition = expression >> "<=" >> expression >> solveRelationalOperator;
        condition = expression >> ">=" >> expression >> solveRelationalOperator;
        condition = expression >> "==" >> expression >> solveRelationalOperator;
        condition = expression >> "!=" >> expression >> solveRelationalOperator;


        condition = condition >> "||" >> conditionMark >>  condition >>
            [&](PIT v, P&o) {
                backPatch(v[1].falseList, v[3].cntLabel);
                o.trueList = merge(v[1].trueList, v[4].trueList);
                o.falseList = v[4].falseList;

            }  < "&&" > "||";

        condition = condition >> "&&" >> conditionMark >> condition >>
            [&](PIT v, P&o) {
                backPatch(v[1].trueList, v[3].cntLabel);
                o.trueList = v[4].trueList;
                o.falseList = merge(v[1].falseList, v[4].falseList);

            }  > "&&" > "||";

        condition = "!" >>  condition >>
            [&](PIT v, P&o) {
                o.trueList = v[2].falseList;
                o.falseList = v[2].trueList;

            } > "&&" > "||";

        condition = "(" >>  condition >> ")" >>
            [&](PIT v, P&o) {
                o.trueList = v[2].trueList;
                o.falseList = v[2].falseList;

            } > "&&" > "||";


        conditionMark = eps >>
            [&](PIT v, P&o) {
                o.cntLabel = nextLabel();
            };

        elseSymbol = Lex::Else >>
            [&](PIT v, P&o) {
                o.nextList.push_back(nextLabel());
                generateCode("goto", "", "", "-");

            };

//        statementList = statement >> statementList;
//        statementList = eps;

//        condition = Lex::True;
//        condition = Lex::False;


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
//                std::cout << "operator=+++++++++++" << std::endl;

                if(auto checkIt = checkVariableDeclare(v[1].addr); checkIt != identifierTable.end())
                {
//                    std::cout << "009((((((((((((((((=+++++++++++" << std::endl;
//                    generateCode("=", v[3].addr, "", "SS");
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





    // bellow function must be static,
    // otherwise, when GrammarInput is destoryed, the bellow calls will be error
    static void solveArithmeticOperator (std::string op, PIT v, P &o)
    {
        o.addr = getTemporaryVariableName();
        generateCode(op, v[1].addr, v[3].addr, o.addr);
    };

    static auto checkVariableDeclare ( std::string variable)
    {
        auto it = global_identifierTable->find(variable);
        if(it == global_identifierTable->end())
        {
            (*global_errorOfstream) << "\"" << variable << "\" was not declare \n";
            return global_identifierTable->end();
        }
        else
            return it;
    };


    static std::string getVariableName (IdentifierTable::iterator it)
    {
        return it->first.name; // 目前先返回变量的真实的identifier，便于debug
    };


    static void generateCode(std::string op, std::string arg1, std::string arg2, std::string res)
    {
        global_codeTable->push_back({op, arg1, arg2, res});
        global_codeTable->back().label = global_codeTable->size() - 1;
    };

    static int nextLabel()
    {
        return global_codeTable->size();
    }


    static void backPatch(const std::list<int>&a1, int label)
    {
        for(auto x: a1)
        {
            (*global_codeTable)[x].res = "L" + std::to_string(label);
        }
    }

    // can be improve
    static std::list<int> merge(std::list<int>&a1, std::list<int>&a2)
    {
        a1.splice(a1.end(), a2);
        return a1;
    }

    static void splice(std::list<int>&a1, std::list<int>&a2)
    {
        a1.splice(a1.end(), a2);
    }



    static std::list<int> makeList(int label)
    {
        return std::list<int>{label};
    }

    static void solveRelationalOperator(PIT v, P &o)
    {
        o.trueList.push_back(nextLabel());
        generateCode("if" + v[2].addr, v[1].addr, v[3].addr, "-");
        o.falseList.push_back(nextLabel());
        generateCode("goto", "", "", "-");

    }

};


template<typename E>
IdentifierTable* GrammarInput<E>::global_identifierTable=  nullptr;


template<typename E>
IRTable* GrammarInput<E>::global_codeTable=  nullptr;


template<typename E>
E* GrammarInput<E>::global_errorOfstream =  nullptr;



template<typename ErrorOfstream>
auto makeGrammarFactory(
    IdentifierTable &identifierTable,
    IRTable &codeTable,
    ErrorOfstream &errorOfstream)
{

    GrammarInput<ErrorOfstream>::global_identifierTable = &identifierTable;
    GrammarInput<ErrorOfstream>::global_codeTable = &codeTable;
    GrammarInput<ErrorOfstream>::global_errorOfstream = &errorOfstream;



    return  GrammarInput<ErrorOfstream>
       (identifierTable, codeTable, errorOfstream).gf;

}














} // namespace lz





#endif /* LZ_COMPILER_C_MAKE_GRAMMAR_FACTORY_H_ */
