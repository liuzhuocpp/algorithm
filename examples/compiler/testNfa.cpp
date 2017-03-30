#include <bits/stdc++.h>
#include <lz/debug.h>


#include <lz/compiler/nodeterministic_finite_automaton.h>
#include <lz/depth_first_search.h>
#include <lz/compiler/simulate_nfa.h>
using namespace lz;
using namespace std;


int main()
{

//    std::optional<int> a;


    string s = "a*b|c|dj*kk";
//    s = "a";


    NFA<char> nfa = analyzeRegex(s.begin(), s.end());
//    cout << nfa << endl;


//    for(int i = 0; i < 10; ++ i)
//    {
//        cout << "I: " << i << ": ";
//        for(auto e: outEdges(nfa, i))
//        {
//            cout << "(" <<
//                    source(nfa, e) << ", " << target(nfa, e) <<  ")|";
//        }
//        cout << endl;
//
//    }




    string text;
    text =("aaaaaaaaaaaaaaaab");
    cout << simulateNFA(nfa, text.begin(), text.end()) << endl;

    text =("b");
    cout << simulateNFA(nfa, text.begin(), text.end()) << endl;

    text =("k");
    cout << simulateNFA(nfa, text.begin(), text.end()) << endl;

    text =("cc");
    cout << simulateNFA(nfa, text.begin(), text.end()) << endl;

    text =("dkk");
    cout << simulateNFA(nfa, text.begin(), text.end()) << endl;

    text =("djjjjjjjjjkk");
    cout << simulateNFA(nfa, text.begin(), text.end()) << endl;

//    auto nfaMap = edgePropertyMap(nfa, nfaEdgeTag);
//
//    depthFirstSearch(nfa, (
//        DepthFirstSearchKeywords::enterVertex = nfa.start,
//        DepthFirstSearchKeywords::treeEdge = [=](auto edge, auto u, auto to)
//        {
//            cout << u << " " << to << " ";
//            if(nfaMap[edge] != nullptr) cout << *nfaMap[edge];
//            cout << endl;
//        }));



//
//    cout << string(100, '-') << endl;


	return 0;
}
