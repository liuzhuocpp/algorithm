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
    return number + "\\z_error.txt"; // 为了使得这个文件在文件夹的最后
}




std::ofstream outStream , errorOfstream;

GrammarParser<decltype(outStream), decltype(errorOfstream)>
    grammarParser{outStream, errorOfstream};


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

//    freopen(fileName.c_str(), "r", stdin); //用这个不好使，不知道为啥, 可能是其他地方有异常
    std::ifstream fileIn(fileName);

    string text, tmpText;
    while(getline(fileIn, tmpText))
    {
        text += tmpText + "\n";
    }
    vector<LexicalSymbol> ans = lexicalAnalyze(text.begin(), text.end());
    for(auto x : ans)
    {
        cout << x << endl;
    }

    outStream.open(getOutFileName(fileNumber), std::ofstream::out);
    errorOfstream .open(getErrorFileName(fileNumber), std::ofstream::out);


    grammarParser.parse(ans.begin(), ans.end());

    outStream.close();
    errorOfstream.close();

}






bool isFileExist(const string &fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}

int main()
{
    grammarParser.construct();
    vector<int> test;

    for(int i = 0;
            ; ++ i)
    {
        string fileName = to_string(i) + "\\in.txt";
        if(!isFileExist(fileName)) break;

        solve(fileName.c_str());
    }
//

//    solve("0\\in.txt");
//    solve("1\\in.txt");





    return 0;
}
