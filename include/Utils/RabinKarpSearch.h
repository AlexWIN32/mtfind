/*******************************************************************************
    taken from https://www.geeksforgeeks.org/rabin-karp-algorithm-for-pattern-searching/
*******************************************************************************/

#pragma once
#include <limits>
#include <list>

namespace Utils
{

template<class TData, class TPattern>
void RabinKarpSearch(const TData &Data, const TPattern &Pattern, std::list<size_t> &Matches, int64_t Prime)
{
    const size_t CHARS_RANGE = std::numeric_limits<typename TData::value_type>::max();

    size_t paternLen = Pattern.size();
    size_t dataLen = Data.size();

    int64_t patternHash = 0; // hash value for pattern
    int64_t textRollingHash = 0; // hash value for txt

    if (paternLen > dataLen)
        return;

    int64_t h = 1;
    // The value of h would be "pow(CHARS_RANGE, paternLen-1) % prime"
    for (size_t i = 0; i < paternLen - 1; i++)
        h = (h * CHARS_RANGE) % Prime;

    // Calculate the hash value of pattern and first window of text
    for (size_t i = 0; i < paternLen; i++){
        patternHash = (CHARS_RANGE * patternHash + Pattern[i]) % Prime;
        textRollingHash = (CHARS_RANGE * textRollingHash + Data[i]) % Prime;
    }

    // Slide the pattern over text one by one
    for (size_t i = 0; i <= dataLen - paternLen; i++){
        // Check the hash values of current window of text
        // and pattern. If the hash values match then only
        // check for characters on by one
        if (patternHash == textRollingHash)
        {
            // Check for characters one by one
            size_t j = 0;
            for (; j < paternLen; j++)
                if (Data[i+j] != Pattern[j])
                    break;

            // if patternHash == textRollingHash and pattern[0...paternLen-1] = Data[i, i+1, ...i+paternLen-1
            if (j == paternLen)
                Matches.push_back(i);
        }

        // Calculate hash value for next window of text: Remove
        // leading digit, add trailing digit
        if (i < dataLen - paternLen){
            char charToRemove = Data[i];
            char charToAdd = Data[i + paternLen];

            textRollingHash = (textRollingHash - charToRemove * h) * CHARS_RANGE + charToAdd;
            textRollingHash %= Prime;

            if(textRollingHash < 0)
                textRollingHash += Prime;
        }
    }
}

}