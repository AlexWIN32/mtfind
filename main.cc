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
#include <cstdint>
#include <cstddef>

void PrintUsage(char *ProgramName)
{
    std::cout << "Usage: " << ProgramName << " file ?pattern" << std::endl
              << "Performs multithreaded pattern search using Pipeline pattern" << std::endl
              << std::endl 
              << "  file        path to file in which you want to perform search" << std::endl
              << "  pattern     sequence of symbols you want to find" << std::endl;
}

void OnMatch(const MatchData &Match)
{
    std::wcout << Match.filePath << L":" << Match.lineInd << L" "<< Utils::ToWString(Match.line) << std::endl;
}

int main(int argc, char ** argv)
{
    if(argc != 3){
        PrintUsage(argv[0]);
        return 0;
    }

    std::wstring filePath = Utils::ToWString(argv[1]);
    std::string pattern = argv[2];

    if(pattern.empty() || pattern[0] != '?'){
        std::cerr << "error: pattern must start with '?' symbol";
        return 1;
    }

    pattern = pattern.substr(1);

    const size_t CHUNK_LENGTH = 256;

    try{
        mtfind(filePath, pattern, CHUNK_LENGTH, OnMatch);
    }catch(const Exception &ex){
        if(!ex.What().empty())
            std::cerr << "error: " << ex.What() << std::endl;
        else
            std::wcerr << L"error: " << ex.WhatW() << std::endl;
        return 1;
    }

    return 0;
}