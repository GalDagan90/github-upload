#ifndef __GAL_DAGAN_PROJ_THREAD_SAFE_Q__
#define __GAL_DAGAN_PROJ_THREAD_SAFE_Q__

#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <memory>
#include <chrono>

template <typename T, typename Q = std::queue<T>>
class ThreadSafeQueue
{
private:
    Q m_queue;
    mutable std::mutex m_mutex;
    std::condition_variable m_cond_var;

public:
    ThreadSafeQueue() = default;
    ~ThreadSafeQueue() = default;
    ThreadSafeQueue(const ThreadSafeQueue& other) = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue& other) = delete;

    ThreadSafeQueue(ThreadSafeQueue&& other) = delete;
    ThreadSafeQueue& operator=(ThreadSafeQueue&& other) = delete;

    bool empty() const;
    std::size_t size() const;
    
    void push(T val);

    void wait_and_pop(T& res);
    std::shared_ptr<T> wait_and_pop();
    
    void wait_and_pop_w_timeout(T& res, std::chrono::milliseconds time);
    std::shared_ptr<T> wait_and_pop_w_timeout(std::chrono::milliseconds time);
    
    void try_pop(T& res);
    std::shared_ptr<T> try_pop();
};

/********************************************************************************/
/*                        Implementation Detail                                 */
/********************************************************************************/

template <typename T, typename Q>
bool ThreadSafeQueue<T, Q>::empty() const
{
    std::lock_guard<std::mutex> lk(m_mutex);
    return m_queue.empty();
}

template <typename T, typename Q>
std::size_t ThreadSafeQueue<T, Q>::size() const
{
    std::lock_guard<std::mutex> lk(m_mutex);
    return m_queue.size();
}

template <typename T, typename Q>
void ThreadSafeQueue<T, Q>::push(T val)
{
    std::lock_guard<std::mutex> lk(m_mutex);
    m_queue.push(std::move(val));
    m_cond_var.notify_one();
}


#endif //"__GAL_DAGAN_PROJ_THREAD_SAFE_Q__"