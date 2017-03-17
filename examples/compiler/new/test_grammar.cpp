
#include <bits/stdc++.h>
#include <lz/debug.h>
#include <lz/std_utility.h>
#include <lz/compiler/new/grammar.h>
#include <lz/compiler/new/grammar_design.h>
#include <lz/compiler/new/ll1_grammar.h>

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







void testParseLL1Grammar()
{
    OUT_FUNCTION_NAME

    GrammarFactory<char, int> gf;
    auto all = gf.makeNonternimals<2>();
    auto S = std::get<0>(all);
    auto A = std::get<1>(all);
    vector<string > nonterminalNames = {"S", "A", "B"};

    using P = int;


    S
    [([](const vector<P>& v, P &ans) {
        ans = v[0] + 1;
    })]
    = '1' >> A

    [([](const vector<P>& v, P &ans) {
//             cout << "kkk" << endl;
         })]
     ;


    S
    [([](const vector<P>& v, P &ans) {
        ans = v[0] + 2;
    })]
    = '2' >> A;


    A
    [([](const vector<P>& v, P &ans) {
        ans = v[0];
    })]
    = '+' >> S;


    A
    [([](const vector<P>& v, P &ans) {
        ans = 0;
    })]
    = eps;

    cout << "action size: " << gf.g.actions.size() << endl;

    cout << GrammerForOutput<char,decltype(gf.g)>{gf.g, nonterminalNames, gf.calculateTerminalNames()} ;


    auto firstSets = calculateFirstSets(gf.g);
    auto followSets = calculateFollowSets(gf.g, firstSets);
    cout << string(100, '-') << endl;
    outVectorSet(firstSets, nonterminalNames, gf.calculateTerminalNames());
    cout << string(100, '-') << endl;
    outVectorSet(followSets, nonterminalNames, gf.calculateTerminalNames());
    cout << string(100, '-') << endl;
    cout << "isLL1 Grammar ? " << std::boolalpha << " " <<  isLL1Grammar(gf.g) << endl;;

    auto table = constructLL1Table(gf.g);


    string text = "1+2+2+1+1+1";
    parseLL1Grammar<string::iterator, Grammar<int>>(text.begin(), text.end(), gf.terminalMap, gf.g, table);


}
















int main()
{
//    testGrammar();
//    testRules();
//    testLL1Table();
    testParseLL1Grammar();
	return 0;
}
