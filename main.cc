/*******************************************************************************
    Author: Alexey Frolov (alexwin32@mail.ru)

    This software is distributed freely under the terms of the MIT License.
    See "LICENSE" or "http://copyfree.org/content/standard/licenses/mit/license.txt".
*******************************************************************************/

#include <Utils/ToString.h>
#include <Utils/FileGuard.h>
#include <Exception.h>
#include <Mtfind.h>
#include <iostream>
#include <vector>
#include <cstdint>
#include <cstddef>

void PrintUsage(char *ProgramName)
{
    std::cout << "Usage: " << ProgramName << " file pattern" << std::endl
              << "Performs multithreaded pattern search using Pipeline pattern" << std::endl
              << std::endl 
              << "  file        path to file in which you want to perform search" << std::endl
              << "  pattern     sequence of symbols you want to find. '?' symbol means any single char" << std::endl;
}

void OutputMatches(const std::vector<MatchData> &Matches, const std::wstring &FilePath)
{
    std::cout << Matches.size() << std::endl;

    Utils::FileGuard file(FilePath, L"rb");

    for(const MatchData &m : Matches){
        std::string line(m.matchParams.count, '\0');

        file.SetPos(m.matchParams.start);
        file.ReadArray(&line[0], line.size());

        int lineBasedInd = (m.matchParams.start - m.lineParams.start) + 1;

        std::cout << m.lineInd << " " << lineBasedInd << " " << line << std::endl;
    }
}

int main(int argc, char ** argv)
{
    if(argc != 3){
        PrintUsage(argv[0]);
        return 0;
    }

    std::wstring filePath = Utils::ToWString(argv[1]);
    std::string pattern = argv[2];

    const size_t CHUNK_LENGTH = 1024;

    std::vector<MatchData> matches;
    auto matchFunc = [&matches](const MatchData &Match)
    {
        matches.push_back(Match);
    };

    try{
        mtfind(filePath, pattern, CHUNK_LENGTH, matchFunc);
    }catch(const Exception &ex){
        if(!ex.What().empty())
            std::cerr << "error: " << ex.What() << std::endl;
        else
            std::wcerr << L"error: " << ex.WhatW() << std::endl;
        return 1;
    }

    OutputMatches(matches, filePath);

    return 0;
}
