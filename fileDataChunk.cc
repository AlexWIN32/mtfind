/*******************************************************************************
    Author: Alexey Frolov (alexwin32@mail.ru)

    This software is distributed freely under the terms of the MIT License.
    See "LICENSE" or "http://copyfree.org/content/standard/licenses/mit/license.txt".
*******************************************************************************/

#include "FileDataChunk.h"

FileDataChunk::FileDataChunk(std::vector<char> &&Data, int32_t Offset, size_t PaddingLen)
    : data(std::move(Data)), offset(Offset), paddingLen(PaddingLen)
{}

const std::vector<char> &FileDataChunk::GetData() const
{
    return data;
}

int32_t FileDataChunk::GetOffset() const
{
    return offset;
}

size_t FileDataChunk::GetPaddingLen() const
{
    return paddingLen;
}