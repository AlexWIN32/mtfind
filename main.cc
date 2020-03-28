/*******************************************************************************
    Author: Alexey Frolov (alexwin32@mail.ru)

    This software is distributed freely under the terms of the MIT License.
    See "LICENSE" or "http://copyfree.org/content/standard/licenses/mit/license.txt".
*******************************************************************************/

#include <Utils/FileGuard.h>
#include <Utils/ParallelDataStorage.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include "fileReader.h"
#include "patternSearchManager.h"
#include "patternMatchProcessor.h"

int FindInsertPos(const std::vector<float> &Data, int StartInd, int EndInd, float Val) 
{ 
    if (EndInd >= StartInd) { 
        int mid = StartInd + (EndInd - StartInd) / 2; 
  
        if (Data[mid] > Val) 
            return FindInsertPos(Data, StartInd, mid - 1, Val); 
        else if (Data[mid] < Val)
            return FindInsertPos(Data, mid + 1, EndInd, Val); 
        else
            return mid;
    } 
  
    return EndInd; 
}

int main(char **argc, int argv)
{
    std::vector<float> data = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};

    int ind = FindInsertPos(data, 0, data.size() - 1, 0.5f);

    std::string pattern = "TT";
    std::wstring filePath = L"./fileReaderTest1.txt";
    size_t chunkLength = 256;

    FileDataChunksStorage matchProcessorChunks, searchManagerChunks;

    FileReader fileReader(filePath, pattern.size() - 1, chunkLength, &searchManagerChunks, &matchProcessorChunks);
    PatternMatchProcessor patternMatchProcessor(&matchProcessorChunks, filePath);
    PatternSearchManager patternSearchManager(pattern, &searchManagerChunks, &patternMatchProcessor);

    std::thread readThread(std::bind(&FileReader::Process, &fileReader));
    std::thread searchThread(std::bind(&PatternSearchManager::Process, &patternSearchManager));
    std::thread processThread(std::bind(&PatternMatchProcessor::Process, &patternMatchProcessor));

    readThread.join();
    searchThread.join();
    processThread.join();
    /*
    Utils::ParallelDataStorage<int32_t> data;

    std::thread producerThread([&]()
    {
        int32_t ind = 0;
        while(ind < 10){

            std::this_thread::sleep_for(std::chrono::seconds(1));

            std::cout << "P " << ind << std::endl;

            data.Add(ind);

            ind++;
        }

        data.StopAdding();
    });

    std::thread consumer_thread([&]()
    {
        bool stop = false;
        while(!stop){
            
            bool hasElement;
            int32_t v = data.Wait(stop, hasElement);

            if(hasElement){
                std::this_thread::sleep_for(std::chrono::seconds(5));

                std::cout << "C " << v << std::endl;    
            }
        }
    });
    
    producerThread.join();
    consumer_thread.join();
    */
    return 0;
}