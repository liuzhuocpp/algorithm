#include <bits/stdc++.h>
#include <lz/compiler/c/example/file_operation.h>
#include <lz/debug.h>




#include <lz/compiler/c/lexical_analyze.h>





using namespace lz;
using namespace std;


void solve(const string & text)
{

    auto ans = lexicalAnalyze(text.begin(), text.end());
    for(auto x: ans)
    {
        cout << x << endl;
    }

}



int main()
{


    solve(readFile(getInputFileName(0)));
//    for(auto x: getAllInputFileDirectoryNumberList())
//    {
//        string text = readFile(getInputFileName(x));
//        solve(text);
//    }





    return 0;
}
