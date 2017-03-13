
#include <bits/stdc++.h>
#include <lz/debug.h>
#include <lz/compiler/new/grammar.h>

using namespace lz;
using namespace std;



void testGrammar()
{
    GrammarFactory<char> gf;
    auto all = gf.makeNonternimals<3>();
    auto S = std::get<0>(all);
    auto A = std::get<1>(all);
    auto B = std::get<2>(all);
    vector<string > nonterminalNames = {"S", "A", "B"};

    S = A >> 'a';
    S = A >> 'a' >> B;
    S = eps;
    A = eps;

    B = 'a';

    B = eps >> 'a' >> 'b' >> 'z';




    B = A >> 'a'  >> 'z';


    cout << GrammerForOutput<char>{gf.g, nonterminalNames, gf.calculateTerminalNames()} ;





}

int main()
{
    testGrammar();
	return 0;
}
