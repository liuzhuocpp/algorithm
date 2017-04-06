#include <bits/stdc++.h>
#include <lz/debug.h>
#include <lz/std_utility.h>
#include <lz/compiler/grammar.h>

#include <lz/compiler/noterminal_proxy.h>

#include <lz/compiler/grammar_design.h>
#include <lz/compiler/slr1_grammar.h>
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
//    auto nonterminalMap = makeIteratorMap(nonterminalNames.begin());

    auto terminalMap = gf.getIndexToTerminalMap();


    auto startRule = makeAugmentedGrammar(g, 0);
    cout << "startRule " << startRule.head << " " << startRule.body << endl;

    cout << GrammerForOutput<decltype(g),decltype(nonterminalMap), decltype(terminalMap)>
        {g, nonterminalMap, terminalMap} ;


    auto result = makeItemSets(g, startRule);

    auto itemSets = get<0>(result);
    auto itemSetToId = get<1>(result);
    auto gotoFunction = get<2>(result);





    cout << string(100, '*') << endl;
    cout << "Item set number: " << itemSets.size() << endl << endl;;
    int counter = 0;
    for(auto itemSet: itemSets)
    {
        cout << "I" << counter ++ << ": " << endl;
        for(auto item: itemSet)
        {
            cout << ItemDescriptorForOutput<Grammar<P>, decltype(nonterminalMap), decltype(terminalMap)>
                {item, g, nonterminalMap, terminalMap} << endl;
        }

        if(outputNonkernelItem)
        {
            cout << string(10, '~') << endl;
            for(auto item: calculateNonkernelItemSetClosure(g, itemSet))
            {
                cout << ItemDescriptorForOutput<Grammar<P>, decltype(nonterminalMap), decltype(terminalMap) >
                    {item, g, nonterminalMap, terminalMap} << endl;
            }

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


    auto actionTableOption = calculateSLR1ActionTable(g, startRule, itemSets, gotoFunction);

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
                << " " <<endl;
    }


    auto terminalToIndexMap = gf.getTerminalToIndexMap();

    using TerminalIterator = TerminalIndexIterator<InputIterator, decltype(terminalToIndexMap) >;



    TerminalIterator begin(first, terminalToIndexMap);
    TerminalIterator end(last, terminalToIndexMap);

    cout << endl << endl << "Begin parsing..." << endl << endl;
    auto ans = parseLRGrammar(begin, end,  g, actionTableOption.value(), gotoFunction, gf.getIndexToTerminalMap(),
            nonterminalMap);

    if constexpr(!std::is_same<P, NoProperty>::value)
    {
        cout << "\n\nanswer: " << ans << endl;
    }

}




void testParseSLR1AmbiguousGrammar()
{
    OUT_FUNCTION_NAME
    using P = int;

    NonterminalProxy<char, P> S;

    GrammarFactory<char, P> gf(S);
    vector<string > nonterminalNames = {
            "S",
            "S'", };




//    S = S >> '+' >> S     > '+' > '-' < '*' < '/';
//    S = S >> '-' >> S     > '+' > '-' < '*' < '/';
//    S = S >> '*' >> S     > '+' > '-' > '*' > '/';
//    S = S >> '/' >> S     > '+' > '-' > '*' > '/';
//    S = '-' >> S          > '+' > '-' > '*' > '/';
//    S = '+' >> S          > '+' > '-' > '*' > '/';
//    S = '(' >> S >> ')';

    S[( [](const vector<P>& p, P& ans) {
            ans = p[0] + p[1];
        })]

      = S >> '+' >> S > '+';

    S[( [](const vector<P>& p, P& ans) {
        ans = 1;
        })]

    = '1';

    S[( [](const vector<P>& p, P& ans) {
        ans = 2;
        })]

    = '2';


    string text;
    text = "1+1+1+1";

    runParseSLR1Grammar(text.begin(), text.end(), gf, makeIteratorMap(nonterminalNames.begin()), true );

}



















int main()
{
    testParseSLR1AmbiguousGrammar();
    return 0;
}
