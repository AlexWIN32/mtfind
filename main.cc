/*******************************************************************************
    Author: Alexey Frolov (alexwin32@mail.ru)

    This software is distributed freely under the terms of the MIT License.
    See "LICENSE" or "http://copyfree.org/content/standard/licenses/mit/license.txt".
*******************************************************************************/

#include <Utils/ToString.h>
#include <Exception.h>
#include <Mtfind.h>
#include <iostream>
#include <vector>

void OnMatch(const MatchData &Match)
{
    std::wcout << Match.filePath << L":" << Match.lineInd << L" "<< Utils::ToWString(Match.line) << std::endl;
}

int main(char **argc, int argv)
{
    std::string pattern = "TT";
    std::wstring filePath = L"./fileReaderTest1.txt";
    size_t chunkLength = 5;

    try{
        mtfind(filePath, pattern, chunkLength, OnMatch);
    }catch(const Exception &ex){
        std::cerr << "error: " << ex.What() << std::endl;
        return 1;
    }

    return 0;
}