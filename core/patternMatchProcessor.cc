/*******************************************************************************
    Author: Alexey Frolov (alexwin32@mail.ru)

    This software is distributed freely under the terms of the MIT License.
    See "LICENSE" or "http://copyfree.org/content/standard/licenses/mit/license.txt".
*******************************************************************************/

#include "patternMatchProcessor.h"
#include "fileDataChunk.h"
#include <iostream>
#include <cstdint>
#include <cstddef>

PatternMatchProcessor::PatternMatchProcessor(FileDataChunksStorage *FileDataChunksStorage,const OnMatchProc &OnMatchProcedure)
    : chunksStorage(FileDataChunksStorage), onMatchProcedure(OnMatchProcedure)
{
}

void PatternMatchProcessor::AddMatch(const BlockParams &NewMatch)
{
    std::unique_lock<std::recursive_mutex> lock(matchesMutex);

    pendingMatches.push_back(NewMatch);

    CheckPendingMatches();
}

void PatternMatchProcessor::Process()
{
    bool stop = false;
    while (stop == false){

        bool hasElement = false;
        std::shared_ptr<FileDataChunk> chunk = chunksStorage->Wait(stop, hasElement);

        if(hasElement)
            ProcessChunk(chunk);
    }

    linesData.push_back(lastLine);

    CheckPendingMatches();
}

void PatternMatchProcessor::ProcessChunk(const std::shared_ptr<FileDataChunk> &Chunk)
{    
    const std::vector<char> &data = Chunk->GetData();

    for(size_t i = Chunk->GetPaddingLen(); i < data.size(); i++){
        if(data[i] == '\n'){
            linesData.push_back(lastLine);

            CheckPendingMatches();

            lastLine.count = 0;
            lastLine.start = Chunk->GetOffset() + (i - Chunk->GetPaddingLen()) + 1;

        }else
            lastLine.count++;
    }
}

void PatternMatchProcessor::CheckPendingMatches()
{
    std::unique_lock<std::recursive_mutex> lock(matchesMutex);

    for(auto it = pendingMatches.begin(); it != pendingMatches.end();){

        const BlockParams &match = *it;
        int32_t ind = FindLineInd(linesData, 0, linesData.size() - 1, match.start);

        if(ind != -1){

            MatchData matchData;
            matchData.lineInd = ind + 1;
            matchData.lineParams = linesData[ind];
            matchData.matchParams = match;

            if(onMatchProcedure)
                onMatchProcedure(matchData);

            it = pendingMatches.erase(it);
        }else
            it++;
    }
}

int32_t PatternMatchProcessor::FindLineInd(const std::vector<BlockParams> &Data,int StartInd, int EndInd, int32_t Ind)
{
    if(EndInd >= StartInd){
        int mid = StartInd + (EndInd - StartInd) / 2;
        int32_t start = Data[mid].start;
        int32_t end = start + Data[mid].count;

        if(start > Ind && end > Ind)
            return FindLineInd(Data, StartInd, mid - 1, Ind);
        else if(start < Ind && end < Ind)
            return FindLineInd(Data, mid + 1, EndInd, Ind);
        else
            return mid;
    }

    return -1;
}