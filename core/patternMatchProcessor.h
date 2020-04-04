/*******************************************************************************
    Author: Alexey Frolov (alexwin32@mail.ru)

    This software is distributed freely under the terms of the MIT License.
    See "LICENSE" or "http://copyfree.org/content/standard/licenses/mit/license.txt".
*******************************************************************************/

#pragma once
#include "commonTypes.h"
#include <Utils/FileGuard.h>
#include <Mtfind.h>
#include <vector>
#include <string>
#include <cstdint>
#include <cstddef>

class PatternMatchProcessor
{
public:
    PatternMatchProcessor(FileDataChunksStorage *FileDataChunksStorage, const OnMatchProc &OnMatchProcedure);
    void AddMatch(const BlockParams &NewMatch);
    void Process();
private:
    void ProcessChunk(const std::shared_ptr<FileDataChunk> &Chunk);
    void CheckPendingMatches();
    int32_t FindLineInd(const std::vector<BlockParams> &LinesData, int StartInd, int EndInd, int32_t Ind);
    std::recursive_mutex matchesMutex;
    std::vector<BlockParams> linesData;
    std::vector<BlockParams> pendingMatches;
    FileDataChunksStorage *chunksStorage;
    BlockParams lastLine;
    OnMatchProc onMatchProcedure;
};