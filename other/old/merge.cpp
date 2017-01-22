/*
 * merge.cpp
 *
 *  Created on: 2015Äê12ÔÂ25ÈÕ
 *      Author: LZ
 *
 * merge multi included files
 */




#include <bits/stdc++.h>
using namespace std;

string includedDirPath = "D:\\code\\github\\algorithm\\";

string fileName= "D:\\code\\github\\algorithm\\examples\\strongly_connected_components\\cf402_E.cpp";
string ansFileName = "ans.txt";



string getFileContent(string fileName)
{
	string ans = "";
	std::fstream fs (fileName, std::fstream::in);
	while(fs.peek() != -1)
	{
		ans.push_back(fs.get());
	}
	fs.close();
	return ans;
}

set<string> fileNameSet;

string dfs(string fileName)
{
	string content = getFileContent(fileName);
	string ans;
	const string Include = "#include";
	for(int i = 0; i < content.size();)
	{
		if(content.substr(i, Include.size()) == Include)
		{
			int j, k;
			bool ok;

			j = i - 1;
			ok = 1;
			for(;j >= 0; -- j)
			{
				if(content[j] == ' ') continue;
				if(content[j] == '/' && (j - 1 >= 0 && (content[j - 1] == '/')) )
				{
					ok = 0;
				}
				break;
			}
			if(!ok)
			{
				ans.push_back(content[i ++ ]);
				continue;
			}



			j = i + Include.size(), k;
			ok = 1;
			for(;; ++ j)
			{
				if(content[j] == '\"')
				{
					break;
				}
				if(content[j] == '<')
				{
					ok = 0;
					break;
				}
			}
			if(!ok)
			{
				ans.push_back(content[i ++ ]);

				continue;
			}

			k = j + 1;
			while(content[k] != '\"') ++ k;

			string includedFileName = content.substr(j + 1, k - j - 1);

			if(fileNameSet.count(includedFileName))
			{

			}
			else
			{
				fileNameSet.insert(includedFileName);
				includedFileName = includedDirPath + includedFileName;

				ans += dfs(includedFileName);
			}


			i = k + 1;
		}
		else
		{
			ans.push_back(content[i]);
			++ i;
		}

	}
	return ans;


}

int main()
{

	auto ans = dfs(fileName);

	cout << ans;

	cout << string(100, '-') << "\n";
	cout << (int)ans.back() << endl;
	cout << ans.back() << endl;
	std::fstream fs (ansFileName, std::fstream::out | std::fstream::in);
	fs << ans;

	fs.close();


//	std::fstream fs (fileName, std::fstream::in | std::fstream::out);


//	while(fs.peek() != -1)
//	{
//		cout <<
//				(char)
//				fs.get();
//	}
//	fs.close();



	return 0;
}














































