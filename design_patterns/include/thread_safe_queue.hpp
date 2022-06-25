/****************************************************************************
*Author : Gal Dagan
*Date : 
*Reviewer :
*
*Description :
*
*Lab : RD5678
****************************************************************************/
#ifndef __THREAD_SAFE_QUEUE_H_RD5678_ILRD__
#define __THREAD_SAFE_QUEUE_H_RD5678_ILRD__

#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>

namespace design_pattern
{

enum class Priority {LOW, MEDIUM, HIGH};

template <typename T>
class ThreadSafePQ
{
public:
	ThreadSafePQ();

	~ThreadSafePQ() = default;
	ThreadSafePQ(const ThreadSafePQ& other) = delete;
	ThreadSafePQ& operator=(const ThreadSafePQ& other) = delete;
	ThreadSafePQ(ThreadSafePQ&& other) = delete;
	ThreadSafePQ& operator=(ThreadSafePQ&& other) = delete;

	void Push(T&& data, Priority priority);
	T Pop();
	bool Empty() const;

private:
	using Pair = std::pair<T, Priority>;
	using UniqueLock = std::unique_lock<std::mutex>;

	struct Compare
	{
		template <typename U>
		bool operator()(const U& lhs, const U& rhs)
		{
			return lhs.second < rhs.second;
		}
	};

	std::priority_queue<Pair, std::vector<Pair>, Compare> m_pq;
	mutable std::mutex m_pqMutex;
	std::condition_variable m_cond;
};

template <typename T>
ThreadSafePQ<T>::ThreadSafePQ() : m_pq{}, m_pqMutex{}, m_cond{}
{
	//empty Ctor
}

template <typename T>
bool ThreadSafePQ<T>::Empty() const
{
	UniqueLock lock{m_pqMutex};
	return m_pq.empty();
}

template <typename T>
void ThreadSafePQ<T>::Push(T&& data, Priority priority)
{
	Pair pq_pair = std::make_pair(std::forward<T>(data), priority);

	UniqueLock lock{m_pqMutex};
	m_pq.push(std::move(pq_pair));
	m_cond.notify_one();
}

template <typename T>
T ThreadSafePQ<T>::Pop()
{
	UniqueLock lock{m_pqMutex};

	while (m_pq.empty())
	{
		m_cond.wait(lock);
	}

	auto front_pair = m_pq.top();
	m_pq.pop();
	return front_pair.first;
}

} // namespace design_pattern


#endif /* __THREAD_SAFE_QUEUE_H_RD5678_ILRD__ */
