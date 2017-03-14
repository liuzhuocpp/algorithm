
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
        for(Symbol s: vectorSet)
        {
            cout << SymbolForOutput<char>{s, nonterminalNames, terminalNames} << ' ';
        }
        cout << "|" <<endl;
    }

}

void testGrammar()
{
    OUT_FUNCTION_NAME

    GrammarFactory<char, int> gf;
    auto all = gf.makeNonternimals<3>();
    auto S = std::get<0>(all);
    auto A = std::get<1>(all);
    auto B = std::get<2>(all);
    vector<string > nonterminalNames = {"S", "A", "B"};

    auto x = [](auto v, auto& ans)
    {
        cout << "xxxxxxxxxxxxxx" << endl;
    };


    S[x] = A[x] >> 'a';
    A = 'b';
    A = eps;

    vector<Set > firstSets = calculateFirstSets(gf.g);

    for(Set firstSet: firstSets)
    {
        for(Symbol s: firstSet)
        {
            cout << SymbolForOutput<char>{s, nonterminalNames, gf.calculateTerminalNames()} << ' ';
        }
        cout << "|" <<endl;
    }

    cout << string(100, '-') <<endl;

    vector<Set> followSets = calculateFollowSets(gf.g, firstSets);

    for(Set followSet: followSets)
    {
        for(Symbol s: followSet)
        {
            cout << SymbolForOutput<char>{s, nonterminalNames, gf.calculateTerminalNames()} << ' ';
        }
        cout << "|" <<endl;
    }


    cout << GrammerForOutput<char>{gf.g, nonterminalNames, gf.calculateTerminalNames()} ;


    for(auto o: gf.actions)
    {
        int ans;
        o(std::vector<int>(), ans);
    }

}


void testRules()
{
    OUT_FUNCTION_NAME


    GrammarFactory<char, int> gf;
    auto all = gf.makeNonternimals<3>();
    auto S = std::get<0>(all);
    auto A = std::get<1>(all);
    auto B = std::get<2>(all);
    vector<string > nonterminalNames = {"S", "A", "B"};

    auto x = [](auto v, auto&ans)
    {
        cout << "xxxxxxxxxxxxxx" << endl;
    };


    S[x] = A[x] >> 'a';
    A = 'b';
    A = eps;
    B = 'c';
    B = A;
    for(auto rule: gf.g.rules())
    {
        auto head = gf.g.ruleHead(rule);
        auto body = gf.g.ruleBody(rule);
        cout << RuleForOutput<char>{head, body, nonterminalNames, gf.calculateTerminalNames()} << endl;
    }

}




void testLL1Table()
{
    OUT_FUNCTION_NAME

    GrammarFactory<char, int> gf;
    auto all = gf.makeNonternimals<3>();
    auto S = std::get<0>(all);
    auto A = std::get<1>(all);
    auto B = std::get<2>(all);
    vector<string > nonterminalNames = {"S", "A", "B"};

    S = eps;

    S = 's' >> S;
//    S = eps;
    A = 'a';
    B = 'b';


    auto firstSets = calculateFirstSets(gf.g);
    auto followSets = calculateFollowSets(gf.g, firstSets);
    outVectorSet(firstSets, nonterminalNames, gf.calculateTerminalNames());
    cout << string(100, '-') << endl;
    outVectorSet(followSets, nonterminalNames, gf.calculateTerminalNames());

    auto table = constructLL1Table(gf.g);


    for(auto x: table)
    {
        cout << SymbolForOutput<char>{x.first.first, nonterminalNames, gf.calculateTerminalNames()}
             << " "
             << SymbolForOutput<char>{x.first.second, nonterminalNames, gf.calculateTerminalNames()}
             << ": " << " " ;
//        cout << x.first.first << " " << x.first.second << ":" << endl;
        auto rule = x.second;
        auto head = gf.g.ruleHead(rule);
        auto body = gf.g.ruleBody(rule);
        cout << RuleForOutput<char>{head, body, nonterminalNames, gf.calculateTerminalNames()} << endl;

    }



//    cout

}






void testParseLL1Grammar()
{
    OUT_FUNCTION_NAME

    GrammarFactory<char, int> gf;
    auto all = gf.makeNonternimals<2>();
    auto S = std::get<0>(all);
    auto A = std::get<1>(all);
//    auto B = std::get<2>(all);
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


    cout << "GF action size" << gf.actions.size() << endl;

    cout << GrammerForOutput<char>{gf.g, nonterminalNames, gf.calculateTerminalNames()} ;


    auto firstSets = calculateFirstSets(gf.g);
    auto followSets = calculateFollowSets(gf.g, firstSets);
    outVectorSet(firstSets, nonterminalNames, gf.calculateTerminalNames());
    cout << string(100, '-') << endl;
    outVectorSet(followSets, nonterminalNames, gf.calculateTerminalNames());

    cout << "isLL1 Grammar ? " << std::boolalpha << " " <<  isLL1Grammar(gf.g) << endl;;
//    return ;

    auto table = constructLL1Table(gf.g);

//
//    return ;

    string text = "1+1+1+1+2";
    parseLL1Grammar<string::iterator, int>(text.begin(), text.end(), gf.terminalMap, gf.g, gf.actions, table);


}
















int main()
{
//    testGrammar();
//    testRules();
//    testLL1Table();
    testParseLL1Grammar();
	return 0;
}
