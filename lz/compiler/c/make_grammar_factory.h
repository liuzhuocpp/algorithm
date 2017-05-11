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
#include <lz/compiler/c/three_address_code.h>
#include <lz/compiler/c/utility.h>


namespace lz {


// this is private

template<typename GrammarParser>
struct GrammarInput
{
    using P = Properties;
    using T = LexicalSymbol;
    using PIT = std::vector<P>::iterator;
    using InstructionCategory = ThreeAddressInstruction::Category;


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

    GrammarInput():

        gf(program)
    {

        using Lex = LexicalSymbol::Category;

        program = declare >> program >>
            [&](PIT v, P& o) {
                o.nextList = v[2].nextList;
            };

        program = statement >> conditionMark >>  program >>
            [&](PIT v, P& o) {
                backPatch(v[1].nextList, v[2].cntInstructionIndex);
                o.nextList = v[3].nextList;

            };

        program = eps;

        declare = typeDeclare >> Lex::Identifier >> ";" >>
            [&](PIT v, P& o) {
                identifierTable().insert(v[2].addr, v[1].type);
            };

        typeDeclare = baseTypeDeclare >>
            [&](PIT v, P& o) {
                o.type = v[1].type;
            };

        typeDeclare = typeDeclare >> "[" >> Lex::IntNumber >> "]" >>
            [&](PIT v, P& o) {
                if(v[1].type.category() != TypeCategory::Array)
                {
                    o.type = typeTable().insert(TypeCategory::Array);
                    typeTable().arrayBaseType(o.type) = v[1].type;
                }
                else o.type = v[1].type;
                typeTable().arrayDimensions(o.type).push_back(std::stoi(v[3].addr));
            };

        baseTypeDeclare = Lex::Int >>
            [&](PIT v, P&o) {
                o.type = TypeCategory::Int;
            };

        baseTypeDeclare = Lex::Float >>
            [&](PIT v, P&o) {
                o.type = TypeCategory::Float;
            };
        baseTypeDeclare = Lex::Double >>
            [&](PIT v, P&o) {
                o.type = TypeCategory::Double;
            };

        statement = expression >> ";";
        statement = ";";
        statement = "{" >> statementList >> "}";

        statement = eps >> Lex::If >> "(" >> condition >> ")" >> conditionMark >> statement >>
            [&](PIT v, P&o) {
                backPatch(v[3].trueList, v[5].cntInstructionIndex);
                o.nextList = merge(v[3].falseList, v[6].nextList);


            } < Lex::Else;

        statement = eps >> Lex::If >> "(" >> condition >> ")" >> conditionMark >> statement >> elseSymbol >> conditionMark >> statement >>
            [&](PIT v, P&o) {
                backPatch(v[3].trueList, v[5].cntInstructionIndex);
                backPatch(v[3].falseList, v[8].cntInstructionIndex);

                merge(o.nextList, v[6].nextList);
                merge(o.nextList, v[7].nextList);
                merge(o.nextList, v[9].nextList);
            };

        statement = eps >> Lex::While >> "(" >> conditionMark >>  condition >> ")" >> conditionMark >> statement >>
            [&](PIT v, P&o) {

                backPatch(v[4].trueList, v[6].cntInstructionIndex);

                merge(o.nextList, v[4].falseList);

                backPatch(v[7].nextList, nextInstructionIndex());

                generateGotoCode(v[3].cntInstructionIndex);


            };

        statementList =  statement >> conditionMark >>  statementList >>
            [&](PIT v, P&o) {
                backPatch(v[1].nextList, v[2].cntInstructionIndex);
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
                backPatch(v[1].falseList, v[3].cntInstructionIndex);
                o.trueList = merge(v[1].trueList, v[4].trueList);
                o.falseList = v[4].falseList;

            }  < "&&" > "||";

        condition = condition >> "&&" >> conditionMark >> condition >>
            [&](PIT v, P&o) {
                backPatch(v[1].trueList, v[3].cntInstructionIndex);
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

            };


        conditionMark = eps >>
            [&](PIT v, P&o) {
                o.cntInstructionIndex = nextInstructionIndex();
            };

        elseSymbol = Lex::Else >>
            [&](PIT v, P&o) {
                o.nextList.push_back(nextInstructionIndex());
                generateGotoCode();

            };



        expression = expression >> "+" >> expression >> solveArithmeticOperator
             > "+" > "-" < "*" < "/";

        expression = expression >> "-" >> expression >> solveArithmeticOperator
            > "+" > "-" < "*" < "/";

        expression = expression >> "*" >> expression >> solveArithmeticOperator
            > "+" > "-" > "*" > "/";

        expression = expression >> "/" >> expression >> solveArithmeticOperator
            > "+" > "-" > "*" > "/";

        expression = "+" >> expression  >> solveUnaryPlusOrMinusOperator
            > "+" > "-" > "*" > "/";

        expression = "-" >> expression >> solveUnaryPlusOrMinusOperator
            > "+" > "-" > "*" > "/";

        expression = "(" >> expression >> ")" >>
            [&](PIT v, P&o) {
                o.addr = v[2].addr;
            };

        expression = Lex::IntNumber >>
            [&](PIT v, P&o) {
                o.addr = v[1].addr;
                o.type = TypeCategory::Int;
            };

        expression = Lex::DoubleNumber >>
            [&](PIT v, P&o) {
                o.addr = v[1].addr;
                o.type = TypeCategory::Double;
            };


        expression = Lex::Identifier >>
            [&](PIT v, P&o) {

                checkVariableDeclare(v[1].addr, [&](int id) {
                    o.addr = getVariableName(id);
                    o.type = identifierTable().type(id);
                });
            };

        expression = eps >> Lex::Identifier >> "=" >> expression >>
            [&](PIT v, P&o) {

                checkVariableDeclare(v[1].addr, [&](int id) {
                    generateCode(InstructionCategory::Assign, v[3].addr, "", getVariableName(id));
                    o.addr = v[1].addr;
                    o.type = identifierTable().type(id);
                });
            } < "+" < "-" < "*" < "/";

        expression = arrayExpression >>
            [&](PIT v, P &o){

                std::string tmp = getTemporaryVariableName();
                generateCode(InstructionCategory::ReadArray, v[1].addr, v[1].arrayOffsetAddr, tmp);
                o.addr = tmp;
            };

        expression = arrayExpression >> "=" >> expression >>
            [&](PIT v, P&o) {

                generateCode(InstructionCategory::WriteArray, v[3].addr, v[1].arrayOffsetAddr, v[1].addr);
                std::string tmp = getTemporaryVariableName();
                generateCode(InstructionCategory::ReadArray, v[1].addr, v[1].arrayOffsetAddr, tmp);
                o.addr = tmp;
            } < "+" < "-" < "*" < "/";

        arrayExpression = Lex::Identifier >> "[" >> expression >> "]" >>
            [&](PIT v, P &o){

                checkVariableDeclare(v[1].addr, [&](int identifierId) {
                    o.addr = v[1].addr; // 数组名称
                    o.type = identifierTable().type(identifierId); // 数组类型
                    o.cntArrayDimensionId = 0;
                    std::string tmp = generateCalcualteArrayPartialIndexCode(v[3].addr, o.cntArrayDimensionId, o.type);
                    o.arrayOffsetAddr = tmp;
                });
            };

        arrayExpression = arrayExpression >> "[" >> expression >> "]" >>
            [&](PIT v, P &o) {
                o.addr = v[1].addr;
                o.type = v[1].type;
                o.cntArrayDimensionId = v[1].cntArrayDimensionId + 1;
                std::string tmp1 = generateCalcualteArrayPartialIndexCode(v[3].addr, o.cntArrayDimensionId, o.type);
                std::string tmp2 = getTemporaryVariableName();
                generateCode(InstructionCategory::Plus, v[1].arrayOffsetAddr, tmp1 , tmp2);
                o.arrayOffsetAddr = tmp2;

            };


    }


    // I think use the global variable is very ugly.
    static GrammarParser* global_grammarParser;

    static IdentifierTable& identifierTable() { return global_grammarParser->identifierTable(); }
    static TypeTable& typeTable() { return global_grammarParser->typeTable(); }
    static ThreeAddressCode& codeTable() { return global_grammarParser->codeTable(); }
    static auto& errorOfstream() { return global_grammarParser->errorOfstream(); }



    // Bellow function must be static,
    // otherwise, when GrammarInput is destoryed, the bellow calls will be error
    static void solveArithmeticOperator (PIT v, P &o)
    {
        o.addr = getTemporaryVariableName();

        generateCode(ThreeAddressInstruction::toCategory(v[2].addr), v[1].addr, v[3].addr, o.addr);
    };


    template<typename Callback>
    static void checkVariableDeclare ( std::string variable, Callback callback)
    {
        int it = identifierTable().find(variable);
        if(it == -1)
        {
            errorOfstream() << "\"" << variable << "\" was not declare \n";
        }
        else
        {
            callback(it);
        }
    };




    static std::string getVariableName (int i)
    {
        return identifierTable().identifier(i);// 目前先返回变量的真实的identifier，便于debug
    };

    static void generateCode(InstructionCategory op, std::string arg1, std::string arg2, std::string res)
    {
        codeTable().generateCode(op, arg1, arg2, res);
    };


    static void generateGotoCode(int label)
    {
        codeTable().generateGotoCode(label);
    }

    static void generateGotoCode()
    {
        codeTable().generateGotoCode();
    }



    static int nextInstructionIndex()
    {
        return codeTable().nextInstructionIndex();
    }


    static void backPatch(const std::list<int> & a, int instructionId)
    {
        codeTable().backPatch(a.begin(), a.end(), instructionId);
    }

    static std::list<int> merge(std::list<int>&a1, std::list<int>&a2)
    {
        a1.splice(a1.end(), a2);
        return a1;
    }

    static void solveRelationalOperator(PIT v, P &o)
    {
        o.trueList.push_back(nextInstructionIndex());
        generateCode(ThreeAddressInstruction::toIfRel(v[2].addr), v[1].addr, v[3].addr, "-");
        o.falseList.push_back(nextInstructionIndex());
        generateGotoCode();
    }

    static void solveUnaryPlusOrMinusOperator(PIT v, P &o)
    {
        o.addr = getTemporaryVariableName();
        std::string op = v[1].addr;
        InstructionCategory ansOp;
        if(op == "+")
            ansOp = InstructionCategory::UnaryPlus;
        else if(op == "-")
            ansOp = InstructionCategory::UnaryMinus;
        else assert(0);
        generateCode(ansOp, v[2].addr, "", o.addr);
    }


    static std::string generateCalcualteArrayPartialIndexCode(
        std::string cntDimensionAddr, int cntDimensionId, TypeDescriptor arrayType)
    {
        std::string tmp = getTemporaryVariableName();
        generateCode(
            InstructionCategory::Multiply,
            cntDimensionAddr,
            std::to_string(calculateArrayDimensionProduct(arrayType, cntDimensionId)),
            tmp);
        return tmp;

    }

    static int calculateArrayDimensionProduct(TypeDescriptor t, int cntArrayDimensionId)
    {
        const std::vector<int>& dimensions = typeTable().arrayDimensions(t);
        return calProduct(dimensions.begin() + cntArrayDimensionId + 1, dimensions.end()  ) ;
    }

    template<typename Iterator>
    static int calProduct(Iterator first, Iterator last)
    {
        int sum = 1;
        while(first < last)
        {
            sum *= *first;
            first++;
        }
        return sum;
    }

};


template<typename GrammarParser>
GrammarParser* GrammarInput<GrammarParser>::global_grammarParser =  nullptr;

template<typename GrammarParser>
auto makeGrammarFactory(
        GrammarParser& grammarParser )
{

    GrammarInput<GrammarParser>::global_grammarParser = &grammarParser;
    return  GrammarInput<GrammarParser> ().gf;
}




} // namespace lz





#endif /* LZ_COMPILER_C_MAKE_GRAMMAR_FACTORY_H_ */
