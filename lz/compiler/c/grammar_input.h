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

#include <lz/compiler/c/utility.h>


namespace lz {

struct GrammarInput
{
private:
    GrammarInput() = delete;

    static IdentifierTable& identifierTable() { return grammarInputData.m_identifierTable; }
    static TypeTable& typeTable() { return grammarInputData.m_typeTable; }
    static ThreeAddressCode& threeAddressCode() { return grammarInputData.m_threeAddressCode; }

    static std::ofstream& outStream() { return grammarInputData.outStream; }
    static std::ofstream& errorOfstream() { return grammarInputData.errorStream; }

    static int& offset() { return grammarInputData.offset; }


    using P = Properties;
    using T = LexicalSymbol;
    using PIT = std::vector<P>::iterator;

    using InstructionCategory = ThreeAddressInstruction::Category;
    using InstructionArgument = ThreeAddressInstructionArgument;
    using InstructionArgumentType = ThreeAddressInstructionArgumentType;
    using InstructionArgumentTypeCategory = ThreeAddressInstructionArgumentType::Category;



public:
    static void initData(const std::string & outFileName, const std::string & errorFileName)
    {
        TemporaryVariableNumberGenerator::reset();
        identifierTable().clear();
        threeAddressCode().clear();
        typeTable().clear();
        offset() = 0;

        outStream().open(outFileName, std::ofstream::out);
        errorOfstream().open(errorFileName, std::ofstream::out);
    }

    static void finalizeData()
    {
        outStream() << threeAddressCode();
        outStream().close();
        errorOfstream().close();
    }


    static GrammarFactory<T, P> build()
    {

        NonterminalProxy<T, P>
            LZ_NONTERMINAL_PROXY(program),
            LZ_NONTERMINAL_PROXY(declare),
            LZ_NONTERMINAL_PROXY(typeDeclare),
            LZ_NONTERMINAL_PROXY(baseTypeDeclare),

            LZ_NONTERMINAL_PROXY(statement),
            LZ_NONTERMINAL_PROXY(statementList),

            LZ_NONTERMINAL_PROXY(function),

            LZ_NONTERMINAL_PROXY(conditionMark),
            LZ_NONTERMINAL_PROXY(elseConditionMark),

            LZ_NONTERMINAL_PROXY(expression),
            LZ_NONTERMINAL_PROXY(arrayExpression)

            ;

        GrammarFactory<T, P> gf(program);

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

        program = function >> program;

        function = typeDeclare >> Lex::Identifier >> "(" >> ")" >> "{" >>
            [&](PIT v, P&o ) {

                offset() = 0;
                int functionId = identifierTable().insert(v[2].lexValue, TypeCategory::Function);
                threeAddressCode().beginFunction(functionId);

            } >> statementList >> "}" >>

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

                threeAddressCode().endFunction();
            };

        declare = typeDeclare >> Lex::Identifier >> ";" >>
            [&](PIT v, P& o) {
                int insertedId = identifierTable().insert(v[2].lexValue, v[1].type);

                InstructionArgumentType argType = newArgType(v[1].type);
                threeAddressCode().addGlobalArgument(InstructionArgument::makeVariable(insertedId), argType);


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
                threeAddressCode().backPatch(v[1].nextList, v[2].cntInstructionIndex);

                o.breakList = merge(v[1].breakList, v[3].breakList);
                o.continueList = merge(v[1].continueList, v[3].continueList);
            };

        statementList = eps ;

        statement = eps >> Lex::Break >> ";" >>
            [&](PIT v, P& o) {
                o.breakList.push_back(threeAddressCode().nextInstructionIndex());
                threeAddressCode().generateGotoCode();
            };

        statement = eps >> Lex::Continue >> ";" >>
            [&](PIT v, P& o) {
                o.continueList.push_back(threeAddressCode().nextInstructionIndex());
                threeAddressCode().generateGotoCode();
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
                threeAddressCode().backPatch(v[3].trueList, v[5].cntInstructionIndex);
                o.nextList = merge(v[3].falseList, v[6].nextList);


                o.breakList = v[6].breakList;
                o.continueList = v[6].continueList;
            } < Lex::Else;

        statement = eps >> Lex::If >> "(" >> expression >> ")" >> conditionMark >> statement >> Lex::Else >> elseConditionMark >> statement >>
            [&](PIT v, P&o) {

                auto& condition = v[3], &conMark = v[5], &ifStatement = v[6], &elseConMark = v[8], &elseStatement = v[9];

                threeAddressCode().backPatch(condition.trueList, conMark.cntInstructionIndex);
                threeAddressCode().backPatch(condition.falseList, elseConMark.cntInstructionIndex);
                merge(o.nextList, ifStatement.nextList);
                merge(o.nextList, elseConMark.nextList);
                merge(o.nextList, elseStatement.nextList);


                o.breakList = merge(ifStatement.breakList, elseStatement.breakList);
                o.continueList = merge(ifStatement.continueList, elseStatement.continueList);
            };

        statement = eps >> Lex::While >> "(" >> conditionMark >>  expression >> ")" >> conditionMark >> statement >>
            [&](PIT v, P&o) {

                threeAddressCode().backPatch(v[4].trueList, v[6].cntInstructionIndex);
                merge(o.nextList, v[4].falseList);
                threeAddressCode().backPatch(v[7].nextList, threeAddressCode().nextInstructionIndex());
                threeAddressCode().generateGotoCode(v[3].cntInstructionIndex);

                threeAddressCode().backPatch(v[7].breakList, threeAddressCode().nextInstructionIndex());
                threeAddressCode().backPatch(v[7].continueList, v[3].cntInstructionIndex);
            };

        conditionMark = eps >>
            [&](PIT v, P&o) {
                o.cntInstructionIndex = threeAddressCode().nextInstructionIndex();
            };

        elseConditionMark = eps >>
            [&](PIT v, P& o) {

                o.nextList.push_back(threeAddressCode().nextInstructionIndex());
                threeAddressCode().generateGotoCode();
                o.cntInstructionIndex = threeAddressCode().nextInstructionIndex();
            };


        expression = Lex::Identifier >> "(" >> ")" >>
            [&](PIT v, P& o) {

                o.type = TypeCategory::Void;// 目前不支持返回值

                threeAddressCode().generateCallCode(identifierTable().find(v[1].lexValue));

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
                        threeAddressCode().backPatch(v[1].falseList, v[3].cntInstructionIndex);
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

                        threeAddressCode().backPatch(v[1].trueList, v[3].cntInstructionIndex);
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
                o.addr = InstructionArgument::makeNumber(1);
                o.type = TypeCategory::Bool;
            };

        expression = Lex::False >>
            [&](PIT v, P&o) {
                o.addr = InstructionArgument::makeNumber(0);
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
                        threeAddressCode().generateAssignCode(readAddr(v[3]), v[1].addr);
                    }
                    else
                    {
                        threeAddressCode().generateWriteArrayCode(readAddr(v[3]), v[1].addr, v[1].arrayId);
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
                    o.addr = generateCalculateArrayOffsetCode(v[3], o.type);

                });
            };

        arrayExpression = arrayExpression >> "[" >> expression >> "]" >>
            [&](PIT v, P &o) {

                o.arrayId = v[1].arrayId;
                o.type = typeTable().subarrayType(v[1].type);

                auto tmpArg1 = generateCalculateArrayOffsetCode(v[3], o.type);
                auto tmpArg2 = InstructionArgument::makeTempVariable(getTemporaryVariableId());
                threeAddressCode().addArgument(tmpArg2, newArgType(InstructionArgumentTypeCategory::Int64));
                threeAddressCode().generateBinaryArithmeticCode('+', v[1].addr, tmpArg1, tmpArg2);

                o.addr = tmpArg2;
            };

        return gf;

    }

private:

    static InstructionArgument generateCalculateArrayOffsetCode(P& arrayOffsetP, TypeDescriptor arrayType)
    {
        InstructionArgument arrayOffset = readAddr(arrayOffsetP);
        auto tmpArg1 = InstructionArgument::makeTempVariable(getTemporaryVariableId());
        auto tmpArgType1 = newArgType(InstructionArgumentTypeCategory::Int64);
        threeAddressCode().addArgument(tmpArg1, tmpArgType1);
        threeAddressCode().generateBinaryArithmeticCode('*', arrayOffset,
            InstructionArgument::makeNumber(typeTable().getWidth(arrayType)), tmpArg1);

        return tmpArg1;
    }
    static InstructionArgumentType newArgType(InstructionArgumentTypeCategory cate, int arrayWidth = -1)
    {
        InstructionArgumentType ans(cate, offset());

        if(cate == InstructionArgumentTypeCategory::Array)
        {
            assert(arrayWidth > 0);
            ans.setArrayWidth(arrayWidth);
        }
        offset() += ans.getWidth();
        return ans;
    }

    static InstructionArgumentType newArgType(TypeDescriptor i)
    {
        InstructionArgumentTypeCategory c;

        int arrayWidth = -1;
        switch(i.category())
        {
        case TypeCategory::Bool:
            c = InstructionArgumentTypeCategory::Bool;
            break;
        case TypeCategory::Int :
            c = InstructionArgumentTypeCategory::Int32;
            break;
        case TypeCategory::Array :
            c = InstructionArgumentTypeCategory::Array;
            arrayWidth = typeTable().getWidth(i);
            break;
        case TypeCategory::Float :
            c = InstructionArgumentTypeCategory::Float;
            break;
        case TypeCategory::Double :
            c = InstructionArgumentTypeCategory::Double;
            break;
        default:
            assert(0);
        }

        return newArgType(c, arrayWidth);

    }

    // Bellow function must be static,
    static void solveArithmeticOperator (PIT v, P &o)
    {
        checkTypeEquality(v[1].type, v[3].type, [&]() {
            o.type = v[1].type;


            InstructionArgument firstArg = readAddr(v[1]), secondArg = readAddr(v[3]);
            auto tmpArg = InstructionArgument::makeTempVariable(getTemporaryVariableId());
            threeAddressCode().addArgument(tmpArg, newArgType(o.type ) );
            o.addr = tmpArg;


            threeAddressCode().generateBinaryArithmeticCode(v[2].lexValue[0], firstArg, secondArg, tmpArg);
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
            auto tmpArg = InstructionArgument::makeTempVariable(getTemporaryVariableId());

            TypeDescriptor arrayType = identifierTable().type(p.arrayId);


            threeAddressCode().addArgument(tmpArg, newArgType(typeTable().arrayBaseType(arrayType)));

            threeAddressCode().generateReadArrayCode(p.arrayId, p.addr, tmpArg);

            return tmpArg;
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

            o.trueList.push_back(threeAddressCode().nextInstructionIndex());

            threeAddressCode().generateIfRelCode(v[2].lexValue, readAddr(v[1]), readAddr(v[3]));
            o.falseList.push_back(threeAddressCode().nextInstructionIndex());
            threeAddressCode().generateGotoCode();
            o.type = TypeCategory::Bool;
        });

    }

    static void solveUnaryPlusOrMinusOperator(PIT v, P &o)
    {
        o.addr = InstructionArgument::makeTempVariable(getTemporaryVariableId());
        o.type = v[2].type;

        threeAddressCode().generateUnaryArithmeticCode(v[1].lexValue[0], readAddr(v[2]), o.addr);
    }




};










} // namespace lz





#endif /* LZ_COMPILER_C_GRAMMAR_INPUT_H_ */
