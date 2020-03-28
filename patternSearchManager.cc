/*******************************************************************************
    Author: Alexey Frolov (alexwin32@mail.ru)

    This software is distributed freely under the terms of the MIT License.
    See "LICENSE" or "http://copyfree.org/content/standard/licenses/mit/license.txt".
*******************************************************************************/

#pragma once
#include "patternSearchManager.h"
#include "fileDataChunk.h"
#include "patternMatchProcessor.h"
#include <functional>
#include <iostream>

PatternSearchManager::PatternSearchManager(const std::string &Pattern,
                                           FileDataChunksStorage *FileDataChunksStorage,
                                           PatternMatchProcessor *MatchProcessor)
    : pattern(Pattern), chunksStorage(FileDataChunksStorage), matchProcessor(MatchProcessor)
{    
}

void PatternSearchManager::Process()
{
    bool stop = false;
    while (stop == false){

        bool hasElement = false;
        std::shared_ptr<FileDataChunk> chunk = chunksStorage->Wait(stop, hasElement);

        if(hasElement)
            ProcessChunk(chunk);
    }
}

void PatternSearchManager::ProcessChunk(const std::shared_ptr<FileDataChunk> &Chunk)
{
    const std::vector<char> &chunkData = Chunk->GetData();

    for(size_t i = 0; i < chunkData.size() - pattern.size(); i++){

        int charsMatched = 0;

        for(size_t ii = 0; ii < pattern.size(); ii++)
            charsMatched += chunkData[i + ii] == pattern[ii];
            
        if(charsMatched == pattern.size())
            matchProcessor->AddMatch(Chunk->GetOffset() + i);
    }
}
