#include <bits/stdc++.h>
#include <lz/debug.h>

#include <lz/compiler/regex.h>
#include <lz/compiler/simulate_nfa.h>

#include <lz/compiler/noterminal_proxy.h>
#include <lz/compiler/grammar.h>
#include <lz/compiler/slr1_grammar.h>
#include <lz/compiler/lr_grammar.h>


#include <lz/compiler/c/lexical_analyze.h>


//#include <lz/compiler/grammar.h>
//#include <lz/compiler/noterminal_proxy.h>
////#include <lz/compiler/grammar_design.h>
//#include <lz/compiler/slr1_grammar.h>
//#include <lz/compiler/lr_grammar.h>




using namespace lz;
using namespace std;




struct Properties
{

};

template<typename InputIterator>
void grammarAnalyze(InputIterator first, InputIterator last)
{
    OUT_FUNCTION_NAME


    using P = Properties;
    using T = LexicalSymbol;


    NonterminalProxy<T, P> S;
    GrammarFactory<T, P> gf(S);

    vector<string > nonterminalNames = {
            "S",
            "M",
            "S'",
    };


    Grammar<P> g =  gf.g;

//    auto indexToNonterminalMap = makeIteratorMap(nonterminalNames.begin());
//    auto indexToTerminalMap =  gf.getIndexToTerminalMap();
//    auto terminalToIndexMap =  gf.getTerminalToIndexMap();
//
//
//    auto [newG, actionTableOption, gotoFunction, markNonterminalsMap]
//           = extendGrammarAndConstructActionGotoMark(g, indexToNonterminalMap, indexToTerminalMap, terminalToIndexMap);
//
//    cout << string(2, '\n') << "Begin parsing..." << endl << endl;
//    auto ans = parseLRGrammar(
//            first,
//            last,
//            newG,
//            actionTableOption.value(),
//            gotoFunction,
//            markNonterminalsMap,
//            terminalToIndexMap,
//            indexToTerminalMap,
//            indexToNonterminalMap);









}







int main()
{
    freopen("in.txt", "r", stdin);

    string text, tmpText;
    while(getline(cin, tmpText))
    {
        text += tmpText + "\n";
    }
    vector<LexicalSymbol> ans = lexicalAnalyze(text.begin(), text.end());
    for(auto x : ans)
    {
        cout << x << endl;
    }
//    cout << ans << endl;





    return 0;
}
