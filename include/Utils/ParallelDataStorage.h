/*******************************************************************************
    Author: Alexey Frolov (alexwin32@mail.ru)

    This software is distributed freely under the terms of the MIT License.
    See "LICENSE" or "http://copyfree.org/content/standard/licenses/mit/license.txt".
*******************************************************************************/

#pragma once
#include <mutex>
#include <deque>
#include <condition_variable>

namespace Utils
{

template<class TData>
class ParallelDataStorage
{
private:
    std::deque<TData> data;
    std::mutex m;
    std::condition_variable condVar;
    bool adding = true;
public:
    void Add(const TData &NewElement)
    {
        std::unique_lock<std::mutex> lock(m);

        data.push_back(NewElement);

        condVar.notify_all();
    }
    void Add(TData &&NewElement)
    {
        std::unique_lock<std::mutex> lock(m);

        data.push_back(std::move(NewElement));

        condVar.notify_all();
    }
    void StopAdding()
    {
        std::unique_lock<std::mutex> lock(m);

        adding = false;

        condVar.notify_all();
    }
    TData Wait(bool &Stop, bool &HasElement)
    {
        TData elem;
        HasElement = false;

        {
            std::unique_lock<std::mutex> lock(m);

            if(adding)
                condVar.wait(lock);

            if(data.size() != 0){

                elem = data.front();
                data.pop_front();

                HasElement = true;
            }

            Stop = !adding && data.size() == 0;
        }

        return elem;
    }
};

}