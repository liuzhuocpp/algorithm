#include <bits/stdc++.h>
#include <lz/debug.h>
#include <lz/std_utility.h>
#include <lz/compiler/grammar.h>

#include <lz/compiler/noterminal_proxy.h>

#include <lz/compiler/grammar_design.h>
#include <lz/compiler/slr1_grammar.h>





using namespace lz;
using namespace std;




void testParseSLR1Grammar()
{
    OUT_FUNCTION_NAME

    using P = NoProperty;

    NonterminalProxy<char, P> S, A, B, E, T, F;



    GrammarFactory<char, P> gf(E);
    vector<string > nonterminalNames = {"E", "T", "F", "E'" };



    E = E >> '+' >> T;
    E = T;
    T = T >> '*' >> F;
    T = F;
    F = '(' >> E >> ')';
    F = 'a';




    Grammar<P> g = gf.g;
    auto nonterminalMap = makeIteratorMap(nonterminalNames.begin());

    auto terminalMap = gf.getTerminalMap();


    auto startRule = makeAugmentedGrammar(g, 0);
    cout << "startRule " << startRule.head << " " << startRule.body << endl;

    cout << GrammerForOutput<decltype(g),decltype(nonterminalMap), decltype(terminalMap)>
        {g, nonterminalMap, terminalMap} ;


    auto result = makeItemSets(g, startRule);

    auto itemSets = get<0>(result);
    auto itemSetToId = get<1>(result);
    auto gotoFunction = get<2>(result);





    cout << string(100, '*') << endl;
    cout << "Item set number: " << itemSets.size() << endl;
    int counter = 0;
    for(auto itemSet: itemSets)
    {
        cout << "I" << counter ++ << endl;
        for(auto item: itemSet)
        {
            cout << ItemDescriptorForOutput<Grammar<P>, decltype(nonterminalMap), decltype(terminalMap)>
                {item, g, nonterminalMap, terminalMap} << endl;
        }
        cout << string(10, '~') << endl;
        for(auto item: calculateNonkernelItemSetClosure(g, itemSet))
        {
            cout << ItemDescriptorForOutput<Grammar<P>, decltype(nonterminalMap), decltype(terminalMap) >
                {item, g, nonterminalMap, terminalMap} << endl;
        }
        cout << string(50, '-') << endl;
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


    auto actionTableOption = calculateSLR1ActionTable(g, startRule, itemSets, gotoFunction);

    cout << string(100, '*') << "actionTable" << endl;

    for(auto _pair: actionTableOption.value())
    {

        cout << _pair.first.first << " " <<
                SymbolForOutput<SymbolDescriptor, decltype(nonterminalMap), decltype(terminalMap)>
                    {_pair.first.second, nonterminalMap, terminalMap}
                << " "
                <<endl;
    }


    cout << "Is SLR(1) grammar? " << std::boolalpha << actionTableOption.hasValue() << endl;





    string text = "a*a";
    parseSLR1Grammar(text.begin(), text.end(), gf.terminalMap, g, actionTableOption.value(), gotoFunction);










}


int main()
{
    testParseSLR1Grammar();
    return 0;
}
