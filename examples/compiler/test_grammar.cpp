#include <bits/stdc++.h>
#include <lz/debug.h>

#include <lz/compiler/grammar.h>
#include <lz/compiler/ll1_grammar.h>

using namespace lz;
using namespace std;



void testNonterminals()
{
    OUT_FUNCTION_NAME
    Grammer<char> g;
    int n = 5;
    g.resize(n);

    auto
    E = g.getNonterminalProxy(0),
    _E = g.getNonterminalProxy(1),
    T = g.getNonterminalProxy(2),
    _T = g.getNonterminalProxy(3),
    F = g.getNonterminalProxy(4);

    E = T >> _E;

    _E = '+' >> T >> _E;
    _E = EmptyStringSymbol<char>;

    T = F >> _T;

    _T = '*' >> F >> _T ;
    _T = EmptyStringSymbol<char>;

    F = '(' >> E >> ')';
    F = 'a';

    std::vector<std::string> names{"E", "E'", "T", "T'", "F"};
    cout << GrammerForOutput<char>{g, names} << endl;


    cout << "table -----------------------------" << endl<< endl;
    cout << "Is LL1 grammar? " << std::boolalpha << isLL1Grammar(g) << endl;

    PredictiveParsingTable<char> table(g);
    cout << PredictiveParsingTableForOutput<char>{table, names} << endl;




    string text = "a+a*(a+a)";
    parseLL1Grammar(table, text.begin(),text.end(), Symbol<char>(SymbolType::Nonterminal, 0), names);

}

void testIsLL1grammar()
{
    OUT_FUNCTION_NAME
    int n = 5;
    Grammer<char> g(n);

    auto E = g.getNonterminalProxy(0);
    std::vector<std::string> names{"E",};

    E = EmptyStringSymbol<char>;
    E = 'a' >> E;

    cout << GrammerForOutput<char>{g, names} << endl;


    cout << "Is LL1 grammar? " << std::boolalpha << isLL1Grammar(g) << endl;


}

int main()
{

    testNonterminals();

    testIsLL1grammar();


	return 0;
}
