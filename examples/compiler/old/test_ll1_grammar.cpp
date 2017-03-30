#include <bits/stdc++.h>
#include <lz/debug.h>

#include <lz/compiler/grammar.h>
#include <lz/compiler/ll1_grammar.h>

using namespace lz;
using namespace std;



void testAnalazyRegexGrammar()
{
    OUT_FUNCTION_NAME

    int n = 4;
    Grammar<char> g(n);

    auto S = g.getNonterminalProxy(0);
    auto E = g.getNonterminalProxy(1);
    auto A = g.getNonterminalProxy(2);
    auto B = g.getNonterminalProxy(3);
    std::vector<std::string> names{"S", "E", "A", "B", "A'", "B'", "E'", "S'", "S''"};
    A = 'a';
    A = 'a' >> A;
    B = '*';
    B = '*' >> B;

    E = A;
    E = A >> B;
    E = A >> B >> E;

//    S = EmptyStringSymbol<char>;
    S = E >> '|' >> S;
    S = E;


    leftFactor(g, A.nonterminal());
    leftFactor(g, B.nonterminal());
    leftFactor(g, E.nonterminal());
    leftFactor(g, S.nonterminal());

//    eliminateLeftRecursion(g);

    cout << GrammerForOutput<char>{g, names} << endl;


    cout << "Is LL1 grammar? " << std::boolalpha << isLL1Grammar(g) << endl;




    LL1ParsingTable<char> table = constructLL1ParsingTable(g);
    cout << LL1ParsingTableForOutput<char>{table, names} << endl;
    string text = "a|aa";
    parseLL1Grammar(table, text.begin(),text.end(), makeNonterminal<char>(0), names);




}



int main()
{
    testAnalazyRegexGrammar();








    return 0;
}









