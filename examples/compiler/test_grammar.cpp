#include <bits/stdc++.h>
#include <lz/debug.h>

#include <lz/compiler/grammar.h>
#include <lz/compiler/ll1_grammar.h>

using namespace lz;
using namespace std;



void testNonterminals()
{
    OUT_FUNCTION_NAME
    Grammar<char> g;
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

    LL1ParsingTable<char> table = constructLL1ParsingTable(g);
    cout << LL1ParsingTableForOutput<char>{table, names} << endl;




    string text = "a+a*(a+a)";

    parseLL1Grammar(table, text.begin(),text.end(), makeNonterminal<char>(0), names);

}

void testIsLL1grammar()
{
    OUT_FUNCTION_NAME

    int n = 5;
    Grammar<char> g(n);

    auto E = g.getNonterminalProxy(0);
    std::vector<std::string> names{"E",};

    E = EmptyStringSymbol<char>;
    E = 'a' >> E;

//    names = {};
    cout << GrammerForOutput<char>{g, names} << endl;


    cout << "Is LL1 grammar? " << std::boolalpha << isLL1Grammar(g) << endl;


}


void testEliminateDirectLeftRecursion()
{
    OUT_FUNCTION_NAME

    int n = 1;
    Grammar<char> g(n);
    auto E = g.getNonterminalProxy(0);
    std::vector<std::string> names{"E", "E'"};

    E = 'k';
    E = 'e';
    E = 'j';
    E = E >> 'a';
    E = E >> 'b';
    E = E >> 'k';
    eliminateDirectLeftRecursion(g, E.nonterminal());


    cout << GrammerForOutput<char>{g, names} << endl;

    cout << "Is LL1 grammar? " << std::boolalpha << isLL1Grammar(g) << endl;

}

void testEliminateIndirectLeftRecursion()
{
    OUT_FUNCTION_NAME

    int n = 2;
    Grammar<char> g(n);
    auto A = g.getNonterminalProxy(0);
    auto B = g.getNonterminalProxy(1);
    std::vector<std::string> names{"A", "B", "B'",

    };
    A = B >> 'a';

    A = 'a';

    B = A >> 'b';
    B = 'b';






    eliminateLeftRecursion(g);


    cout << GrammerForOutput<char>{g, names} << endl;

    cout << "Is LL1 grammar? " << std::boolalpha << isLL1Grammar(g) << endl;

}



void testLeftFactor()
{
    OUT_FUNCTION_NAME

    int n = 3;
    Grammar<char> g(n);
    auto A = g.getNonterminalProxy(0);
    auto B = g.getNonterminalProxy(1);
    auto C = g.getNonterminalProxy(2);
    std::vector<std::string> names{
        "A",
        "B",
        "C",
        "A'",
        "A''",
    };


    A = 'a' >> B >> C;
    A = 'a';
    A = 'a' >> B;



    B = 'b';
    C = 'c';







    leftFactor(g, A.nonterminal());


    cout << GrammerForOutput<char>{g, names} << endl;
//
    cout << "Is LL1 grammar? " << std::boolalpha << isLL1Grammar(g) << endl;

}






int main()
{

#define OPEN_TEST true
#if OPEN_TEST == true
    testNonterminals();
    testIsLL1grammar();
    testEliminateDirectLeftRecursion();
    testEliminateIndirectLeftRecursion();
#else
#endif



    testLeftFactor();




	return 0;
}









