/*******************************************************************************
    Author: Alexey Frolov (alexwin32@mail.ru)

    This software is distributed freely under the terms of the MIT License.
    See "LICENSE" or "http://copyfree.org/content/standard/licenses/mit/license.txt".
*******************************************************************************/

#include <Utils/FileGuard.h>
#include <Utils/ParallelDataStorage.h>
#include <Utils/ToString.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include "fileReader.h"
#include "patternSearchManager.h"
#include "patternMatchProcessor.h"

void OnMatch(const PatternMatchProcessor::MatchData &Match)
{
    std::wcout << Match.filePath << L":" << Match.lineInd << L" "<< Utils::ToWString(Match.line) << std::endl;
}

int main(char **argc, int argv)
{
    std::string pattern = "TT";
    std::wstring filePath = L"./fileReaderTest1.txt";
    size_t chunkLength = 5;

    try{
        FileDataChunksStorage matchProcessorChunks, searchManagerChunks;

        FileReader fileReader(filePath, pattern.size() - 1, chunkLength, &searchManagerChunks, &matchProcessorChunks);
        PatternMatchProcessor patternMatchProcessor(&matchProcessorChunks, filePath, OnMatch);
        PatternSearchManager patternSearchManager(pattern, &searchManagerChunks, &patternMatchProcessor);

        std::thread readThread(std::bind(&FileReader::Process, &fileReader));
        std::thread searchThread(std::bind(&PatternSearchManager::Process, &patternSearchManager));
        std::thread processThread(std::bind(&PatternMatchProcessor::Process, &patternMatchProcessor));

        readThread.join();
        searchThread.join();
        processThread.join();

    }catch(const Exception &ex){
        std::cerr << "error: " << ex.What() << std::endl;
        return 1;
    }

    return 0;
}