#include <bits/stdc++.h>
#include <lz/debug.h>
#include <lz/std_utility.h>
#include <lz/compiler/grammar.h>

#include <lz/compiler/noterminal_proxy.h>

#include <lz/compiler/grammar_design.h>
#include <lz/compiler/slr1_grammar.h>
#include <lz/compiler/lr_grammar.h>





using namespace lz;
using namespace std;

template<typename InputIterator, typename P, typename IndexToNonterminalMap,
    typename IndexToTerminalMap, typename TerminalToIndexMap >
auto runParseSLR1Grammar(
        InputIterator first,
        InputIterator last,
        const Grammar<P>& g,
        IndexToNonterminalMap indexToNonterminalMap,
        IndexToTerminalMap indexToTerminalMap,
        TerminalToIndexMap terminalToIndexMap,
        bool outputNonkernelItem = true)
{
    auto newGrammarAndData = extendGrammarAndConstructActionGotoMark(g, indexToNonterminalMap, indexToTerminalMap, terminalToIndexMap);
    auto newG = std::get<0>(newGrammarAndData);
    auto actionTableOption = std::get<1>(newGrammarAndData);
    auto gotoFunction = std::get<2>(newGrammarAndData);
    auto markNonterminalsMap = std::get<3>(newGrammarAndData);
    using TerminalIterator = TerminalIndexIterator<InputIterator, decltype(terminalToIndexMap) >;

    TerminalIterator begin(first, terminalToIndexMap);
    TerminalIterator end(last, terminalToIndexMap);
    cout << string(2, '\n') << "Begin parsing..." << endl << endl;
    auto ans = parseLRGrammar(
            begin,
            end,
            newG,
            actionTableOption.value(),
            gotoFunction,
            markNonterminalsMap,
            indexToTerminalMap,
            indexToNonterminalMap);

    if constexpr(!std::is_same<P, NoProperty>::value)
    {
        cout << "\n\nanswer is: " << ans << endl;
    }

    return ans;
}




void testParseSLR1AmbiguousGrammar()
{
    OUT_FUNCTION_NAME
    using P = int;

    NonterminalProxy<char, P> S;

    GrammarFactory<char, P> gf(S);
    vector<string > nonterminalNames = {
            "S",
            "M",
            "S'",
    };

    S = '1' >> [](auto v, P& o) { o = 1; };
    S = '2' >> [](auto v, P& o) { o = 2; };

    S = S >> '+' >> S >> [](auto v, P& o) { o = v[1] + v[2]; } > '+' > '-' < '*' < '/';
    S = S >> '-' >> S >> [](auto v, P& o) { o = v[1] - v[2]; } > '+' > '-' < '*' < '/';
    S = S >> '*' >> S >> [](auto v, P& o) { o = v[1] * v[2]; } > '+' > '-' > '*' > '/';
    S = S >> '/' >> S >> [](auto v, P& o) { o = v[1] / v[2]; } > '+' > '-' > '*' > '/';
    S = eps >> '+' >> '+' >> S >> [](auto v, P& o) { o = 1+v[1]; } > '+' > '-' > '*' > '/';


    cout << "G: " << gf.g.actionsNumber() << endl;

    string text;
    text = "1+2/2+1+2*2-1";
    text = "++2+++2";

    P ans = runParseSLR1Grammar(
        text.begin(),
        text.end(),
        gf.g,
        makeIteratorMap(nonterminalNames.begin()),
        gf.getIndexToTerminalMap(),
        gf.getTerminalToIndexMap());

    assert(ans == 6);
}









void testCalculateTypeAndWidth()
{
    OUT_FUNCTION_NAME

    using P = int;
    NonterminalProxy<char, P> T, B, C;

    GrammarFactory<char, P> gf(T);
    vector<string > nonterminalNames = {
            "T",
            "B",
            "C",
            "M",
            "T'",
    };

    P w;

    T = B >> [&](auto vit, P& o) {  w = vit[1]; } >> C >>
        [&](auto vit, P& o) { o = vit[2]; };


    B = 'i' >> [](auto vit, P&o) { o = 4; };
    B = 'f' >> [](auto vit, P&o) { o = 8; };
    C = [&](auto vit, P &o) { o = w; };
    C = eps >> '[' >> '1' >> ']' >> C >> [](auto vit, P&o) { o = 1 * vit[1]; };
    C = eps >> '[' >> '2' >> ']' >> C >> [](auto vit, P&o) { o = 2 * vit[1]; };
    C = eps >> '[' >> '3' >> ']' >> C >> [](auto vit, P&o) { o = 3 * vit[1]; };

    auto nonterminalNameMap = makeIteratorMap(nonterminalNames.begin());
    auto indexToTerminalMap = gf.getIndexToTerminalMap();



    string text;
    text = "f[2][2][3]";
    P ans = runParseSLR1Grammar(
        text.begin(),
        text.end(),
        gf.g,
        makeIteratorMap(nonterminalNames.begin()),
        gf.getIndexToTerminalMap(),
        gf.getTerminalToIndexMap());

    assert(ans == 96);
}



int main()
{
    testParseSLR1AmbiguousGrammar();
    testCalculateTypeAndWidth();
    return 0;
}







