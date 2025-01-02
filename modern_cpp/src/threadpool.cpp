#include "threadpool.hpp"

ThreadPool::ThreadPool() : m_done{false}, m_numThreads{std::thread::hardware_concurrency()}
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

/***********************************************************************************************
                                          Private Methods
************************************************************************************************/

void ThreadPool::WorkerThread()
{
    while (!m_done)
    {
        auto currTask = m_workQueue.WaitForPop(std::chrono::milliseconds(100));
        if (currTask)
        {
            (*currTask)();
        }
    }
}
