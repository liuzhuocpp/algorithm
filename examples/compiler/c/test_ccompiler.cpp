#include <bits/stdc++.h>
#include <lz/debug.h>



#include <lz/compiler/c/grammar_analyze.h>

#include <direct.h>





using namespace lz;
using namespace std;





string getOutFileName(string number)
{
    return number + "\\out.txt";
}

string getErrorFileName(string number)
{
    return number + "\\error.txt";
//    return "error" + number + ".txt";
}




void solve(const string& fileName)
{

    regex fileNumberRegex(R"(([0-9]*)\\in\.txt)");





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


bool isFileExist(const string &fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}

int main()
{

    vector<string> inFiles = {
//            "in.txt",
            "0\\in.txt",
    };

    cout << isFileExist("0\\in.txt") << endl;
    cout << isFileExist("1\\in.txt") << endl;

//
//    for(string fileName: inFiles)
//    {
//        solve(fileName.c_str());
//    }






    return 0;
}
