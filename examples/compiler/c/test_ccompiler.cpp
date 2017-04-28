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

    outStream.clear();
    errorOfstream.clear();

}


bool isFileExist(const string &fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}

int main()
{


    for(int i = 0; ; ++ i)
    {
        string fileName = to_string(i) + "\\in.txt";
        if(!isFileExist(fileName)) break;

        solve(fileName.c_str());
    }






    return 0;
}
