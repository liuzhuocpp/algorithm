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
    using InstructionArgument = ThreeAddressInstructionArgument;


    NonterminalProxy<T, P>
        LZ_NONTERMINAL_PROXY(program),
        LZ_NONTERMINAL_PROXY(declare),
        LZ_NONTERMINAL_PROXY(typeDeclare),
        LZ_NONTERMINAL_PROXY(baseTypeDeclare),

        LZ_NONTERMINAL_PROXY(statement),
        LZ_NONTERMINAL_PROXY(statementList),

        LZ_NONTERMINAL_PROXY(conditionMark),
        LZ_NONTERMINAL_PROXY(elseConditionMark),



        LZ_NONTERMINAL_PROXY(expression),
        LZ_NONTERMINAL_PROXY(arrayExpression)


        ;

    GrammarFactory<T, P> gf;

    GrammarInput():

        gf(program)
    {

        identifierTableForThreeAddressInstructionArgumentOutput = &identifierTable();

        // 优先级依次降低排列

        gf.addRightAssociativity("!");

        gf.addLeftAssociativity("*", "/");
        gf.addLeftAssociativity("+", "-");
        gf.addLeftAssociativity("<", "<=", ">", ">=");
        gf.addLeftAssociativity("==", "!=");
        gf.addLeftAssociativity("&&");
        gf.addLeftAssociativity("||");

        gf.addRightAssociativity("=");



        using Lex = LexicalSymbol::Category;

        program = declare >> program;

        program = statementList >>
            [&](PIT v, P& o) {
                if(!v[1].breakList.empty())
                {
                    errorOfstream() << "break statement not within a loop\n";
                }

                if(!v[1].continueList.empty())
                {
                    errorOfstream() << "continue statement not within a loop\n";
                }
            };

        declare = typeDeclare >> Lex::Identifier >> ";" >>
            [&](PIT v, P& o) {
                identifierTable().insert(v[2].lexValue, v[1].type);
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
                typeTable().addArrayDimension(o.type, std::stoi(v[3].lexValue));
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

        baseTypeDeclare = Lex::Bool >>
            [&](PIT v, P&o) {
                o.type = TypeCategory::Bool;
            };

        statementList = statement >> conditionMark >> statementList >>
            [&](PIT v, P&o) {
                backPatch(v[1].nextList, v[2].cntInstructionIndex);

                o.breakList = merge(v[1].breakList, v[3].breakList);
                o.continueList = merge(v[1].continueList, v[3].continueList);
            };

        statementList = eps ;

        statement = eps >> Lex::Break >> ";" >>
            [&](PIT v, P& o) {
                o.breakList.push_back(nextInstructionIndex());
                generateGotoCode();
            };

        statement = eps >> Lex::Continue >> ";" >>
            [&](PIT v, P& o) {
                o.continueList.push_back(nextInstructionIndex());
                generateGotoCode();
            };

        statement = expression >> ";";
        statement = ";";
        statement = "{" >> statementList >> "}" >>
            [&](PIT v, P& o){
                o.breakList = v[2].breakList;
                o.continueList = v[2].continueList;
            };

        statement = eps >> Lex::If >> "(" >> expression >> ")" >> conditionMark >> statement >>
            [&](PIT v, P&o) {
                backPatch(v[3].trueList, v[5].cntInstructionIndex);
                o.nextList = merge(v[3].falseList, v[6].nextList);


                o.breakList = v[6].breakList;
                o.continueList = v[6].continueList;
            } < Lex::Else;

        statement = eps >> Lex::If >> "(" >> expression >> ")" >> conditionMark >> statement >> Lex::Else >> elseConditionMark >> statement >>
            [&](PIT v, P&o) {

                auto& condition = v[3], &conMark = v[5], &ifStatement = v[6], &elseConMark = v[8], &elseStatement = v[9];

                backPatch(condition.trueList, conMark.cntInstructionIndex);
                backPatch(condition.falseList, elseConMark.cntInstructionIndex);
                merge(o.nextList, ifStatement.nextList);
                merge(o.nextList, elseConMark.nextList);
                merge(o.nextList, elseStatement.nextList);


                o.breakList = merge(ifStatement.breakList, elseStatement.breakList);
                o.continueList = merge(ifStatement.continueList, elseStatement.continueList);
            };

        statement = eps >> Lex::While >> "(" >> conditionMark >>  expression >> ")" >> conditionMark >> statement >>
            [&](PIT v, P&o) {

                backPatch(v[4].trueList, v[6].cntInstructionIndex);
                merge(o.nextList, v[4].falseList);
                backPatch(v[7].nextList, nextInstructionIndex());
                generateGotoCode(v[3].cntInstructionIndex);

                backPatch(v[7].breakList, nextInstructionIndex());
                backPatch(v[7].continueList, v[3].cntInstructionIndex);
            };

        conditionMark = eps >>
            [&](PIT v, P&o) {
                o.cntInstructionIndex = nextInstructionIndex();
            };

        elseConditionMark = eps >>
            [&](PIT v, P& o) {

                o.nextList.push_back(nextInstructionIndex());
                generateGotoCode();
                o.cntInstructionIndex = nextInstructionIndex();
            };

        expression = expression >> "<" >> expression >> solveRelationalOperator;
        expression = expression >> "<=" >> expression >> solveRelationalOperator;

        expression = expression >> ">" >> expression >> solveRelationalOperator;
        expression = expression >> ">=" >> expression >> solveRelationalOperator;

        expression = expression >> "==" >> expression >> solveRelationalOperator;
        expression = expression >> "!=" >> expression >> solveRelationalOperator;


        expression = expression >> "||" >> conditionMark >>  expression >>
            [&](PIT v, P&o) {

                checkTypeEquality(v[1].type, v[4].type, [&]() {
                    checkBoolRequired(v[1].type, [&](){
                        backPatch(v[1].falseList, v[3].cntInstructionIndex);
                        o.trueList = merge(v[1].trueList, v[4].trueList);
                        o.falseList = v[4].falseList;
                        o.type = TypeCategory::Bool;
                    });
                });
            };

        expression = expression >> "&&" >> conditionMark >> expression >>
            [&](PIT v, P&o) {
                checkTypeEquality(v[1].type, v[4].type, [&]() {
                    checkBoolRequired(v[1].type, [&](){

                        backPatch(v[1].trueList, v[3].cntInstructionIndex);
                        o.trueList = v[4].trueList;
                        o.falseList = merge(v[1].falseList, v[4].falseList);

                        o.type = TypeCategory::Bool;
                    });
                });


            };

        expression = "!" >>  expression >>
            [&](PIT v, P&o) {
                checkBoolRequired(v[2].type, [&](){
                    o.trueList = v[2].falseList;
                    o.falseList = v[2].trueList;
                    o.type = v[2].type;
                });
            };

        expression = expression >> "+" >> expression >> solveArithmeticOperator;
        expression = expression >> "-" >> expression >> solveArithmeticOperator;
        expression = expression >> "*" >> expression >> solveArithmeticOperator;
        expression = expression >> "/" >> expression >> solveArithmeticOperator;

        expression = "+" >> expression >> solveUnaryPlusOrMinusOperator > "*";
        expression = "-" >> expression >> solveUnaryPlusOrMinusOperator > "/";

        expression = "(" >> expression >> ")" >>
            [&](PIT v, P&o) {
                o.addr = v[2].addr;
                o.type = v[2].type;
                o.trueList = v[2].trueList;
                o.falseList = v[2].falseList;
            };

        expression = Lex::IntNumber >>
            [&](PIT v, P&o) {
                o.addr = InstructionArgument::makeNumber(std::stoi(v[1].lexValue));
//                o.addr = v[1].addr;
                o.type = TypeCategory::Int;
            };

        expression = Lex::DoubleNumber >>
            [&](PIT v, P&o) {
                o.addr = v[1].addr; assert(0);
                o.type = TypeCategory::Double;
            };

        expression = Lex::True >>
            [&](PIT v, P&o) {
                o.addr = InstructionArgument::makeTrue();
//                o.addr = v[1].addr;
                o.type = TypeCategory::Bool;
            };

        expression = Lex::False >>
            [&](PIT v, P&o) {
                o.addr = InstructionArgument::makeFalse();
                o.type = TypeCategory::Bool;
            };

        expression = Lex::Identifier >>
            [&](PIT v, P&o) {

                checkVariableDeclare(v[1].lexValue, [&](int id) {
                    o.addr = InstructionArgument::makeVariable(id);
                    o.type = identifierTable().type(id);
                });
            };

        expression = eps >> Lex::Identifier >> "=" >> expression >>
            [&](PIT v, P&o) {
                checkVariableDeclare(v[1].lexValue, [&](int resId) {
                    checkTypeEquality(identifierTable().type(resId), v[3].type, [&](){
                        generateCode(InstructionCategory::Assign,
                            v[3].addr,
                            InstructionArgument::makeEmpty(),
                            InstructionArgument::makeVariable(resId));

                        o.addr = InstructionArgument::makeVariable(resId);
                        o.type = identifierTable().type(resId);
                    });
                });
            };

        expression = arrayExpression >>
            [&](PIT v, P &o){

                unsigned tmp = getTemporaryVariableId();
                generateCode(InstructionCategory::ReadArray,
                    v[1].addr,
                    v[1].arrayOffsetAddr,
                    InstructionArgument::makeTempVariable(tmp));

                o.addr = InstructionArgument::makeTempVariable(tmp);
                o.type = v[1].type;
            };

        expression = arrayExpression >> "=" >> expression >>
            [&](PIT v, P&o) {

                generateCode(InstructionCategory::WriteArray, v[3].addr, v[1].arrayOffsetAddr, v[1].addr);
                unsigned tmp = getTemporaryVariableId();
                generateCode(InstructionCategory::ReadArray, v[1].addr, v[1].arrayOffsetAddr, InstructionArgument::makeTempVariable(tmp));
                o.addr = InstructionArgument::makeTempVariable(tmp);
                o.type = v[1].type;
            };

        arrayExpression = Lex::Identifier >> "[" >> expression >> "]" >>
            [&](PIT v, P &o){

                checkVariableDeclare(v[1].lexValue, [&](int identifierId) {

                    o.addr = InstructionArgument::makeVariable(identifierId); // 数组名称
                    TypeDescriptor arrayType = identifierTable().type(identifierId); // 数组类型
                    o.type = typeTable().subarrayType(arrayType);
                    unsigned tmp = getTemporaryVariableId();
                    generateCode(InstructionCategory::Multiply,
                        v[3].addr,
                        InstructionArgument::makeNumber(typeTable().getWidth(o.type)),
                        InstructionArgument::makeTempVariable(tmp));
                    o.arrayOffsetAddr =  InstructionArgument::makeTempVariable(tmp);

                });
            };

        arrayExpression = arrayExpression >> "[" >> expression >> "]" >>
            [&](PIT v, P &o) {
                o.addr = v[1].addr;
                o.type = typeTable().subarrayType(v[1].type);

                unsigned tmp1 = getTemporaryVariableId();

                generateCode(InstructionCategory::Multiply,
                    v[3].addr,
                    InstructionArgument::makeNumber(typeTable().getWidth(o.type)),
                    InstructionArgument::makeTempVariable(tmp1) );

                unsigned tmp2 = getTemporaryVariableId();
                generateCode(InstructionCategory::Plus,
                    v[1].arrayOffsetAddr,
                    InstructionArgument::makeTempVariable(tmp1),
                    InstructionArgument::makeTempVariable(tmp2));
                o.arrayOffsetAddr = InstructionArgument::makeTempVariable(tmp2);
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
        checkTypeEquality(v[1].type, v[3].type, [&](){
            o.addr = InstructionArgument::makeTempVariable(getTemporaryVariableId());
            o.type = v[1].type;

            generateCode(ThreeAddressInstruction::toCategory(v[2].lexValue), v[1].addr, v[3].addr, o.addr);
        });

    }

    template<typename Callback>
    static void checkVariableDeclare( std::string variable, Callback callback)
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
    }

    template<typename Callback>
    static void checkTypeEquality(TypeDescriptor t1, TypeDescriptor t2, Callback callback)
    {

        if(typeTable().typeEqual(t1, t2))
        {
            callback();
        }
        else
        {
            errorOfstream() << "type inconsistent: " << typeTable().typeToString(t1) << ", " << typeTable().typeToString(t2) << "\n";
        }
    }

    template<typename Callback>
    static void checkBoolRequired(TypeDescriptor t, Callback callback)
    {
        if(t.category() == TypeCategory::Bool)
        {
            callback();
        }
        else
        {
            errorOfstream() << "required type is bool, but is " << typeCategoryToName(t.category()) <<  "\n";
        }
    }

    template<typename Callback>
    static void checkCanArithmeticOperate(TypeDescriptor t, Callback callback)
    {
        if(isBaseType(t.category()))
        {
            callback();
        }
        else
        {
            errorOfstream() << "type " << typeCategoryToName(t.category()) << "can't be used as arithmetic operator "<<  "\n";
        }
    }




    static std::string getVariableName (int i)
    {
        return identifierTable().identifier(i);// 目前先返回变量的真实的identifier，便于debug
    };

    static void generateCode(InstructionCategory op, InstructionArgument arg1, InstructionArgument arg2, InstructionArgument res)
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
        checkTypeEquality(v[1].type, v[3].type, [&](){
            o.trueList.push_back(nextInstructionIndex());
            generateCode(ThreeAddressInstruction::toIfRel(v[2].lexValue),
                v[1].addr,
                v[3].addr,
                InstructionArgument::makeEmpty());
            o.falseList.push_back(nextInstructionIndex());
            generateGotoCode();
            o.type = TypeCategory::Bool;
        });

    }

    static void solveUnaryPlusOrMinusOperator(PIT v, P &o)
    {
        o.addr = InstructionArgument::makeTempVariable(getTemporaryVariableId());
        o.type = v[2].type;
        std::string op = v[1].lexValue;
        InstructionCategory ansOp;
        if(op == "+")
            ansOp = InstructionCategory::UnaryPlus;
        else if(op == "-")
            ansOp = InstructionCategory::UnaryMinus;
        else assert(0);

        generateCode(ansOp,
            v[2].addr,
            InstructionArgument::makeEmpty(),
            o.addr);
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
