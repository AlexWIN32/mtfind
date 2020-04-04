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

static void SearchWithWildcards(const std::vector<char> &Text,
                                               const std::string &Pattern,
                                               std::list<size_t> &Matches)
{
    if(Text.size() >= Pattern.size()){
        for(size_t i = 0; i <= Text.size() - Pattern.size(); i++){

            size_t cnt = 0;
            for(size_t ii = 0; ii < Pattern.size(); ii++)
                if(Text[i + ii] == Pattern[ii] || Pattern[ii] == '?')
                    cnt ++;

            if(cnt == Pattern.size())
                Matches.push_back(i);
        }
    }
}

PatternSearchManager::PatternSearchManager(const std::string &Pattern,
                                           FileDataChunksStorage *FileDataChunksStorage,
                                           PatternMatchProcessor *MatchProcessor,
                                           bool HasWildcards)
    : pattern(Pattern),
      chunksStorage(FileDataChunksStorage),
      matchProcessor(MatchProcessor),
      hasWildcards(HasWildcards)
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

            if(hasWildcards)
                SearchWithWildcards(chunk->GetData(), pattern, matches);
            else
                Utils::RabinKarpSearch(chunk->GetData(), pattern, matches, 101);

            for(size_t m : matches){

                BlockParams newMatch;
                newMatch.start = chunk->GetOffset() + m;
                newMatch.count = pattern.size();

                matchProcessor->AddMatch(newMatch);
            }
        }
    }
}
