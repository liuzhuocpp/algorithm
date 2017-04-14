#include <bits/stdc++.h>
#include <lz/debug.h>


//#include <lz/compiler/nodeterministic_finite_automaton.h>
#include <lz/compiler/new/regex.h>

#include <lz/compiler/new/simulate_nfa.h>





using namespace lz;
using namespace std;




int main()
{
    using T = char;
    using NFA = AdjacencyList<DirectedGraphTag, NoProperty, Property<EdgeWeightTag, T> >;

    string s = "a*b|c|dj*kk";
    NFA nfa;

    auto [start, vertexToFunc] = parseMultiRegex(nfa, {
            {"a*b", [](string x) {

                cout << x << endl;

            }},
            {"c", [](string x) {
                cout << x << endl;
            }},
            {"dj*kk", [](string x) {
                cout << x << endl;
            }},

    });


//    for(auto i = 0; i < verticesNumber(nfa); ++ i)
//    {
//        cout << "vertex  " << i << ": " ;
//        for(auto e: outEdges(nfa, i))
//        {
//            cout << "(" << target(nfa, e) << " " << (int)edgePropertyMap(nfa, edgeWeightTag)[e] << "), ";
//        }
//        cout << endl;
//    }

    string text;
    text = "aaaaaaaabdjjjjjjjjjjjjjjjjkkcc";
    auto textBegin = text.begin();
    for(string::iterator cntEnd;textBegin != text.end(); textBegin = ++cntEnd)
    {
        cntEnd = simulateNFA(textBegin, text.end(), nfa, start, vertexToFunc);

//        cout << (cntEnd != text.end()) << endl;


    }
//
//    auto ans = simulateNFA(text.begin(), text.end(), nfa, start, vertexToFunc);
//
//    cout << (ans != text.end()) << endl;
//

//    cout << simulateNFA(text.begin(), text.end(), nfa, start, end) << endl;


//    text =("b");
//    cout << simulateNFA(text.begin(), text.end(), nfa, start, end) << endl;
//
//    text =("k");
//    cout << simulateNFA(text.begin(), text.end(), nfa, start, end) << endl;
//
//    text =("c");
//    cout << simulateNFA(text.begin(), text.end(), nfa, start, end) << endl;
//
//    text =("dkk");
//    cout << simulateNFA(text.begin(), text.end(), nfa, start, end) << endl;
//
//    text =("djjjjjjjjjkk");
//    cout << simulateNFA(text.begin(), text.end(), nfa, start, end) << endl;



	return 0;
}
