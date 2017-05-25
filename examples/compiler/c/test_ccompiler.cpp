#include <bits/stdc++.h>
#include <lz/debug.h>



#include <direct.h>
#include <lz/compiler/c/example/file_operation.h>
#include <lz/compiler/c/grammar_parser.h>





using namespace lz;
using namespace std;


std::ofstream outStream , errorOfstream;

GrammarParser<decltype(outStream), decltype(errorOfstream)>
    grammarParser(outStream, errorOfstream);


void solve(int fileNumber)
{
    std::string inputFileName = getInputFileName(fileNumber);
    std::string outFileName = getOutFileName(fileNumber);
    std::string errorFileName = getErrorFileName(fileNumber);

    auto text = readFile(inputFileName);

    vector<LexicalSymbol> ans = lexicalAnalyze(text.begin(), text.end());
    for(auto x : ans)
    {
        cout << x << endl;
    }

    outStream.open(outFileName, std::ofstream::out);
    errorOfstream.open(errorFileName, std::ofstream::out);


    grammarParser.parse(ans.begin(), ans.end());

    outStream.close();
    errorOfstream.close();

}








int main()
{
    grammarParser.construct();

//    solve(0);

    for(int fileNumber: getAllInputFileDirectoryNumberList())
    {
        solve(fileNumber);
    }








    return 0;
}
