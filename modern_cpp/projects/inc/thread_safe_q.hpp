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
    
    template<typename... Args>
    void emplace_back(Args&&... args);

    void wait_and_pop(T& res);
    std::shared_ptr<T> wait_and_pop();
    
    void wait_and_pop_w_timeout(T& res, std::chrono::milliseconds time);
    std::shared_ptr<T> wait_and_pop_w_timeout(std::chrono::milliseconds time);
    
    bool try_pop(T& res);
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


template <typename T, typename Q>
template<typename... Args>
void ThreadSafeQueue<T, Q>::emplace_back(Args&&... args)
{
    std::lock_guard<std::mutex> lk(m_mutex);
    m_queue.emplace_back(std::forward<Args>(args)...);
    m_cond_var.notify_one();
}

template <typename T, typename Q>
void ThreadSafeQueue<T, Q>::wait_and_pop(T& res)
{
    std::unique_lock<std::mutex> lk(m_mutex);
    m_cond_var.wait(lk, [this](){ return !m_queue.empty();});
    res = std::move(m_queue.front());
    m_queue.pop();
}

template <typename T, typename Q>
std::shared_ptr<T> ThreadSafeQueue<T, Q>::wait_and_pop()
{
    std::unique_lock<std::mutex> lk(m_mutex);
    m_cond_var.wait(lk, [this](){ return !m_queue.empty();});
    auto res{std::make_shared<T>(std::move(m_q.front()))}
    m_q.pop();

    return res;
}

template <typename T, typename Q>
bool ThreadSafeQueue<T, Q>::try_pop(T& res)
{
    std::lock_guard<std::mutex> lk(m_mutex);
    
    if (m_queue.empty())
    {    
        return false;
    }

    res = std::move(m_queue.front());
    m_queue.pop();
    
    return true;
}

template <typename T, typename Q>
std::shared_ptr<T> ThreadSafeQueue<T, Q>::try_pop()
{
    std::lock_guard<std::mutex> lk(m_mutex);
    
    if (m_queue.empty())
    {    
        return std::shared_ptr<T>();
    }

    auto res{std::make_shared<T>(std::move(m_queue.front()))};
    m_queue.pop();
    
    return res;
}



#endif //"__GAL_DAGAN_PROJ_THREAD_SAFE_Q__"