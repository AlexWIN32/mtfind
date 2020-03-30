/*******************************************************************************
    Author: Alexey Frolov (alexwin32@mail.ru)

    This software is distributed freely under the terms of the MIT License.
    See "LICENSE" or "http://copyfree.org/content/standard/licenses/mit/license.txt".
*******************************************************************************/
#include "FileReader.h"
#include "FileDataChunk.h"
#include <Utils/FileGuard.h>
#include <vector>

FileReader::FileReader(const std::wstring &Path,
                       size_t PaddingLength,
                       size_t ChunkLength,
                       FileDataChunksStorage *SearchManagerChunksStorage,
                       FileDataChunksStorage *MatchProcessorChunksStorage)
    : path(Path),
      paddingLength(PaddingLength),
      chunkLength(ChunkLength),
      searchManagerChunksStorage(SearchManagerChunksStorage),
      matchProcessorChunksStorage(MatchProcessorChunksStorage)
{
}

void FileReader::Process()
{    
    Utils::FileGuard file(path, L"rb");

    std::vector<char> buffer(chunkLength + paddingLength);

    size_t fileOffset = 0;

    while(file.Eof() == false){
        
        size_t chunkDataLen = 0, readed = 0;

        if(fileOffset == 0){
            readed = file.ReadArray(buffer.data(), chunkLength + paddingLength);
            chunkDataLen = readed;
        }else{
            std::copy(buffer.end() - paddingLength, buffer.end(), buffer.begin());

            readed = file.ReadArray(buffer.data() + paddingLength, chunkLength);
            chunkDataLen = readed + paddingLength;
        }

        if(readed != 0){

            std::vector<char> chunkData(buffer.begin(), buffer.begin() + chunkDataLen);

            std::shared_ptr<FileDataChunk> newChunk(std::make_shared<FileDataChunk>(std::move(chunkData), fileOffset, (fileOffset == 0) ? 0 : paddingLength));

            searchManagerChunksStorage->Add(newChunk);
            matchProcessorChunksStorage->Add(newChunk);
        }

        fileOffset += readed;
    }

    searchManagerChunksStorage->StopAdding();
    matchProcessorChunksStorage->StopAdding();
}