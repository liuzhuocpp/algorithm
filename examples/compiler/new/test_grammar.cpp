
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
    cout << "isLL1 Grammar ? " << std::boolalpha << " " <<  isLL1Grammar(gf.g) << endl;;

    auto table = constructLL1Table(gf.g);
    auto ans = parseLL1Grammar<InputIterator, Grammar<int>>(first, last, gf.terminalMap, gf.g, table);

    if constexpr(!std::is_same<P, NoProperty>::value)
    {
        cout << ans << endl;
    }
}



void testParseLL1Grammar()
{
    OUT_FUNCTION_NAME

    using P = int;

    NonterminalProxy<char, P> S, A, B;

    GrammarFactory<char, P> gf(S);


    vector<string > nonterminalNames = {"S", "A", "B"};



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

    string text = "1+2+2+2+1";
    runParseLL1Grammar(text.begin(), text.end(), gf, nonterminalNames);


}





void testParseRegexGrammar()
{
    OUT_FUNCTION_NAME

    using P = int;
    NonterminalProxy<char, P> S, A, B;
    GrammarFactory<char, P> gf(S);


    vector<string > nonterminalNames = {"S", "A", "B"};

    S = '(' >> S >> ')' >> A;
    S = 'a' >> A;
    A = eps;
    A = S;
    A = '*' >> A;
    A = '|' >> S;

    string text = "(a)|aaaaaa(((a)aaaa(aaaa|aa)))****|((aaaa)((aaaa)aaa)(aaaaa)***a**a**)***aa(aaaa)";
    runParseLL1Grammar(text.begin(), text.end(), gf, nonterminalNames);

}












int main()
{

    testParseLL1Grammar();
    testParseRegexGrammar();
	return 0;
}
