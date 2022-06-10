/****************************************************************************
*Author : Gal Dagan
*Date : 
*Reviewer :
*
*Description :
*
*Lab : ol108
****************************************************************************/
#ifndef __SINGLETON_H_RD5678_ILRD__
#define __SINGLETON_H_RD5678_ILRD__

#include <boost/noncopyable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/atomic.hpp>

namespace framework
{

template <typename T>
class Singleton : boost::noncopyable
{
public:
	static T *GetInstance();

private:
	static boost::atomic<T *> m_instance;
	static boost::mutex m_mutex;
	static void AtExitFunc();
	static bool m_isCreated;
};


template <typename T>
boost::atomic<T *> Singleton<T>::m_instance(0);

template <typename T>
boost::mutex Singleton<T>::m_mutex;

template <typename T>
bool Singleton<T>::m_isCreated = false;

template<typename T>
void Singleton<T>::AtExitFunc()
{
	T *temp = m_instance.load(boost::memory_order_consume);
	if (0 != m_instance)
	{
		boost::mutex::scoped_lock lock(m_mutex);
		temp = m_instance.load(boost::memory_order_consume);
		if (0 != m_instance)
		{
			delete temp;
			temp = 0;
			m_instance.store(temp, boost::memory_order_release);
			m_isCreated = false;
		}
	}
}


template <typename T>
T *Singleton<T>::GetInstance()
{
	if (m_instance.load(boost::memory_order_consume) == NULL && !m_isCreated)
	{
		boost::mutex::scoped_lock lk(m_mutex);
		if (NULL == m_instance.load(boost::memory_order_consume) && !m_isCreated)
		{
			m_instance.store(new T, boost::memory_order_release);
			std::atexit(AtExitFunc);
			m_isCreated = true;
		}
	}

	return m_instance;
}


}//framework

#endif /* __SINGLETON_H_RD5678_ILRD__ */
