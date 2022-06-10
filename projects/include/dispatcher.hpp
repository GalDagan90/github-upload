/****************************************************************************
*Author : Gal Dagan
*Date : 5.1.22
*Reviewer : Oz
*
*Description : Dispatcher Implementation
*
*Lab : RD5678
****************************************************************************/
#ifndef __DISPATCHER_H_RD5678_ILRD__
#define __DISPATCHER_H_RD5678_ILRD__

#include <vector>	//std::vector
#include <iterator>	//std::iterator, std::const_iterator
#include <boost/noncopyable.hpp>	//boost::non_copyable
#include <boost/function.hpp>		//boost::function


namespace framework
{

template <typename DataType>
class AbstractObserver;

template <typename DataType>
class Observer;

template <typename DataType>
class Dispatcher;
/******************************************************************************/
template <typename DataType>
class AbstractObserver : private boost::noncopyable
{
private:
    explicit AbstractObserver(Dispatcher<DataType>& dispatcher);
    virtual ~AbstractObserver();

    virtual void RecieveNotice(DataType) const = 0;
    virtual void RecieveDeathNotice() = 0;

    friend class Observer<DataType>;
    friend class Dispatcher<DataType>;

private:
    Dispatcher<DataType>* m_dispatcher;
};
/******************************************************************************/
template <typename DataType>
class Observer : public AbstractObserver<DataType>
{
public:
	typedef boost::function<void (DataType data)> notify_func_t;
	typedef boost::function<void ()> death_func_t;

	explicit Observer(Dispatcher<DataType>& dispatcher, notify_func_t notifyFunc
					, death_func_t notifyKilledFunc);

private:
	virtual void RecieveNotice(DataType data) const;
	virtual void RecieveDeathNotice();

	notify_func_t m_notifyFunc;
	death_func_t m_notifyKilledFunc;
};
/******************************************************************************/
template <typename DataType>
class Dispatcher : private boost::noncopyable
{
public:
	explicit Dispatcher();
	~Dispatcher();

    void NotifyAll(DataType data) const;

private:
	void Subscribe(AbstractObserver<DataType> *observer);
	void UnSubscribe(AbstractObserver<DataType> *observer);

	friend class AbstractObserver<DataType>;

private:
	std::vector<AbstractObserver<DataType> *> m_observerList;
};

/*******************************************************************************
 * 								class Dispatcher							   *
 ******************************************************************************/

template <typename DataType>
Dispatcher<DataType>::Dispatcher()
{
	//empty func
}

template <typename DataType>
Dispatcher<DataType>::~Dispatcher()
{
	typename std::vector<AbstractObserver<DataType> *>::const_iterator it;

	for (it = m_observerList.begin(); it != m_observerList.end(); ++it)
	{
		(*it)->RecieveDeathNotice();
	}
}

template <typename DataType>
void Dispatcher<DataType>::NotifyAll(DataType data) const
{
	typename std::vector<AbstractObserver<DataType> *>::const_iterator it;

	for (it = m_observerList.begin(); it != m_observerList.end(); ++it)
	{
		(*it)->RecieveNotice(data);
	}
}

template <typename DataType>
void Dispatcher<DataType>::Subscribe(AbstractObserver<DataType> *observer)
{
	m_observerList.push_back(observer);
}

template <typename DataType>
void Dispatcher<DataType>::UnSubscribe(AbstractObserver<DataType> *observer)
{
	typename std::vector<AbstractObserver<DataType> *>::iterator it;

	it = std::find(m_observerList.begin(), m_observerList.end(), observer);

	if (it != m_observerList.end())
	{
		m_observerList.erase(it);
	}
}

/*******************************************************************************
 * 							class AbstractObserver							   *
 ******************************************************************************/
template <typename DataType>
AbstractObserver<DataType>::AbstractObserver(Dispatcher<DataType>& dispatcher) 
	: m_dispatcher(&dispatcher) 
{
	m_dispatcher->Subscribe(this);
}

template <typename DataType>
AbstractObserver<DataType>::~AbstractObserver()
{
	if (NULL != m_dispatcher)
	{
		m_dispatcher->UnSubscribe(this);
	}
}

template <typename DataType>
void AbstractObserver<DataType>::RecieveDeathNotice()
{
	m_dispatcher = NULL;
}
/*******************************************************************************
 * 								class Observer								   *
 ******************************************************************************/
template <typename DataType>
Observer<DataType>::Observer(Dispatcher<DataType>& dispatcher, notify_func_t notifyFunc
					, death_func_t notifyKilledFunc) : 
											AbstractObserver<DataType>(dispatcher),
											m_notifyFunc(notifyFunc),
											m_notifyKilledFunc(notifyKilledFunc)
{
	//empty
}

template <typename DataType>
void Observer<DataType>::RecieveNotice(DataType data) const
{
	m_notifyFunc(data);
}


template <typename DataType>
void Observer<DataType>::RecieveDeathNotice() 
{
	AbstractObserver<DataType>::RecieveDeathNotice();
	m_notifyKilledFunc();
}

/******************************************************************************/

}//framework

#endif /* __DISPATCHER_H_RD5678_ILRD__ */
