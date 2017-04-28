#include <bits/stdc++.h>
#include <lz/debug.h>



#include <lz/compiler/c/grammar_analyze.h>

#include <direct.h>





using namespace lz;
using namespace std;





string getOutFileName(string number)
{
    return "out" + number + ".txt";
}

string getErrorFileName(string number)
{
    return "error" + number + ".txt";
}




void solve(const string fileName)
{

    regex fileNumberRegex(R"(in([0-9]*)\.txt)");





    std::smatch match;
    if(!regex_match(fileName.begin(), fileName.end(), match, fileNumberRegex))
    {
        cout << "input file name error: " << fileName << endl;
        return ;
    }

    string fileNumber = match[1];

    freopen(fileName.c_str(), "r", stdin);


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


    std::ofstream outStream (getOutFileName(fileNumber), std::ofstream::out);
    std::ofstream errorOfstream (getErrorFileName(fileNumber), std::ofstream::out);

    grammarAnalyze(ans.begin(), ans.end(), outStream, errorOfstream);

}



int main()
{

    vector<string> inFiles = {
            "in.txt",
    };



    for(string fileName: inFiles)
    {
        solve(fileName.c_str());
    }






    return 0;
}
