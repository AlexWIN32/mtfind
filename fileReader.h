/*******************************************************************************
    Author: Alexey Frolov (alexwin32@mail.ru)

    This software is distributed freely under the terms of the MIT License.
    See "LICENSE" or "http://copyfree.org/content/standard/licenses/mit/license.txt".
*******************************************************************************/

#pragma once
#include <string>
#include "commonTypes.h"

class FileDataChunk;

class FileReader
{
public:
    FileReader(const std::wstring &Path,
               size_t PaddingLength,
               size_t ChunkLength,
               FileDataChunksStorage *SearchManagerChunksStorage,
               FileDataChunksStorage *MatchProcessorChunksStorage);
    FileReader(const FileReader&) = delete;
    FileReader &operator= (const FileReader&) = delete;
    FileReader(const FileReader&&) = delete;
    FileReader &operator= (const FileReader&&) = delete;
    void Process();
private:
    std::wstring path;
    size_t paddingLength, chunkLength;
    FileDataChunksStorage *searchManagerChunksStorage;
    FileDataChunksStorage *matchProcessorChunksStorage;
};