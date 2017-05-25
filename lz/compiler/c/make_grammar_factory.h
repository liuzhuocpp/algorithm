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

        LZ_NONTERMINAL_PROXY(functionDefination),

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

        program = eps;

        program = declare >> program;



        program = functionDefination >> program;

        functionDefination = typeDeclare >> Lex::Identifier >> "(" >> ")" >> "{" >> statementList >> "}" >>
            [&](PIT v, P& o) {
                P& statementListP = v[6];

                if(!statementListP.breakList.empty())
                {
                    errorOfstream() << "break statement not within a loop\n";
                }

                if(!statementListP.continueList.empty())
                {
                    errorOfstream() << "continue statement not within a loop\n";
                }
            };

//
//        program = statementList >>
//            [&](PIT v, P& o) {
//                if(!v[1].breakList.empty())
//                {
//                    errorOfstream() << "break statement not within a loop\n";
//                }
//
//                if(!v[1].continueList.empty())
//                {
//                    errorOfstream() << "continue statement not within a loop\n";
//                }
//            };

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

                o.type = typeTable().makeArray(v[1].type, std::stoi(v[3].lexValue));
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
        baseTypeDeclare = Lex::Void >>
            [&](PIT v, P&o) {
                o.type = TypeCategory::Void;
            };

        statementList = statement >> conditionMark >> statementList >>
            [&](PIT v, P&o) {
                codeTable().backPatch(v[1].nextList, v[2].cntInstructionIndex);

                o.breakList = merge(v[1].breakList, v[3].breakList);
                o.continueList = merge(v[1].continueList, v[3].continueList);
            };

        statementList = eps ;

        statement = eps >> Lex::Break >> ";" >>
            [&](PIT v, P& o) {
                o.breakList.push_back(codeTable().nextInstructionIndex());
                codeTable().generateGotoCode();
            };

        statement = eps >> Lex::Continue >> ";" >>
            [&](PIT v, P& o) {
                o.continueList.push_back(codeTable().nextInstructionIndex());
                codeTable().generateGotoCode();
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
                codeTable().backPatch(v[3].trueList, v[5].cntInstructionIndex);
                o.nextList = merge(v[3].falseList, v[6].nextList);


                o.breakList = v[6].breakList;
                o.continueList = v[6].continueList;
            } < Lex::Else;

        statement = eps >> Lex::If >> "(" >> expression >> ")" >> conditionMark >> statement >> Lex::Else >> elseConditionMark >> statement >>
            [&](PIT v, P&o) {

                auto& condition = v[3], &conMark = v[5], &ifStatement = v[6], &elseConMark = v[8], &elseStatement = v[9];

                codeTable().backPatch(condition.trueList, conMark.cntInstructionIndex);
                codeTable().backPatch(condition.falseList, elseConMark.cntInstructionIndex);
                merge(o.nextList, ifStatement.nextList);
                merge(o.nextList, elseConMark.nextList);
                merge(o.nextList, elseStatement.nextList);


                o.breakList = merge(ifStatement.breakList, elseStatement.breakList);
                o.continueList = merge(ifStatement.continueList, elseStatement.continueList);
            };

        statement = eps >> Lex::While >> "(" >> conditionMark >>  expression >> ")" >> conditionMark >> statement >>
            [&](PIT v, P&o) {

                codeTable().backPatch(v[4].trueList, v[6].cntInstructionIndex);
                merge(o.nextList, v[4].falseList);
                codeTable().backPatch(v[7].nextList, codeTable().nextInstructionIndex());
                codeTable().generateGotoCode(v[3].cntInstructionIndex);

                codeTable().backPatch(v[7].breakList, codeTable().nextInstructionIndex());
                codeTable().backPatch(v[7].continueList, v[3].cntInstructionIndex);
            };

        conditionMark = eps >>
            [&](PIT v, P&o) {
                o.cntInstructionIndex = codeTable().nextInstructionIndex();
            };

        elseConditionMark = eps >>
            [&](PIT v, P& o) {

                o.nextList.push_back(codeTable().nextInstructionIndex());
                codeTable().generateGotoCode();
                o.cntInstructionIndex = codeTable().nextInstructionIndex();
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
                        codeTable().backPatch(v[1].falseList, v[3].cntInstructionIndex);
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

                        codeTable().backPatch(v[1].trueList, v[3].cntInstructionIndex);
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

        expression = expression >> "=" >> expression >>
            [&](PIT v, P&o) { // need improve

                checkTypeEquality(v[1].type, v[3].type, [&]() {

                    if(v[1].arrayId == -1)
                    {
                        codeTable().generateCode(InstructionCategory::Assign,
                            readAddr(v[3]),
                            InstructionArgument::makeEmpty(),
                            v[1].addr);
                    }
                    else
                    {
                        codeTable().generateCode(InstructionCategory::WriteArray,
                            readAddr(v[3]),
                            v[1].addr,
                            InstructionArgument::makeVariable(v[1].arrayId));
                    }

                    o.arrayId = v[1].arrayId;
                    o.addr = v[1].addr;
                    o.type = v[1].type;
                });

            };

        expression = arrayExpression >>
            [&](PIT v, P &o){

                o.arrayId = v[1].arrayId;
                o.addr = v[1].addr;
                o.type = v[1].type;
            };

        arrayExpression = Lex::Identifier >> "[" >> expression >> "]" >>
            [&](PIT v, P &o){

                checkVariableDeclare(v[1].lexValue, [&](int identifierId) {


                    TypeDescriptor arrayType = identifierTable().type(identifierId); // 数组类型

                    o.arrayId = identifierId;
                    o.type = typeTable().subarrayType(arrayType);

                    InstructionArgument arrayOffset = readAddr(v[3]);

                    unsigned tmp = getTemporaryVariableId();
                    codeTable().generateCode(InstructionCategory::Multiply,
                        arrayOffset,
                        InstructionArgument::makeNumber(typeTable().getWidth(o.type)),
                        InstructionArgument::makeTempVariable(tmp));


                    o.addr =  InstructionArgument::makeTempVariable(tmp); // 数组偏移量



                });
            };

        arrayExpression = arrayExpression >> "[" >> expression >> "]" >>
            [&](PIT v, P &o) {

                o.arrayId = v[1].arrayId;
                o.type = typeTable().subarrayType(v[1].type);

                InstructionArgument arrayOffset = readAddr(v[3]);
                unsigned tmp1 = getTemporaryVariableId();
                codeTable().generateCode(InstructionCategory::Multiply,
                    arrayOffset,
                    InstructionArgument::makeNumber(typeTable().getWidth(o.type)),
                    InstructionArgument::makeTempVariable(tmp1) );

                unsigned tmp2 = getTemporaryVariableId();
                codeTable().generateCode(InstructionCategory::Plus,
                    v[1].addr,
                    InstructionArgument::makeTempVariable(tmp1),
                    InstructionArgument::makeTempVariable(tmp2));

                o.addr = InstructionArgument::makeTempVariable(tmp2);
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

            InstructionArgument firstArg = readAddr(v[1]), secondArg = readAddr(v[3]);

            o.addr = InstructionArgument::makeTempVariable(getTemporaryVariableId());
            o.type = v[1].type;

            codeTable().generateCode(ThreeAddressInstruction::toCategory(v[2].lexValue),
                firstArg,
                secondArg,
                o.addr);
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


    static InstructionArgument readAddr(const Properties& p)
    {
        if(p.arrayId != -1)
        {
            unsigned tmp = getTemporaryVariableId();
            codeTable().generateCode(InstructionCategory::ReadArray,
                InstructionArgument::makeVariable(p.arrayId),
                p.addr,
                InstructionArgument::makeTempVariable(tmp));

            return InstructionArgument::makeTempVariable(tmp);
        }
        else
        {
            return p.addr;
        }
    }

    static std::list<int> merge(std::list<int>&a1, std::list<int>&a2)
    {
        a1.splice(a1.end(), a2);
        return a1;
    }

    static void solveRelationalOperator(PIT v, P &o)
    {
        checkTypeEquality(v[1].type, v[3].type, [&]() {

            o.trueList.push_back(codeTable().nextInstructionIndex());
            codeTable().generateCode(ThreeAddressInstruction::toIfRel(v[2].lexValue),
                readAddr(v[1]),
                readAddr(v[3]),
                InstructionArgument::makeEmpty());
            o.falseList.push_back(codeTable().nextInstructionIndex());
            codeTable().generateGotoCode();
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

        codeTable().generateCode(ansOp,
            readAddr(v[2]),
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
