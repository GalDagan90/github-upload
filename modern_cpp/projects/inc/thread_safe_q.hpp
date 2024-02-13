#ifndef __GAL_DAGAN_PROJ_THREAD_SAFE_Q__
#define __GAL_DAGAN_PROJ_THREAD_SAFE_Q__

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

public:
    ThreadSafeQueue() = default;
    ~ThreadSafeQueue() = default;
    ThreadSafeQueue(const ThreadSafeQueue& other);
    ThreadSafeQueue& operator=(const ThreadSafeQueue& other);

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





#endif //"__GAL_DAGAN_PROJ_THREAD_SAFE_Q__"