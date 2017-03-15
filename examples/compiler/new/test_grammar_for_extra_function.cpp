
#include <bits/stdc++.h>
#include <lz/debug.h>
#include <lz/std_utility.h>
#include <lz/compiler/new/grammar.h>
//#include <lz/compiler/new/grammar_design.h>
//#include <lz/compiler/new/ll1_grammar.h>

using namespace lz;
using namespace std;

using Set = std::set<SymbolDescriptor>;
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
    auto x = [](std::vector<P> v, P& ans)
    {};


    S
    [([](std::vector<P> v, P &ans) {
        ans = v[0] + 1;
    })]
     = '1' >> A;


    S
    [([](std::vector<P> v, P &ans) {
        ans = v[0] + 2;
    })]
    = '2' >> A;


    A
    [([](std::vector<P> v, P &ans) {
        ans = v[0];
    })]
    = '+' >> S;


    A
    [([](std::vector<P> v, P &ans) {
        ans = 0;
    })]
    = eps;

    cout << GrammerForOutput<char>{gf.g, nonterminalNames, gf.calculateTerminalNames()} ;



}
















int main()
{

    testParseLL1Grammar();
    return 0;
}
