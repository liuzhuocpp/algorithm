#include <bits/stdc++.h>
#include <lz/debug.h>

#include <lz/compiler/grammar.h>

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


void test()
{
    Grammer<char> g;
    auto n = 5;
    g.resize(n); // 3个非终结符号
    using S = Symbol<char>;
    auto V = SymbolType::Variable;
    auto L = SymbolType::LiteralUnit;

    g[0].push_back({S(V, 2), S(V, 1),});

    g[1].push_back({S(L, '+'), S(V, 2),S(V, 1)});
    g[1].push_back({S(SymbolType::EmptyString),});


    g[2].push_back({S(V, 4), S(V, 3)});

    g[3].push_back({S(L, '*'), S(V, 4), S(V, 3)});
    g[3].push_back({S(SymbolType::EmptyString),});

    g[4].push_back({S(L, '('), S(V, 0), S(L, ')')});
    g[4].push_back({S(L, 'a'),});




    cout << g << endl;

    auto firstSet = calculateFirstSet(g);
    cout << firstSet << endl;

    auto followSet = calculateFollowSet(g, firstSet);

    cout << followSet << endl;



    cout << "table -----------------------------" << endl<< endl;
    auto table = constructPredictiveParsingTable(g, firstSet, followSet);

    for(int i = 0; i < n; ++ i)
    {
        for(auto pi: table[i])
        {
            cout << "(" << i <<  "," << pi.first << ") :";
            output(g, i, pi.second);
        }
    }
//    cout << table << endl;



}
void testNonterminals()
{
    Grammer<char> g;
    NonterminalSymbol<char>::reset();

    NonterminalSymbol<char> S("S"), S1("S1"), S2("S2");
    S = 'a' >> S1 >> S2;
    S1 = EmptyStringSymbol<char>;
    S1 = S;

    S2 = '*' >> S >> S2;
    S2 = '|' >> S >> S2;
    S2 = EmptyStringSymbol<char>;


    g = makeGrammer(std::vector<NonterminalSymbol<char>>{S, S1, S2});

    cout << g << endl;

//    cout << "我" << endl;





}

int main()
{

//    test();
    testNonterminals();


	return 0;
}
