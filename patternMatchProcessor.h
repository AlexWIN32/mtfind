/*******************************************************************************
    Author: Alexey Frolov (alexwin32@mail.ru)

    This software is distributed freely under the terms of the MIT License.
    See "LICENSE" or "http://copyfree.org/content/standard/licenses/mit/license.txt".
*******************************************************************************/

#pragma once
#include "commonTypes.h"
#include <vector>
#include <string>

class PatternMatchProcessor
{
public:
    PatternMatchProcessor(FileDataChunksStorage *FileDataChunksStorage, const std::wstring &FilePath);
    void AddMatch(int32_t Pos);
    void Process();
private:
    void ProcessChunk(const std::shared_ptr<FileDataChunk> &Chunk);
    void CheckPendingMatches();
    struct LinePosData
    {
        int32_t start = 0, count = 0;
    };
    std::recursive_mutex matchesMutex;
    std::vector<LinePosData> linesData;
    std::vector<int32_t> pendingMatches;
    FileDataChunksStorage *chunksStorage;
    std::wstring filePath;
    LinePosData lastLine;
};