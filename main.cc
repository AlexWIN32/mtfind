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
#include "fileReader.h"
#include "patternSearchManager.h"
#include "patternMatchProcessor.h"


int main(char **argc, int argv)
{
    Utils::ParallelDataStorage<int32_t> data;

    std::thread producerThread([&]()
    {
        int32_t ind = 0;
        while(ind < 10){

            std::this_thread::sleep_for(std::chrono::seconds(3));

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
            
            data.Wait(stop, [&](const int32_t &v)
            {
                std::this_thread::sleep_for(std::chrono::seconds(1));

                std::cout << "C " << v << std::endl;    
            });
        }
    });

    
    producerThread.join();
    consumer_thread.join();
    return 0;
}