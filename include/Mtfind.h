/*******************************************************************************
    Author: Alexey Frolov (alexwin32@mail.ru)

    This software is distributed freely under the terms of the MIT License.
    See "LICENSE" or "http://copyfree.org/content/standard/licenses/mit/license.txt".
*******************************************************************************/

#pragma once
#include <string>
#include <functional>

struct MatchData
{
    std::wstring filePath;
    std::string line;
    int32_t lineInd = 0;
};

using OnMatchProc = std::function<void(const MatchData &)>;

void mtfind(const std::wstring &FilePath, const std::string &Pattern, size_t ChunkLength, const OnMatchProc &MatchProcedure);