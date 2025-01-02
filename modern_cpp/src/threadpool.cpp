#include "threadpool.hpp"

ThreadPool::ThreadPool() :  m_done{false}, m_paused{false}, m_numThreads{std::thread::hardware_concurrency()}, 
                            m_barrier{static_cast<std::ptrdiff_t>(m_numThreads)} 
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
    std::packaged_task<std::any()> packagedTask([task]() mutable { return task.Invoke();});
    std::future<std::any> result = packagedTask.get_future();
    m_workQueue.Emplace_Back(std::move(packagedTask));

    return result;
}

void ThreadPool::Pause()
{
    m_paused.store(true, std::memory_order_relaxed);
}

void ThreadPool::Resume()
{
    m_paused.store(true, std::memory_order_relaxed);
    m_barrier.arrive_and_wait();
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
            m_barrier.arrive_and_wait();
        }

        auto currTask = m_workQueue.WaitForPop(std::chrono::milliseconds(250));
        if (currTask)
        {
            (*currTask)();
        }
    }
}
