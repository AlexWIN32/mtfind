/*******************************************************************************
    Author: Alexey Frolov (alexwin32@mail.ru)

    This software is distributed freely under the terms of the MIT License.
    See "LICENSE" or "http://copyfree.org/content/standard/licenses/mit/license.txt".
*******************************************************************************/

#pragma once
#include <vector>

class FileDataChunk
{
public:
    FileDataChunk(std::vector<char> &&Data, int32_t Offset, size_t PaddingLen);
    FileDataChunk(const FileDataChunk&) = delete;
    FileDataChunk &operator= (const FileDataChunk&) = delete;
    const std::vector<char> &GetData() const;
    int32_t GetOffset() const;
    size_t GetPaddingLen() const;
private:
    std::vector<char> data;
    int32_t offset = 0;
    size_t paddingLen = 0;
};
