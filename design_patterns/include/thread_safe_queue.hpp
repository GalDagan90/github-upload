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
#include <thread>
#include <mutex>

namespace design_pattern
{

template <typename T>
class ThreadSafePQ
{
public:
	ThreadSafePQ()
	~ThreadSafePQ();

	ThreadSafePQ(const ThreadSafePQ& other) = delete;
	ThreadSafePQ& operator=(const ThreadSafePQ& other) = delete;
	ThreadSafePQ(ThreadSafePQ&& other) = delete;
	ThreadSafePQ& operator=(ThreadSafePQ&& other) = delete;

	enum {lowPriority, medPriority, highPriority};

	template <typename U>
	void Push(U&& data, int priority);
	void Pop();
	bool Empty() const;
	std::size_t Size() const;

private:
	std::priority_queue<T> m_pq;
	std::mutex m_mutex;
};



} // namespace ilrd_5678


#endif /* __THREAD_SAFE_QUEUE_H_RD5678_ILRD__ */
