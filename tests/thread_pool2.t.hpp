#ifndef THREAD_POOL_2_T_HPP
#define THREAD_POOL_2_T_HPP 1

#include <worker.hpp>

size_t getWorkerIdForCurrentThread();

template <size_t STORAGE_SIZE>
size_t getWorkerIdForCurrentThread2()
{
    return Worker<STORAGE_SIZE>::getWorkerIdForCurrentThread();    
}

#endif // THREAD_POOL_2_T_HPP 1
