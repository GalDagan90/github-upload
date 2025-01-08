#include <algorithm>
#include "threadpool.hpp"

ThreadPool::ThreadPool() :  m_done{false}, m_paused{false}, m_stoped{false}, m_numThreads{std::thread::hardware_concurrency()}, m_countSem{0}
{
    m_syncPoint.emplace(m_numThreads);
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
    m_paused.store(true, std::memory_order_relaxed);

    VerifyPause();
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
    
    this->Pause();

    if (num < m_numThreads)
    {
        ReduceThreadVecSize(num);
    }
    else
    {
        //IncreaseThreadVecSize(num);
    }

    m_syncPoint.emplace(num);
    m_numThreads = num;

    this->Resume();

    return true;
}
/***********************************************************************************************
                                          Private Methods
************************************************************************************************/

void ThreadPool::WorkerThread()
{
    while (!m_done)
    {        
        if (m_paused)
        {
            if (m_syncPoint)
            {
                m_syncPoint->arrive_and_wait();
                {                
                    std::unique_lock<std::mutex> lk(m_mutex);
                    m_countSem.release();
                    m_cond.wait(lk);
                }
            }
        }

        auto currTask = m_workQueue.WaitForPop(std::chrono::milliseconds(250));
        if (currTask)
        {
            (*currTask)();
        }

        if (m_stoped.load(std::memory_order_acquire))
        {
            auto it =
                std::find_if(m_threads.begin(), m_threads.end(), 
                            [current_thread_id = std::this_thread::get_id()](const auto& thread) {
                                return thread->get_id() == current_thread_id;
                            });
            
            if (it != m_threads.end()){
                m_deadThreadsQueue.Push_Back(*it);
            }
            
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
    m_stoped.store(true, std::memory_order_release);
    for (std::size_t i = m_numThreads; i > num; --i)
    {
        m_cond.notify_one();
        
        //waits pop blocks until the queue is occupied
        auto threadToKill = m_deadThreadsQueue.WaitPop();
        (*threadToKill)->join();
    }
    
    if (m_deadThreadsQueue.IsEmpty())
        m_stoped.store(false, std::memory_order_release);
}