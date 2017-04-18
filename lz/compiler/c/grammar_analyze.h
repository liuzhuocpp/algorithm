/*
 * grammar_analyze.h
 *
 *  Created on: 2017年4月18日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_C_GRAMMAR_ANALYZE_H_
#define LZ_COMPILER_C_GRAMMAR_ANALYZE_H_


#include <lz/compiler/c/lexical_analyze.h>


#include <lz/compiler/noterminal_proxy.h>
#include <lz/compiler/grammar.h>
#include <lz/compiler/slr1_grammar.h>
#include <lz/compiler/lr_grammar.h>

namespace lz {

struct Identifier
{
    enum class Type
    {
        Int,
        Float,
        Double,
        Unknown,
    };

    Type type; // int, float, double, ... 等基础类型
    std::string name; // 名字不可能相同，暂时不考虑作用域
    friend bool operator<(const Identifier &a, const Identifier &b)
    {
        return a.name < b.name;
    }

    Identifier() = default;
    Identifier(Type type, std::string name):
        type(type), name(name) {}

    Identifier(std::string name):
            type(Type::Unknown), name(name)
    {

    }

};


std::map<Identifier, int> identifierTable;


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

int insertIdentifierTable(Identifier::Type type, std::string name)
{
    if(identifierTable.count(name))
    {
        std::cout << "conflicting declaration:" << name << endl;
        assert(0);
        return -1;
    }
    else
    {
        int addr = identifierTable.size();

        identifierTable.insert(std::make_pair(
            Identifier(Identifier::Type::Int, name),
            addr ) );
        return addr;
    }

}
struct Properties
{
    std::string addr;// 若以a开头，后边的数字表示identifierTable中的下表；若是以t开头则表示临时变量；还有可能是从 LexicalSymbol 得到


    Properties() = default;
    Properties(LexicalSymbol t)
    {
        addr = t.value;

    }

};




template<typename InputIterator>
void grammarAnalyze(InputIterator first, InputIterator last)
{
    std::vector<std::string > nonterminalNames = {
            "program",
            "declare",
            "expression",
            "subexpression",
            "operateExpression",

            "program'",
    };

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









    NonterminalProxy<T, P> program, declare, expression, subexpression, operateExpression;
    GrammarFactory<T, P> gf(program);

    program = declare >> program;
    program = expression >> program;
    program = declare;
    program = expression;

    declare = eps >> "int" >> LexicalSymbol::Type::Identifier >> ";" >>
        [&](auto v, P& o) {
            insertIdentifierTable(Identifier::Type::Int, v[2].addr);
        };


    declare = eps >> "float" >> LexicalSymbol::Type::Identifier >> ";" >>
        [&](auto v, P&o) {
            insertIdentifierTable(Identifier::Type::Float, v[2].addr);
        };

    expression = subexpression >> ";";

    subexpression = eps >> LexicalSymbol::Type::Identifier >> "=" >> subexpression >>
        [&](auto v, P&o) {

            if(auto checkId = checkVariableDeclare(v[1].addr); checkId != -1)
            {
                generateCode("=", getVariableName(checkId), v[3].addr, "");
            }

        };

    subexpression = operateExpression >>
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
    auto indexToNonterminalMap = makeIteratorMap(nonterminalNames.begin());
    auto indexToTerminalMap =  gf.getIndexToTerminalMap();
    auto terminalToIndexMap =  gf.getTerminalToIndexMap();

//    std::cout << "HAHA" << std::endl;
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
