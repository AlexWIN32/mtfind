/*******************************************************************************
    Author: Alexey Frolov (alexwin32@mail.ru)

    This software is distributed freely under the terms of the MIT License.
    See "LICENSE" or "http://copyfree.org/content/standard/licenses/mit/license.txt".
*******************************************************************************/

#include <Mtfind.h>
#include <Utils/ParallelDataStorage.h>
#include <thread>
#include "fileReader.h"
#include "patternSearchManager.h"
#include "patternMatchProcessor.h"

void mtfind(const std::wstring &FilePath, const std::string &Pattern, size_t ChunkLength, OnMatchProc MatchProcedure)
{
    FileDataChunksStorage matchProcessorChunks, searchManagerChunks;

    bool hasWildcards = Pattern.find('?') != std::string::npos;

    FileReader fileReader(FilePath, Pattern.size() - 1, ChunkLength, &searchManagerChunks, &matchProcessorChunks);
    PatternMatchProcessor patternMatchProcessor(&matchProcessorChunks, MatchProcedure);
    PatternSearchManager patternSearchManager(Pattern, &searchManagerChunks, &patternMatchProcessor, hasWildcards);

    std::thread readThread(std::bind(&FileReader::Process, &fileReader));
    std::thread searchThread(std::bind(&PatternSearchManager::Process, &patternSearchManager));
    std::thread processThread(std::bind(&PatternMatchProcessor::Process, &patternMatchProcessor));

    readThread.join();
    searchThread.join();
    processThread.join();
}