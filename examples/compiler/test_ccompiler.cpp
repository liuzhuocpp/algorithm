#include <bits/stdc++.h>
#include <lz/debug.h>



#include <lz/compiler/c/grammar_analyze.h>


//#include <lz/compiler/grammar.h>
//#include <lz/compiler/noterminal_proxy.h>
////#include <lz/compiler/grammar_design.h>
//#include <lz/compiler/slr1_grammar.h>
//#include <lz/compiler/lr_grammar.h>




using namespace lz;
using namespace std;











int main()
{
    freopen("in.txt", "r", stdin);

    string text, tmpText;
    while(getline(cin, tmpText))
    {
        text += tmpText + "\n";
    }
    vector<LexicalSymbol> ans = lexicalAnalyze(text.begin(), text.end());
    for(auto x : ans)
    {
        cout << x << endl;
    }

    grammarAnalyze(ans.begin(), ans.end());
//    cout << ans << endl;





    return 0;
}
