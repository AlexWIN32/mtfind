/*******************************************************************************
    Author: Alexey Frolov (alexwin32@mail.ru)

    This software is distributed freely under the terms of the MIT License.
    See "LICENSE" or "http://copyfree.org/content/standard/licenses/mit/license.txt".
*******************************************************************************/

#include "patternSearchManager.h"
#include "fileDataChunk.h"
#include "patternMatchProcessor.h"
#include <Utils/RabinKarpSearch.h>
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

        if(hasElement){

            std::list<size_t> matches;
            Utils::RabinKarpSearch(chunk->GetData(), pattern, matches, 101);

            for(size_t m : matches)
                matchProcessor->AddMatch(chunk->GetOffset() + m);
        }
    }
}
