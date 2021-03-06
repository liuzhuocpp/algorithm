#include <bits/stdc++.h>
#include <lz/debug.h>


//#include <lz/compiler/nodeterministic_finite_automaton.h>
#include <lz/compiler/regex.h>

#include <lz/compiler/simulate_nfa.h>





using namespace lz;
using namespace std;




int main()
{
    using T = char;
    using NFA = AdjacencyList<DirectedGraphTag, NoProperty, Property<EdgeWeightTag, T> >;

    string s = "a*b|c|dj*kk";
    NFA nfa;

    auto [start, vertexToFunc] = parseMultiRegex(nfa, {

            {"(_|[a-zA-Z])(_|[a-zA-Z0-9])*", [](auto first, auto last) {

                cout << "this is identifier:" << string(first, last) << endl;

            }},


            {"&", [](auto first, auto last) {

                cout << "this is &:" << string(first, last) << endl;
            }},

            {"&&", [](auto first, auto last) {

                cout << "this is &&:" << string(first, last) << endl;
            }},

            {"&&&&*", [](auto first, auto last) {

                cout << "this is more three &:" << string(first, last) << endl;
            }},



            {"([0-9][0-9]*)", [](auto first, auto last) {

                cout << "this is number:" << string(first, last) << endl;

            }},

            {"  *", [](auto first, auto last) {

                cout << "this is space:" << string(first, last) << endl;

            }},

            {"\n\n*", [](auto first, auto last) {

                cout << "this is newline:" << string(first, last) << endl;

            }},


            {"\\+", [](auto first, auto last) {

                cout << "this is +:" << string(first, last) << endl;
            }},

            {"-", [](auto first, auto last) {

                cout << "this is -:" << string(first, last) << endl;

            }},


            {"\\*", [](auto first, auto last) {

                cout << "this is *:" << string(first, last) << endl;
            }},

            {"/", [](auto first, auto last) {

                cout << "this is divide:" << string(first, last) << endl;

            }},

            {"\\|", [](auto first, auto last) {

                cout << "this is |:" << string(first, last) << endl;

            }},

            {"\\|\\|", [](auto first, auto last) {

                cout << "this is ||:" << string(first, last) << endl;
            }},

            {"\\|\\|\\|*", [](auto first, auto last) {

                cout << "this is more than three |:" << string(first, last) << endl;
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
    text = "aaaaaaaabdjjjjjjjjjjjjjjjjkkccxyzxyz";
    text = "xyzxyzxyz   xyz";
    text = "    _abc   a +   _   a0 aaa1     ";

    text = "|||| ||   || | ++ && && & &&&&&&&&  1231230981098  01111 1231212309800000  \n\n   + + - abcxyz int double fload   while if  else  long long  aaaaa   aaaa  a00000000 a000000 aa00000000";

    auto textBegin = text.begin();
    for(string::iterator cntEnd;textBegin != text.end(); textBegin = cntEnd)
    {
        cntEnd = simulateNFA(textBegin, text.end(), nfa, start, vertexToFunc);

        if(cntEnd == textBegin)
        {
            cout << "Unrecognized " << endl;
            cntEnd ++;
        }
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
