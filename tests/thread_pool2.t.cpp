#include "thread_pool2.t.hpp"

size_t getWorkerIdForCurrentThread()
{
    return *detail::thread_id();
}

template size_t getWorkerIdForCurrentThread2<64>();
template size_t getWorkerIdForCurrentThread2<128>();
