/*
 * readFile.h
 *
 *  Created on: 2017年5月10日
 *      Author: LZ
 */

#ifndef EXAMPLES_COMPILER_C_READ_FILE_H_
#define EXAMPLES_COMPILER_C_READ_FILE_H_

#include <bits/stdc++.h>
#include <lz/debug.h>






namespace lz {


std::string getFilePreifx(int number)
{
    return "data\\" + std::to_string(number);
}
std::string getOutFileName(int number)
{
    return getFilePreifx(number) + "\\out.txt";
}

std::string getErrorFileName(int number)
{
    return getFilePreifx(number) + "\\z_error.txt"; // 为了使得这个文件在文件夹的最后
}

std::string getInputFileName(int fileNumber)
{
    return getFilePreifx(fileNumber) + "\\in.txt";
}

std::string readFile(const std::string& fileName)
{

    std::regex fileNumberRegex(R"((data\\[0-9]*)\\in\.txt)");

    std::smatch match;
    if(!std::regex_match(fileName.begin(), fileName.end(), match, fileNumberRegex))
    {
        std::cout << "input file name error: " << fileName << std::endl;
        return "";
    }

    std::string fileNumber = match[1];

//    freopen(fileName.c_str(), "r", stdin); //用这个不好使，不知道为啥, 可能是其他地方有异常
    std::ifstream fileIn(fileName);

    std::string text, tmpText;
    while(std::getline(fileIn, tmpText))
    {
        text += tmpText + "\n";
    }
    return text;
}

bool isFileExist(const std::string &fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}

std::vector<int> getAllInputFileDirectoryNumberList()
{
    std::vector<int> ans;
    for(int i = 0;
            ; ++ i)
    {
        std::string fileName = getInputFileName(i);
        if(!isFileExist(fileName)) break;

        ans.push_back(i);
    }
    return ans;

}



} // namespace lz





#endif /* EXAMPLES_COMPILER_C_READ_FILE_H_ */
