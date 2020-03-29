/*******************************************************************************
    Author: Alexey Frolov (alexwin32@mail.ru)

    This software is distributed freely under the terms of the MIT License.
    See "LICENSE" or "http://copyfree.org/content/standard/licenses/mit/license.txt".
*******************************************************************************/

#pragma once
#include "commonTypes.h"
#include <Utils/FileGuard.h>
#include <vector>
#include <string>
#include <functional>

class PatternMatchProcessor
{
public:
    struct MatchData
    {
        std::wstring filePath;
        std::string line;
        int32_t lineInd = 0;
    };
    using OnMatchProc = std::function<void(const MatchData &)>;
    PatternMatchProcessor(FileDataChunksStorage *FileDataChunksStorage,
                          const std::wstring &FilePath,
                          const OnMatchProc &OnMatchProcedure);
    void AddMatch(int32_t Pos);
    void Process();
private:
    struct LinePosData
    {
        int32_t start = 0, count = 0;
    };
    void ProcessChunk(const std::shared_ptr<FileDataChunk> &Chunk);
    void CheckPendingMatches();
    int32_t FindLineInd(const std::vector<LinePosData> &LinesData, int StartInd, int EndInd, int32_t Ind);
    std::recursive_mutex matchesMutex;
    std::vector<LinePosData> linesData;
    std::vector<int32_t> pendingMatches;
    FileDataChunksStorage *chunksStorage;
    LinePosData lastLine;
    Utils::FileGuard file;
    OnMatchProc onMatchProcedure;
};