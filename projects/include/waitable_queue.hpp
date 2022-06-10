/******************************************************************************
 * Author : Gal Dagan
 * Date : 24.12.21
 * Reviewer: Keren Robbins
 * 
 * Description : WaitableQueue implementation
 * 
 * RD_5678
 *****************************************************************************/ 

#ifndef __WAITABLE_QUEUE_H_RD5678__
#define __WAITABLE_QUEUE_H_RD5678__

#include <queue>
#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/noncopyable.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

namespace framework 
{

template <typename T, class Q = std::queue<T> >
class WaitableQueue : private boost::noncopyable
{
private:
	typedef boost::chrono::milliseconds Millisec;
public:
	//  WaitableQueue(); = generated
    // ~WaitableQueue(); = generated

	void Push(const T& elem);
	void Pop(T& ret_val);
	bool Pop(T& ret_val, Millisec millisec);
	bool Empty() const;

private:
	Q m_queue;
	mutable boost::mutex m_lock;
	boost::condition_variable m_canPop;
};

template <typename T>
class PriorityQueue : private boost::noncopyable
{
public:
	void push(const T& elem)
	{
		m_pq.push(elem);
	}

	void pop()
	{
		m_pq.pop();
	}
	
	const T& front() const
	{
		return m_pq.top();
	}

	bool empty() const
	{
		return m_pq.empty();
	}

private:
	std::priority_queue<T> m_pq;
};

/*******************************************************************************
*							Ctors and Dtors									   *
*******************************************************************************/



/*******************************************************************************
*					WaitableQueue Member Functions							   *
*******************************************************************************/
template <typename T, class Q>
void WaitableQueue<T,Q>::Push(const T& elem)
{
	boost::mutex::scoped_lock lk(m_lock);
	m_queue.push(elem);
	m_canPop.notify_one();
}

template <typename T, class Q>
bool WaitableQueue<T,Q>::Empty() const
{
	boost::mutex::scoped_lock lk(m_lock);
	return m_queue.empty();
}

template <typename T, class Q>
void WaitableQueue<T,Q>::Pop(T& ret_val)
{
	boost::mutex::scoped_lock lk(m_lock);

	while (m_queue.empty())
	{
		m_canPop.wait(lk);
	}

	ret_val = m_queue.front();
	m_queue.pop();
}

template <typename T, class Q>
bool WaitableQueue<T,Q>::Pop(T& ret_val, Millisec millisec)
{
	boost::mutex::scoped_lock lk(m_lock);

	while(m_queue.empty())
	{
		if (boost::cv_status::timeout == m_canPop.wait_for(lk, millisec))
		{
			return false;
		}
	}

	ret_val = m_queue.front();
	m_queue.pop();

	return true;
}

}//namespace framework

#endif /* __WAITABLE_QUEUE_H_RD5678__ */
