
#include <bits/stdc++.h>
#include <lz/debug.h>
#include <lz/std_utility.h>
#include <lz/compiler/grammar.h>
#include <lz/compiler/noterminal_proxy.h>
#include <lz/compiler/grammar_design.h>
#include <lz/compiler/ll1_grammar.h>
#include <lz/map.h>
using namespace lz;
using namespace std;

template<typename T1, typename T2>
void outVectorSet(vector<Set > vectorSets, T1 nonterminalNames, T2 terminalNames)
{
    cout << string(100, '-') << endl;
    for(Set vectorSet: vectorSets)
    {
        for(SymbolDescriptor s: vectorSet)
        {
            cout << SymbolForOutput<SymbolDescriptor, T1, T2>{s, nonterminalNames, terminalNames} << ' ';
        }
        cout << "|" <<endl;
    }

}



// run parsing process to see the detail
template< typename InputIterator, typename GrammarFactory, typename NonterminalMap>
auto runParseLL1Grammar(InputIterator first, InputIterator last, GrammarFactory &gf, NonterminalMap nonterminalMap)
{
    using P = typename decltype(gf.g)::NodeProperties;
    cout << "action size: " << gf.g.actionsNumber() << endl;

    cout << GrammerForOutput<decltype(gf.g), NonterminalMap, decltype(gf.getIndexToTerminalMap())>
        {gf.g, nonterminalMap, gf.getIndexToTerminalMap()} ;

    auto firstSets = calculateFirstSets(gf.g);
    auto followSets = calculateFollowSets(gf.g, firstSets);

    cout << "First sets:" << endl;
    outVectorSet(firstSets, nonterminalMap, gf.getIndexToTerminalMap());

    cout << "Follow sets:" << endl;
    outVectorSet(followSets, nonterminalMap, gf.getIndexToTerminalMap());
    cout << string(100, '-') << endl;
    cout << "isLL1 Grammar ? " << std::boolalpha << " " <<  isLL1Grammar(gf.g) << endl;;

    auto table = constructLL1Table(gf.g);


    using TerminalType = typename std::iterator_traits<InputIterator>::value_type;

    auto terminalToIndexMap = gf.getTerminalToIndexMap();

    using TerminalIterator = TerminalIndexIterator<InputIterator, decltype(terminalToIndexMap) >;



    TerminalIterator begin(first, terminalToIndexMap);
    TerminalIterator end(last, terminalToIndexMap);

//
//    cout << "JJJ" << terminalToIndexMap['1'] << endl;
//    cout << "JJJ" << terminalToIndexMap['2'] << endl;
//    cout << "JJJ" << terminalToIndexMap['+'] << endl;

//    cout << "HHHH" << endl;
//    gf.getIndexToTerminalMap();

//    cout << "HHHH" << endl;
    auto ans = parseLL1Grammar<TerminalIterator, Grammar<P>, decltype(table) >
        (begin, end, gf.g, table, 0, gf.getIndexToTerminalMap() );

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

    using P = NoProperty;
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
    runParseLL1Grammar(text.begin(), text.end(), gf, makeIteratorMap(nonterminalNames.begin())   );

}












int main()
{
    testParseRegexGrammar();
    testParseLL1Grammar();

	return 0;
}
