
#include <bits/stdc++.h>
#include <lz/debug.h>
#include <lz/compiler/new/grammar.h>

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

    S = A >> 'z' >> 'd';
    S = A >> 'z' >> S;
    S = 'a' >> A;
    S = 't';
    S = A;
    S = eps;

    S = eps >> 'a' >> 'b' >> 'z';


    S = A >> 'a';
    S = A >> 'a' >> B;
    S = eps;
    A = eps;

    B = 'a';

    B = eps >> 'a' >> 'b' >> 'z' >> 'k' >> 'h' >> '&';
    B[x] = A[x] >> 'a'  >> 'z';


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
