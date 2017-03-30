#include <bits/stdc++.h>
#include <lz/debug.h>
#include <lz/std_utility.h>
#include <lz/compiler/grammar.h>

#include <lz/compiler/noterminal_proxy.h>

#include <lz/compiler/grammar_design.h>
#include <lz/compiler/slr1_grammar.h>





using namespace lz;
using namespace std;

template<typename T1, typename T2>
void outVectorSet(vector<Set > vectorSets, T1 nonterminalNames, T2 terminalNames)
{

    for(Set vectorSet: vectorSets)
    {
        for(SymbolDescriptor s: vectorSet)
        {
            cout << SymbolForOutput<char>{s, nonterminalNames, terminalNames} << ' ';
        }
        cout << "|" <<endl;
    }

}




void testCalculateSLR1GrammarGoto()
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
    auto startRule = makeAugmentedGrammar(g, 0);

    cout << "startRule " << startRule.head << " " << startRule.body << endl;

    cout << GrammerForOutput<char,decltype(g)>{g, nonterminalNames, gf.calculateTerminalNames()} ;

    string text = "1+1+1+1";



    auto result = makeItemSets(g, startRule);

    auto itemSets = get<0>(result);
    auto itemSetToId = get<1>(result);
    auto gotoFunction = get<2>(result);

    cout << string(100, '*') << endl;
    cout << "Item set number: " << itemSets.size() << endl;


    int counter = 0;
    for(auto itemSet: itemSets)
    {
        cout << "I" << counter++ << ":" << endl;
        for(auto item: itemSet)
        {
            cout << ItemDescriptorForOutput<Grammar<P>, char>{g, item, nonterminalNames, gf.calculateTerminalNames()} << endl;
        }
        cout << string(10, '~') << endl;
        for(auto item: calculateNonkernelItemSetClosure(g, itemSet))
        {
            cout << ItemDescriptorForOutput<Grammar<P>, char>{g, item, nonterminalNames, gf.calculateTerminalNames()} << endl;
        }
        cout << string(50, '-') << endl;
    }
}



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
    auto startRule = makeAugmentedGrammar(g, 0);
    cout << "startRule " << startRule.head << " " << startRule.body << endl;
    cout << GrammerForOutput<char,decltype(g)>{g, nonterminalNames, gf.calculateTerminalNames()} ;


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
            cout << ItemDescriptorForOutput<Grammar<P>, char>{g, item, nonterminalNames, gf.calculateTerminalNames()} << endl;
        }
        cout << string(10, '~') << endl;
        for(auto item: calculateNonkernelItemSetClosure(g, itemSet))
        {
            cout << ItemDescriptorForOutput<Grammar<P>, char>{g, item, nonterminalNames, gf.calculateTerminalNames()} << endl;
        }
        cout << string(50, '-') << endl;
    }


    cout << string(100, '*') << "gotoFunction" << endl;

    for(auto _pair: gotoFunction)
    {

        cout << _pair.first.first << " " <<
                SymbolForOutput<char>{_pair.first.second, nonterminalNames, gf.calculateTerminalNames()} << " "
                << _pair.second << endl;
    }


    auto actionTableOption = calculateSLR1ActionTable(g, startRule, itemSets, gotoFunction);

    cout << string(100, '*') << "actionTable" << endl;

    for(auto _pair: actionTableOption.value())
    {

        cout << _pair.first.first << " " <<
                SymbolForOutput<char>{_pair.first.second, nonterminalNames, gf.calculateTerminalNames()} << " "
//                << _pair.second
                <<endl;
    }


    cout << "Is SLR(1) grammar? " << std::boolalpha << actionTableOption.hasValue() << endl;





    string text = "a*a";
    parseSLR1Grammar(text.begin(), text.end(), gf.terminalMap, g, actionTableOption.value(), gotoFunction);










}


int main()
{
//    testCalculateSLR1GrammarGoto();
    testParseSLR1Grammar();
    return 0;
}
