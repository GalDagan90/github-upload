#ifndef __THREADPOOL_GAL_D_90__
#define __THREADPOOL_GAL_D_90__

#include <thread>                   //std::thread
#include <future>                   //std::future, std::promise 
#include <barrier>                  //std::barrier
#include <vector>                   //std::vector
#include <atomic>                   //std::atomic_bool
#include <memory>                   //std::shared_ptr
#include "threadsafe_queue.hpp"
#include "task_wrapper.hpp"

class ThreadPool
{
private:
    std::atomic_bool                                    m_done;
    std::atomic_bool                                    m_paused;
    std::size_t                                         m_numThreads;
    std::barrier<>                                      m_barrier;
    ThreadsafeQueue<std::packaged_task<std::any()>>     m_workQueue;
    std::vector<std::shared_ptr<std::thread>>           m_threads;

public:
    ThreadPool();
    ~ThreadPool();

    ThreadPool(const ThreadPool& other) = delete;
    ThreadPool& operator=(const ThreadPool& other) = delete;
    
    ThreadPool(ThreadPool&& other) = delete;
    ThreadPool& operator=(ThreadPool&& other) = delete;

    std::future<std::any> AddTask(TaskWrapper task);
    void Pause();
    void Resume();


private:
    void WorkerThread();
};



#endif //__THREADPOOL_GAL_D_90__