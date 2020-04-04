/*******************************************************************************
    Author: Alexey Frolov (alexwin32@mail.ru)

    This software is distributed freely under the terms of the MIT License.
    See "LICENSE" or "http://copyfree.org/content/standard/licenses/mit/license.txt".
*******************************************************************************/

#pragma once
#include <memory>
#include <cstdint>
#include <cstddef>
#include "commonTypes.h"

class PatternMatchProcessor;

class PatternSearchManager
{
public:
    PatternSearchManager(const std::string &Pattern,
                         FileDataChunksStorage *FileDataChunksStorage,
                         PatternMatchProcessor *MatchProcessor,
                         bool HasWildcards);
    void Process();
private:
    bool hasWildcards = false;
    std::string pattern;
    FileDataChunksStorage *chunksStorage;
    PatternMatchProcessor *matchProcessor;
};
