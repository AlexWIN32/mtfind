/*******************************************************************************
    Author: Alexey Frolov (alexwin32@mail.ru)

    This software is distributed freely under the terms of the MIT License.
    See "LICENSE" or "http://copyfree.org/content/standard/licenses/mit/license.txt".
*******************************************************************************/

#pragma once
#include <string>

class PatternMatchProcessor
{
public:
    void AddMatch(const std::string &line, uint32_t lineInd, size_t PosInLine);
};