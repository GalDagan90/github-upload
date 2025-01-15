#ifndef __THREADPOOL_GAL_D_90__
#define __THREADPOOL_GAL_D_90__

#include <thread>                   //std::thread
#include <stop_token>               //std::stop_token
#include <future>                   //std::future
#include <semaphore>                //std::semaphore
#include <condition_variable>       //std::condition_variable
#include <mutex>                    //std::mutex
#include <vector>                   //std::vector
#include <atomic>                   //std::atomic_bool
#include <memory>                   //std::shared_ptr
#include <optional>                 //std::optional
#include "threadsafe_queue.hpp"
#include "task_wrapper.hpp"

class ThreadPool
{
private:
    std::atomic_bool                                    m_done;
    std::atomic_bool                                    m_paused;
    std::atomic_bool                                    m_stoped;
    std::size_t                                         m_numThreads;
    std::counting_semaphore<>                           m_countSem;
    std::condition_variable                             m_cond;
    std::mutex                                          m_mutex;
    ThreadsafeQueue<std::packaged_task<std::any()>>     m_workQueue;
    std::vector<std::jthread>                           m_threads;

public:
    ThreadPool();
    ~ThreadPool();

    ThreadPool(const ThreadPool& other) = delete;
    ThreadPool& operator=(const ThreadPool& other) = delete;
    
    ThreadPool(ThreadPool&& other) noexcept = default;
    ThreadPool& operator=(ThreadPool&& other) = default;

    std::future<std::any> AddTask(TaskWrapper task);
    void Pause();
    void Resume();
    bool ChangeNumWorkingThreads(const unsigned int num);
    
    inline std::size_t GetNumWorkingThreads() const
    {
        return m_numThreads;
    }

private:
    void WorkerThread(std::stop_token stopToken);
    void VerifyPause();
    void ReduceThreadVecSize(const unsigned int num);
    void IncreaseThreadVecSize(const unsigned int num);

    class PauseTask;
    class StopTask;
};



#endif //__THREADPOOL_GAL_D_90__