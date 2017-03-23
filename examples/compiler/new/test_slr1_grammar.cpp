#include <bits/stdc++.h>
#include <lz/debug.h>
#include <lz/std_utility.h>
#include <lz/compiler/new/grammar.h>
#include <lz/compiler/new/grammar_design.h>
#include <lz/compiler/new/slr1_grammar.h>



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



// run parsing process to see the detail
template< typename InputIterator, typename GrammarFactory, typename NonterminalNames>
auto runParseLL1Grammar(InputIterator first, InputIterator last, const GrammarFactory &gf, const NonterminalNames& nonterminalNames)
{
    using P = typename decltype(gf.g)::NodeProperties;


    cout << "action size: " << gf.g.actionsNumber() << endl;

    cout << GrammerForOutput<char,decltype(gf.g)>{gf.g, nonterminalNames, gf.calculateTerminalNames()} ;


    auto firstSets = calculateFirstSets(gf.g);
    auto followSets = calculateFollowSets(gf.g, firstSets);
    cout << "First sets:" << endl;
    outVectorSet(firstSets, nonterminalNames, gf.calculateTerminalNames());
    cout << "Follow sets:" << endl;
    outVectorSet(followSets, nonterminalNames, gf.calculateTerminalNames());
    cout << string(100, '-') << endl;
}



void testParseSLR1Grammar()
{
    OUT_FUNCTION_NAME

    using P = int;

    NonterminalProxy<char, P> S, A, B;
    GrammarFactory<char, P> gf(S);
    vector<string > nonterminalNames = {"S", "A", "B"};



    S = '1' >> A;
    A = '+' >> S;
    A = eps;

    string text = "1+1+1+1";
    Grammar<P> g = gf.g;
    auto startRule = makeAugmentedGrammar(g, 0);

    auto result = makeItemSets(g, startRule);

    auto itemSets = get<0>(result);
    auto itemSetToId = get<1>(result);
    auto gotoFunction = get<2>(result);






//    runParseLL1Grammar(text.begin(), text.end(), gf, nonterminalNames);


}


int main()
{
    testParseSLR1Grammar();
    return 0;
}
