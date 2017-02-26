#include <bits/stdc++.h>
#include <lz/debug.h>

#include <lz/compiler/grammar.h>
#include <lz/compiler/ll1_grammar.h>

using namespace lz;
using namespace std;

template<typename T>
void output(const Grammer<T> &g, int i, int j)
{
    cout << i << "-> " << g[i][j] << endl;
}

//void test()
//{
//    Grammer<char> g;
//    auto n = 3;
//    g.resize(n); // 3个非终结符号
//    using S = Symbol<char>;
//    auto V = SymbolType::Variable;
//    auto L = SymbolType::LiteralUnit;
//    g[0].push_back({S(L, 'a'), S(V, 1), S(V, 2),});
//
//    g[1].push_back({S(V, 0),});
//    g[1].push_back({S(SymbolType::EmptyString),});
//
//
//    g[2].push_back({S(L, '*'), S(V, 0), S(V, 2)});
//    g[2].push_back({S(L, '|'), S(V, 0), S(V, 2)});
//    g[2].push_back({S(SymbolType::EmptyString),});
//
//
//    cout << g << endl;
//
//    auto firstSet = calculateFirstSet(g);
//    cout << firstSet << endl;
//
//    auto followSet = calculateFollowSet(g, firstSet);
//
//    cout << followSet << endl;
//
//
//
//    cout << "table -----------------------------" << endl<< endl;
//    auto table = constructPredictiveParsingTable(g, firstSet, followSet);
//
//    for(int i = 0; i < n; ++ i)
//    {
//        for(auto pi: table[i])
//        {
//            cout << "(" << i <<  "," << pi.first << ") :";
//            output(g, i, pi.second);
//        }
//    }
////    cout << table << endl;
//
//
//
//}


//void test()
//{
//    Grammer<char> g;
//    auto n = 5;
//    g.resize(n); // 3个非终结符号
//    using S = Symbol<char>;
//    auto V = SymbolType::Nonterminal;
//    auto L = SymbolType::Terminal;
//
//    g[0].push_back({S(V, 2), S(V, 1),});
//
//    g[1].push_back({S(L, '+'), S(V, 2),S(V, 1)});
//    g[1].push_back({S(SymbolType::EmptyString),});
//
//
//    g[2].push_back({S(V, 4), S(V, 3)});
//
//    g[3].push_back({S(L, '*'), S(V, 4), S(V, 3)});
//    g[3].push_back({S(SymbolType::EmptyString),});
//
//    g[4].push_back({S(L, '('), S(V, 0), S(L, ')')});
//    g[4].push_back({S(L, 'a'),});
//
//
//
//
//    cout << g << endl;
//
//    auto firstSet = calculateFirstSet(g);
//    cout << firstSet << endl;
//
//    auto followSet = calculateFollowSet(g, firstSet);
//
//    cout << followSet << endl;
//
//
//
//    cout << "table -----------------------------" << endl<< endl;
//    auto table = constructPredictiveParsingTable(g, firstSet, followSet);
//
//    for(int i = 0; i < n; ++ i)
//    {
//        for(auto pi: table[i])
//        {
//            cout << "(" << i <<  "," << pi.first << ") :";
//            output(g, i, pi.second);
//        }
//    }
////    cout << table << endl;
//
//
//
//}
void testNonterminals()
{
    Grammer<char> g;
    int n = 5;
    g.resize(n);

    auto E = g.getNonterminalProxy(0);
    auto _E = g.getNonterminalProxy(1);
    auto T = g.getNonterminalProxy(2);
    auto _T = g.getNonterminalProxy(3);
    auto F = g.getNonterminalProxy(4);

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

    PredictiveParsingTable<char> table(g);
    cout << PredictiveParsingTableForOutput<char>{table, names} << endl;

    string text = "a+a*(a+a)";
    predictivePasringLL1(table, text.begin(),text.end(), Symbol<char>(SymbolType::Nonterminal, 0), names);








}

int main()
{

//    test();
    testNonterminals();


	return 0;
}
