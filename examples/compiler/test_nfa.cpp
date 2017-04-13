#include <bits/stdc++.h>
#include <lz/debug.h>


#include <lz/compiler/nodeterministic_finite_automaton.h>
#include <lz/depth_first_search.h>
#include <lz/compiler/simulate_nfa.h>
#include <lz/compiler/regex.h>
using namespace lz;
using namespace std;


int main()
{
    string s = "a*b|c|dj*kk";

    NFA<char> nfa = analyzeRegex(s.begin(), s.end());

    string text;
    text =("aaaaaaaaaaaaaaaab");
    cout << simulateNFA(nfa, text.begin(), text.end()) << endl;

    text =("b");
    cout << simulateNFA(nfa, text.begin(), text.end()) << endl;

    text =("k");
    cout << simulateNFA(nfa, text.begin(), text.end()) << endl;

    text =("c");
    cout << simulateNFA(nfa, text.begin(), text.end()) << endl;

    text =("dkk");
    cout << simulateNFA(nfa, text.begin(), text.end()) << endl;

    text =("djjjjjjjjjkk");
    cout << simulateNFA(nfa, text.begin(), text.end()) << endl;



	return 0;
}
