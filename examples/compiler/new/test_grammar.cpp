
#include <bits/stdc++.h>
#include <lz/debug.h>
#include <lz/compiler/new/grammar.h>
#include <lz/compiler/new/grammar_design.h>

using namespace lz;
using namespace std;



void testGrammar()
{
    GrammarFactory<char, int> gf;
    auto all = gf.makeNonternimals<3>();
    auto S = std::get<0>(all);
    auto A = std::get<1>(all);
    auto B = std::get<2>(all);
    vector<string > nonterminalNames = {"S", "A", "B"};

    auto x = [](auto v)
    {
        cout << "SB222" << endl;
    };


    S = A >> 'a';
    A = 'b';
    A = eps;

    vector<Set > firstSets = calculateFirstSets(gf.g);

    for(Set firstSet: firstSets)
    {
        for(Symbol s: firstSet)
        {
            cout << SymbolForOutput<char>{s, nonterminalNames, gf.calculateTerminalNames()};
        }
        cout << endl;
    }



    cout << GrammerForOutput<char>{gf.g, nonterminalNames, gf.calculateTerminalNames()} ;


    for(auto o: gf.actions)
    {
        o(std::vector<int>());
    }
//    gf.actions[0](std::vector<int>());




}

int main()
{
    testGrammar();
	return 0;
}
