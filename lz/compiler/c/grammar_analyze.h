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

int getTemporaryVariableIndex()
{
    static int temporaryVariable = 0;

    return temporaryVariable++;
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
    auto generateCode = [&](std::string op, std::string arg1, std::string arg2, std::string res)
    {
        outText << op << " " << arg1 << " " << arg2 << " " << res << std::endl;;
    };



    identifierTable.clear();

    using P = Properties;
    using T = LexicalSymbol;


    NonterminalProxy<T, P> program, declare, expression, subexpression, operateExpression;
    GrammarFactory<T, P> gf(program);

    program = declare >> program;
    program = expression >> program;
    program = declare;
    program = expression;

    declare = eps >> "int" >> LexicalSymbol::Type::Identifier >> ";" >>
        [&](auto v, P& o) {
            std::string identifierName = v[2].addr;
            insertIdentifierTable(Identifier::Type::Int, identifierName);
    };


    declare = eps >> "float" >> LexicalSymbol::Type::Identifier >> ";" >>
        [&](auto v, P&o) {
        std::string identifierName = v[2].addr;
        insertIdentifierTable(Identifier::Type::Float, identifierName);
    };

    expression = subexpression >> ";";

    subexpression = eps >> LexicalSymbol::Type::Identifier >> "=" >> subexpression >>
        [&](auto v, P&o) {

        std::string name = v[1].addr;
        if(identifierTable.count(Identifier(name)))
        {
            int identifierId = identifierTable[Identifier(name)];
            generateCode("=", "a"+std::to_string(identifierId), v[3].addr, "");
        }
        else
        {
            cout << "\"" << name << "\" was not declare \n";
        }


    };
    subexpression = operateExpression >>
        [&](auto v, P&o) {
        o.addr = v[1].addr;
    };


    operateExpression = operateExpression >> "+" >> operateExpression >>
        [&](auto v, P&o) {
        int newTempIndex = getTemporaryVariableIndex();
        o.addr = "t"+std::to_string(newTempIndex);
        generateCode("+", v[1].addr, v[3].addr, o.addr) ;


    } > "+" > "-";

    operateExpression = operateExpression >> "-" >> operateExpression >>
        [&](auto v, P&o) {

        int newTempIndex = getTemporaryVariableIndex();
        o.addr = "t"+std::to_string(newTempIndex);
        generateCode("-", v[1].addr, v[3].addr, o.addr) ;


    } > "+" > "-";

    operateExpression = LexicalSymbol::Type::Integer >>
        [&](auto v, P&o) {


            o.addr = v[1].addr;



        };
    operateExpression = LexicalSymbol::Type::Identifier >>
            [&](auto v, P&o) {
                std::string name = v[1].addr;

                if(identifierTable.count(Identifier(name)))
                {
                    o.addr = "a" + std::to_string(identifierTable[Identifier(name)]);
                }
                else
                {
                    cout << "\"" << name << "\" was not declare \n";

                }





            };
;





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


}

} // namespace lz





#endif /* LZ_COMPILER_C_GRAMMAR_ANALYZE_H_ */
