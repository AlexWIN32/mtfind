/*******************************************************************************
    Author: Alexey Frolov (alexwin32@mail.ru)

    This software is distributed freely under the terms of the MIT License.
    See "LICENSE" or "http://copyfree.org/content/standard/licenses/mit/license.txt".
*******************************************************************************/

#include "patternMatchProcessor.h"
#include "FileDataChunk.h"
#include <Utils/ToString.h>
#include <iostream>

PatternMatchProcessor::PatternMatchProcessor(FileDataChunksStorage *FileDataChunksStorage, const std::wstring &FilePath)
    : chunksStorage(FileDataChunksStorage), file(FilePath, L"rb")
{
}

void PatternMatchProcessor::AddMatch(int32_t Pos)
{
    std::unique_lock<std::recursive_mutex> lock(matchesMutex);

    pendingMatches.push_back(Pos);

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

    for(std::vector<int32_t>::iterator it = pendingMatches.begin(); it != pendingMatches.end();){
        
        bool found = false;
        for(size_t l = 0; l < linesData.size(); l++){
            const LinePosData &ld = linesData[l];

            if(ld.start < *it && ld.start + ld.count > *it){
                std::string line(ld.count, '\0');

                file.SetPos(ld.start);
                file.ReadArray(&line[0], line.size());

                std::wcout << file.GetPath() << L":" << (l + 1) << L":"<<ld.start << L":"<<ld.count<< L" " << Utils::ToWString(line) << std::endl;
                found = true;
                break;
            }
        }

        if(found)
            it = pendingMatches.erase(it);
        else 
            it++;
    }

}