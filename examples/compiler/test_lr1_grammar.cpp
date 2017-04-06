#include <bits/stdc++.h>
#include <lz/debug.h>
#include <lz/std_utility.h>
#include <lz/compiler/grammar.h>

#include <lz/compiler/noterminal_proxy.h>

#include <lz/compiler/grammar_design.h>
#include <lz/compiler/lr1_grammar.h>
#include <lz/compiler/lr_grammar.h>





using namespace lz;
using namespace std;

template<typename InputIterator, typename T, typename P, typename NonterminalNameMap>
void runParseSLR1Grammar(
        InputIterator first,
        InputIterator last,
        GrammarFactory<T, P> &gf,
        NonterminalNameMap nonterminalMap,
        bool outputNonkernelItem = true)
{
    Grammar<P> g = gf.g;

    auto terminalMap = gf.getIndexToTerminalMap();


    auto startRule = makeAugmentedGrammar(g, 0);
    cout << "startRule " << startRule.head << " " << startRule.body << endl;

    cout << GrammerForOutput<decltype(g),decltype(nonterminalMap), decltype(terminalMap)>
        {g, nonterminalMap, terminalMap} ;


    auto result = makeExtendItemSets(g, startRule);

    auto itemSets = get<0>(result);
//    auto itemSetToId = get<1>(result);
    auto gotoFunction = get<1>(result);





    cout << string(100, '*') << endl;
    cout << "Item set number: " << itemSets.size() << endl << endl;;
    int counter = 0;
    for(auto itemSet: itemSets)
    {
        cout << "I" << counter ++ << ": " << endl;
        for(auto item: itemSet)
        {
            cout << ExtendItemDescriptorForOutput<Grammar<P>, decltype(nonterminalMap), decltype(terminalMap)>
                {item, g, nonterminalMap, terminalMap} << endl;
        }

        cout << string(20, '-') << endl;
        cout << endl;
        cout << endl;
    }


    cout << string(100, '*') << "gotoFunction" << endl;

    for(auto _pair: gotoFunction)
    {
        cout << _pair.first.first
            << " "
            << SymbolForOutput<SymbolDescriptor, decltype(nonterminalMap), decltype(terminalMap)>
                {_pair.first.second, nonterminalMap, terminalMap} << " "
            << _pair.second << endl;
    }


    auto actionTableOption = calculateLR1ActionTable(g, startRule, itemSets, gotoFunction);

    cout << "Is SLR(1) grammar? " << std::boolalpha << actionTableOption.hasValue() << endl;
    if(!actionTableOption.hasValue())
    {
        return ;
    }

    cout << string(100, '*') << "actionTable" << endl;



    for(auto _pair: actionTableOption.value())
    {

        cout << _pair.first.first << " " <<
                SymbolForOutput<SymbolDescriptor, decltype(nonterminalMap), decltype(terminalMap)>
                    {_pair.first.second, nonterminalMap, terminalMap}
                << " "
                <<endl;
    }






    auto terminalToIndexMap = gf.getTerminalToIndexMap();

    using TerminalIterator = TerminalIndexIterator<InputIterator, decltype(terminalToIndexMap) >;



    TerminalIterator begin(first, terminalToIndexMap);
    TerminalIterator end(last, terminalToIndexMap);


    parseSLR1Grammar(begin, end,  g, actionTableOption.value(), gotoFunction, gf.getIndexToTerminalMap(),
            nonterminalMap);

}


void testParseSLR1AmbiguousGrammar()
{
    OUT_FUNCTION_NAME
    using P = NoProperty;

    NonterminalProxy<char, P> S, M, M1, M2, C;

    GrammarFactory<char, P> gf(S);
    vector<string > nonterminalNames = {
            "S",
            "M",
            "S'", };

    S = 'a';
    S = S >> M >>  '+' >> S > '+';
    M = eps;
    string text ;
    text = "a+a";




//    GrammarFactory<char, P> gf(S);
//    vector<string > nonterminalNames = {
//            "S",
//            "C",
//            "S'", };
//
//    S = C >> C;
//    C = 'c' >> C;
//    C = 'd';
//    string text = "------a+(a-a+a)/--+++++---a/a*a";
//    text = "cd";









    runParseSLR1Grammar(text.begin(), text.end(), gf, makeIteratorMap(nonterminalNames.begin()), true );;

}



















int main()
{
    testParseSLR1AmbiguousGrammar();
    return 0;
}
