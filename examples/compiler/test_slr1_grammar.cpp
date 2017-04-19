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

    auto [newG, actionTableOption, gotoFunction, markNonterminalsMap]
           = extendGrammarAndConstructActionGotoMark(g, indexToNonterminalMap, indexToTerminalMap, terminalToIndexMap);

    cout << string(2, '\n') << "Begin parsing..." << endl << endl;
    auto ans = parseLRGrammar(
            first,
            last,
            newG,
            actionTableOption.value(),
            gotoFunction,
            markNonterminalsMap,
            terminalToIndexMap,
            indexToTerminalMap,
            indexToNonterminalMap);

    if constexpr(!std::is_same<P, NoProperty>::value)
    {
        cout << "\n\nanswer is: " << ans << endl;
    }

    return ans;
}


struct Terminal
{
    string s;


    int getTypeId()
    {
        char ch = s[0];
        if(isdigit(ch))
        {
            return -1;
        }
        else return ch;
    }

    operator int()
    {
        if(isdigit(s[0]))
        {
            return stoi(s);
        }
        else
            return -1;
    }

    Terminal() = default;
    Terminal(char ch):s(1, ch)
    {
    }

    Terminal(const char*s):s(s)
    {
    }
    friend bool operator<(Terminal a, Terminal b)
    {
        return a.getTypeId() < b.getTypeId();
    }
    friend bool operator==(Terminal a, Terminal b)
    {
        return a.getTypeId() == b.getTypeId();
    }


    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
    operator<<(std::basic_ostream<Char, Traits>& os,
               const Terminal&  t)
    {
        if(isdigit(t.s[0]))
            return os << "NUMBER" ;
        else
            return os << t.s;
    }

};


void testParseSLR1AmbiguousGrammar()
{
    OUT_FUNCTION_NAME
    using P = int;




    NonterminalProxy<Terminal, P>
        LZ_NONTERMINAL_PROXY(S);

    GrammarFactory<Terminal, P> gf(S);




    S = '1' >> [](auto v, P& o) { o = v[1]; };

    S = S >> '+' >> S >> [](auto v, P& o) { o = v[1] + v[3]; } > '+' > '-' < '*' < '/';
    S = S >> '-' >> S >> [](auto v, P& o) { o = v[1] - v[3]; } > '+' > '-' < '*' < '/';
    S = S >> '*' >> S >> [](auto v, P& o) { o = v[1] * v[3]; } > '+' > '-' > '*' > '/';
    S = S >> '/' >> S >> [](auto v, P& o) { o = v[1] / v[3]; } > '+' > '-' > '*' > '/';
    S = eps >> '+' >> '+' >> S >> [](auto v, P& o) { o = 1+v[3]; } > '+' > '-' > '*' > '/';


    cout << "G: " << gf.g.actionsNumber() << endl;

    string text;
    vector<Terminal> terminalText;

    terminalText = {
        "111",
        "+",
        "222",
        "/",
        "2",
        "*",
        "7",
        "-",
        "88",
        "+",
        "+",
        "+",
        "87",

    };




    P ans = runParseSLR1Grammar(
        terminalText.begin(),
        terminalText.end(),
        gf.g,
        gf.getIndexToNonterminalMap(),
        gf.getIndexToTerminalMap(),
        gf.getTerminalToIndexMap());

    assert(ans == 888);
}









void testCalculateTypeAndWidth()
{
    OUT_FUNCTION_NAME

    using P = int;
    NonterminalProxy<char, P>
        LZ_NONTERMINAL_PROXY(T),
        LZ_NONTERMINAL_PROXY(B),
        LZ_NONTERMINAL_PROXY(C);

    GrammarFactory<char, P> gf(T);
//    vector<string > nonterminalNames = {
//            "T",
//            "B",
//            "C",
//            "M",
//            "T'",
//    };

    P w;

    T = B >> [&](auto vit, P& o) {  w = vit[1]; } >> C >>
        [&](auto vit, P& o) { o = vit[2]; };


    B = 'i' >> [](auto vit, P&o) { o = 4; };
    B = 'f' >> [](auto vit, P&o) { o = 8; };
    C = [&](auto vit, P &o) { o = w; };
    C = eps >> '[' >> '1' >> ']' >> C >> [](auto vit, P&o) { o = 1 * vit[4]; };
    C = eps >> '[' >> '2' >> ']' >> C >> [](auto vit, P&o) { o = 2 * vit[4]; };
    C = eps >> '[' >> '3' >> ']' >> C >> [](auto vit, P&o) { o = 3 * vit[4]; };

//    auto nonterminalNameMap = makeIteratorMap(nonterminalNames.begin());
//    auto indexToTerminalMap = gf.getIndexToTerminalMap();



    string text;
    text = "f[2][2][3]";
    P ans = runParseSLR1Grammar(
        text.begin(),
        text.end(),
        gf.g,
//        makeIteratorMap(nonterminalNames.begin()),
        gf.getIndexToNonterminalMap(),
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







