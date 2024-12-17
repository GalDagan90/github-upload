#ifndef __THREADSAFE_LIST_GAL_271190__
#define __THREADSAFE_LIST_GAL_271190__

#include <mutex>                //std::mutex
#include <condition_variable>   //std::condition_variable
#include <memory>               //std::unique_ptr, std::shared_ptr
#include <type_traits>          //std::is_move_constructible_v<T>
#include <chrono>               //std::chrono
#include <iostream>

template <typename T>
class ThreadsafeQueue
{
private:
    struct Node
    {
    public:
        Node() : m_nextNode()
        {}

        explicit Node(const T& data) : m_data{std::make_shared<T>(data)}, m_nextNode{std::make_unique<Node>()}
        {}

        std::shared_ptr<T> m_data;
        std::unique_ptr<Node> m_nextNode;
    };

    std::unique_ptr<Node>       m_head;
    std::mutex                  m_headMutex;
    Node*                       m_tail;
    std::mutex                  m_tailMutex;
    std::condition_variable     m_cv;

    Node* GetTail();
    std::unique_ptr<Node> TryPopHead();
    std::unique_ptr<Node> WaitPopHead();
    std::unique_ptr<Node> WaitForPopHead(const std::chrono::milliseconds duration);

public:
    ThreadsafeQueue() : m_head{new Node}, m_tail{m_head.get()} {}

    ~ThreadsafeQueue() = default;
    
    ThreadsafeQueue(const ThreadsafeQueue& other) = delete;
    ThreadsafeQueue& operator=(const ThreadsafeQueue& other) = delete;
    
    ThreadsafeQueue(ThreadsafeQueue&& other) = delete;
    ThreadsafeQueue& operator=(ThreadsafeQueue&& other) = delete;

    bool IsEmpty();

    void Push_Back(const T& data);

    template<typename... Args>
    requires std::is_move_constructible_v<T>
    void Emplace_Back(Args&&... args);

    //Waits indefinitely 
    std::shared_ptr<T> WaitPop();
    bool WaitPop(T& result);

    //Try to pop value immedietly, if fails return nullptr or false
    std::shared_ptr<T> TryPop();
    bool TryPop(T& result);

    //Waits for a set amount of time
    std::shared_ptr<T> WaitForPop(const std::chrono::milliseconds duration = std::chrono::milliseconds(100));
    bool WaitForPop(T& result, const std::chrono::milliseconds duration = std::chrono::milliseconds(100));
};

/**************************************************************************************
 *                               Public member functions
***************************************************************************************/


template<typename T>
bool ThreadsafeQueue<T>::IsEmpty()
{
    std::lock(m_headMutex, m_tailMutex);  // Atomically lock both mutexes
    std::lock_guard<std::mutex> headLock(m_headMutex, std::adopt_lock);
    std::lock_guard<std::mutex> tailLock(m_tailMutex, std::adopt_lock);

    return m_head.get() == m_tail;
}


template<typename T>
void ThreadsafeQueue<T>::Push_Back(const T& data)
{
    std::shared_ptr<T> newData = std::make_shared<T>(data);
    std::unique_ptr<Node> newNode = std::make_unique<Node>();
    {
        std::lock_guard<std::mutex> tailLock(m_tailMutex);
        m_tail->m_data = newData;
        Node* const newTail = newNode.get();
        m_tail->m_nextNode = std::move(newNode);
        m_tail = newTail;
    }
    m_cv.notify_one();
}

template<typename T>
template<typename... Args>
requires std::is_move_constructible_v<T>
void ThreadsafeQueue<T>::Emplace_Back(Args&&... args)
{
    std::shared_ptr<T> newData = std::make_shared<T>(std::forward<Args>(args)...);
    std::unique_ptr<Node> newNode = std::make_unique<Node>();
    {
        std::lock_guard<std::mutex> tailLock(m_tailMutex);
        m_tail->m_data = newData;
        Node* const newTail = newNode.get();
        m_tail->m_nextNode = std::move(newNode);
        m_tail = newTail;
    }
    m_cv.notify_one();
}


template<typename T>
std::shared_ptr<T> ThreadsafeQueue<T>::TryPop()
{
    std::unique_ptr<Node> oldHead = TryPopHead();
    return oldHead ? oldHead->m_data : std::shared_ptr<T>();
}

template<typename T>
bool ThreadsafeQueue<T>::TryPop(T& result)
{
    std::unique_ptr<Node> oldHead = TryPopHead();
    if (oldHead != nullptr)
        result = std::move(*oldHead->m_data);

    return nullptr != oldHead;
}

template<typename T>
std::shared_ptr<T> ThreadsafeQueue<T>::WaitPop()
{
    std::unique_ptr<Node> oldHead = WaitPopHead();
    return oldHead ? oldHead->m_data : std::shared_ptr<T>();
}

template<typename T>
bool ThreadsafeQueue<T>::WaitPop(T& result)
{
    std::unique_ptr<Node> oldHead = WaitPopHead();
    if (oldHead != nullptr)
        result = std::move(*oldHead->m_data);

    return oldHead != nullptr;
}

template<typename T>
std::shared_ptr<T> ThreadsafeQueue<T>::WaitForPop(const std::chrono::milliseconds duration)
{
    std::unique_ptr<Node> oldHead = WaitForPopHead(duration);
    return oldHead ? oldHead->m_data : std::shared_ptr<T>();
}

template<typename T>
bool ThreadsafeQueue<T>::WaitForPop(T& result, const std::chrono::milliseconds duration)
{
    std::unique_ptr<Node> oldHead = WaitForPopHead(duration);
    if (oldHead != nullptr)
        result = std::move(*oldHead->m_data);
    
    return nullptr != oldHead;
}

/**************************************************************************************
 *                               Private member functions
***************************************************************************************/

template<typename T>
ThreadsafeQueue<T>::Node* ThreadsafeQueue<T>::GetTail()
{
    std::lock_guard<std::mutex> tailLock(m_tailMutex);
    return m_tail;
}


template<typename T>
std::unique_ptr<typename ThreadsafeQueue<T>::Node> ThreadsafeQueue<T>::TryPopHead()
{
    std::lock_guard<std::mutex> headLock(m_headMutex);
    
    if (m_head.get() == GetTail())
    {
        return std::unique_ptr<Node>();
    }

    std::unique_ptr<Node> oldHead = std::move(m_head);
    m_head = std::move(oldHead->m_nextNode);
    return oldHead;
}


template<typename T>
std::unique_ptr<typename ThreadsafeQueue<T>::Node> ThreadsafeQueue<T>::WaitPopHead()
{
    std::unique_lock<std::mutex> headLock(m_headMutex);
    m_cv.wait(headLock, [&](){ return m_head.get() != GetTail();});
    std::unique_ptr<Node> oldHead = std::move(m_head);
    m_head = std::move(oldHead->m_nextNode);
    return oldHead;
}

template<typename T>
std::unique_ptr<typename ThreadsafeQueue<T>::Node> ThreadsafeQueue<T>::WaitForPopHead(const std::chrono::milliseconds duration)
{
    std::unique_lock<std::mutex> headLock(m_headMutex);
    auto result = m_cv.wait_for(headLock, duration, [&](){ return m_head.get() != GetTail();});

    if (result)
    {
        std::unique_ptr<Node> oldHead = std::move(m_head);
        m_head = std::move(oldHead->m_nextNode);
        return oldHead;    
    }
    
    return nullptr;
}

#endif //#ifndef __THREADSAFE_LIST_GAL_271190__