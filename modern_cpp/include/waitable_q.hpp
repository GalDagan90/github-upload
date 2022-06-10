/****************************************************************************
*Author : Gal Dagan
*Date : 
*Reviewer :
*
*Description :
*
*Lab : RD5678
****************************************************************************/
#ifndef __WAITABLE_Q_H_RD5678_ILRD__
#define __WAITABLE_Q_H_RD5678_ILRD__

#include <queue>
#include <mutex>
#include <condition_variable>

namespace ilrd_5678
{

template <typename T, class Q = std::queue<T>>
class WaitableQ
{
public:
	explicit WaitableQ() = default;
	WaitableQ(const WaitableQ& _other) = delete;
	WaitableQ& operator=(const WaitableQ& _other) = delete;

	WaitableQ(WaitableQ&& _other);

	template <typename U>
	void Push(U&& data);
	void Pop(T& retVal);
	bool Empty() const;

private:
	Q m_q;
	mutable std::mutex m_mutex;
	std::condition_variable m_canPop;
};

/******************************************************************************/

template <typename T, class Q>
WaitableQ<T,Q>::WaitableQ(WaitableQ&& _other)
{
	m_q = std::move(_other.m_q);
	m_mutex = std::move(_other.m_mutex);
}

template <typename T, class Q>
template <typename U>
void WaitableQ<T,Q>::Push(U&& data)
{
	std::unique_lock<std::mutex> lk(m_mutex);
	m_q.push(std::forward<U>(data));
	m_canPop.notify_one();
}

template <typename T, class Q>
void WaitableQ<T,Q>::Pop(T& retVal)
{
	std::unique_lock<std::mutex> lk(m_mutex);
	
	while (m_q.empty())
	{
		auto timeOut = m_canPop.wait_for(lk, std::chrono::seconds(5));

		if (timeOut == std::cv_status::timeout)
		{
			return;
		}
	}

	retVal = m_q.front();
	m_q.pop();
}

template <typename T, class Q>
bool WaitableQ<T,Q>::Empty() const
{
	std::unique_lock<std::mutex> lk(m_mutex);
	return (m_q.empty());
}

} // namespace ilrd_5678


#endif /* __WAITABLE_Q_H_RD5678_ILRD__ */
