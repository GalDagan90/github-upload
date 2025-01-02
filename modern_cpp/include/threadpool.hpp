#ifndef __THREADPOOL_GAL_D_90__
#define __THREADPOOL_GAL_D_90__

#include <thread>                   //std::thread
#include <future>                   //std::future, std::promise 
#include <vector>                   //std::vector
#include <atomic>                   //std::atomic_bool
#include <memory>                   //std::shared_ptr
#include "threadsafe_queue.hpp"
#include "task_wrapper.hpp"

class ThreadPool
{
private:
    std::atomic_bool                                    m_done;
    std::size_t                                         m_numThreads;
    ThreadsafeQueue<std::packaged_task<std::any()>>     m_workQueue;
    std::vector<std::shared_ptr<std::thread>>           m_threads;

public:
    ThreadPool();
    ~ThreadPool();

    std::future<std::any> AddTask(TaskWrapper task);


private:
    void WorkerThread();
};



#endif //__THREADPOOL_GAL_D_90__