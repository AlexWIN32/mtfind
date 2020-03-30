/*******************************************************************************
    Author: Alexey Frolov (alexwin32@mail.ru)

    This software is distributed freely under the terms of the MIT License.
    See "LICENSE" or "http://copyfree.org/content/standard/licenses/mit/license.txt".
*******************************************************************************/
#include <Mtfind.h>
#include <Utils/FileGuard.h>
#include <Utils/ToString.h>
#include <ctime>
#include <iostream>
#include <vector>

int32_t GetRandVal(int32_t Min, int32_t Max)
{
    return Min + rand() % (Max - Min + 1);
}

char GetRandChar()
{
    return static_cast<char>(GetRandVal('a', 'z'));
}

int main(int argc, char ** argv)
{
    srand(static_cast<unsigned int>(time(nullptr)));

    size_t patternLength = GetRandVal(1, 20);

    std::string pattern(patternLength, '\0');

    for(char &c : pattern)
        c = GetRandChar();

    std::cout << "using pattern '" << pattern << "'" << std::endl;

    size_t appearancesCnt = GetRandVal(1, 20);

    std::cout << "possible appearances count: " << appearancesCnt << std::endl;

    std::wstring filePath = L"./Test.txt";

    size_t linesCnt = GetRandVal(1, 200);
    size_t cymbolsInLine = GetRandVal(1, 200);
    size_t appearanceLine = GetRandVal(1, linesCnt);
    size_t appearanceStartCymbol = GetRandVal(1, cymbolsInLine);

    try{
        Utils::FileGuard file(filePath, L"wb");

        for(size_t a = 0; a < appearancesCnt; a++){

            for(size_t l = 0; l < linesCnt; l++){
                for(size_t c = 0; c < cymbolsInLine; c++){

                    if(l == appearanceLine && c == appearanceStartCymbol)
                        file.WriteArray(&pattern[0], pattern.size());

                    file.Write(GetRandChar());
                }

                file.Write<char>('\n');
            }

            linesCnt = GetRandVal(1, 200);
            cymbolsInLine = GetRandVal(1, 200);
            appearanceLine = GetRandVal(1, linesCnt);
            appearanceStartCymbol = GetRandVal(1, cymbolsInLine);
        }
    }catch(const Exception &ex){
        if(!ex.What().empty())
            std::cerr << "error: " << ex.What() << std::endl;
        else
            std::wcerr << L"error: " << ex.WhatW() << std::endl;
        return 1;
    }

    std::vector<MatchData> results;

    auto func = [&results](const MatchData &Match)
    {
        results.push_back(Match);
    };

    const size_t CHUNK_LENGTH = 256;

    try{
        mtfind(filePath, pattern, CHUNK_LENGTH, func);
    }catch(const Exception &ex){
        if(!ex.What().empty())
            std::cerr << "error: " << ex.What() << std::endl;
        else
            std::wcerr << L"error: " << ex.WhatW() << std::endl;
        return 1;
    }

    for(const MatchData &m : results)
        std::cout << m.lineInd << std::endl;

    std::cout << "matches count: " << results.size() << std::endl;

    return 0;
}