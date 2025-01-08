#include <algorithm>
#include "threadpool.hpp"

/***********************************************************************/
struct ThreadPool::PauseTask
{
    PauseTask(ThreadPool& _threadpool) : m_pool{_threadpool} {}

    void operator()()
    {
        std::unique_lock<std::mutex> lk(m_pool.m_mutex);
        m_pool.m_countSem.release();
        m_pool.m_cond.wait(lk);
    }

private:
    ThreadPool& m_pool;
};

struct ThreadPool::StopTask
{
    StopTask(ThreadPool& _threadpool) : m_pool{_threadpool} {}

    void operator()()
    {
        auto it = std::find_if(m_pool.m_threads.begin(), m_pool.m_threads.end(), 
                                [current_thread_id = std::this_thread::get_id()](const auto& thread){
                                    return thread->get_id() == current_thread_id;
                                });
            
        if (it != m_pool.m_threads.end()){
            m_pool.m_deadThreadsQueue.Push_Back(*it);
        }
    }

private:
    ThreadPool& m_pool;
};

/********************************************************************** */
/*                          public methods                              */                        
/********************************************************************** */
ThreadPool::ThreadPool() :  m_done{false}, m_paused{false}, m_stoped{false}, m_numThreads{std::thread::hardware_concurrency()}, m_countSem{0}
{
    for (std::size_t i = 0; i < m_numThreads; ++i)
    {
        auto shPtr = std::make_shared<std::thread>(&ThreadPool::WorkerThread, this);
        m_threads.emplace_back(shPtr);
    }
}

ThreadPool::~ThreadPool()
{
    m_done = true;
    for (auto threadPtr : m_threads)
    {
        if (threadPtr->joinable())
            threadPtr->join();
    }
}


std::future<std::any> ThreadPool::AddTask(TaskWrapper task)
{
    std::packaged_task<std::any()> packagedTask([task]() mutable { return task.Invoke(); });
    std::future<std::any> result = packagedTask.get_future();
    m_workQueue.Emplace_Back(std::move(packagedTask));

    return result;
}

void ThreadPool::Pause()
{
    for (std::size_t i = 0; i < m_numThreads; ++i)
    {
        TaskWrapper tw(PauseTask(*this));
        AddTask(tw);
    }
    
    VerifyPause();
    m_paused.store(true, std::memory_order_release);
}

void ThreadPool::Resume()
{
    if (m_paused.load(std::memory_order_acquire))
    {
        m_paused.store(false, std::memory_order_release);
        m_cond.notify_all();
    }
}

bool ThreadPool::ChangeNumWorkinThreads(const unsigned int num)
{
    if (num == 0 || num > std::thread::hardware_concurrency() || num == m_numThreads)
        return false;
    
    if (num < m_numThreads)
    {
        ReduceThreadVecSize(num);
    }
    else
    {
        IncreaseThreadVecSize(num);
    }

    m_numThreads = num;
    
    return true;
}
/***********************************************************************************************
                                          Private Methods
************************************************************************************************/

void ThreadPool::WorkerThread()
{
    while (!m_done)
    {   
        auto currTask = m_workQueue.WaitForPop(std::chrono::milliseconds(250));
        if (currTask)
        {
            (*currTask)();
        }

        if (m_stoped.load(std::memory_order_acquire))
        {
            auto stopTask = m_workQueue.TryPop(); (std::chrono::milliseconds(250));
            (*stopTask)();
            break;
        }      
    }
}

void ThreadPool::VerifyPause()
{
    for (std::size_t i = 0; i < m_numThreads; ++i)
    {
        m_countSem.acquire();
    }
}

void ThreadPool::ReduceThreadVecSize(const unsigned int num)
{
    Pause();
    m_stoped.store(true, std::memory_order_release);
    for (std::size_t i = m_numThreads; i > num; --i)
    {
        TaskWrapper tw(StopTask(*this));
        AddTask(tw);

        m_cond.notify_one();
        
        //waits pop blocks until the queue is occupied
        auto threadToKill = m_deadThreadsQueue.WaitPop();
        (*threadToKill)->join();
    }
    
    m_stoped.store(false, std::memory_order_release);
    Resume();
}

void ThreadPool::IncreaseThreadVecSize(const unsigned int num)
{
    for(std::size_t i = m_numThreads; i < num; ++i)
    {
        auto ShPtrToThrd = std::make_shared<std::thread>(&ThreadPool::WorkerThread, this);
        m_threads.emplace_back(ShPtrToThrd);
    }
}
