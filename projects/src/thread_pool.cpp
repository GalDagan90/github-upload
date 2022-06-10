/******************************************************************************
 * Author: Gal Dagan                                                          *
 * Reviewer: Tal Fiderer                                                      *
 * Date: 26.12.2021            	                                              *
 *                                                                            *
 * Description:                                                               *
 * ThreadPool implementation.                                                 *
 *                                                                            *
 * RD5678                                                                     *
 ******************************************************************************/

#include <algorithm>		/* std::iterator, std::find_if() */
#include <exception>		/* std::exception	*/
#include <sys/time.h>		/* setpriority()	*/
#include <sys/resource.h>	/* setpriority()	*/

#include "thread_pool.hpp"

namespace framework
{

bool is_same(boost::shared_ptr<boost::thread> t)
{
	return (*t).get_id() == boost::this_thread::get_id();
}

class KillMe : public std::exception
{
	//empty class
};

/*******************************************************************************
*							class ThreadRoutine								   *
*******************************************************************************/
class ThreadPool::ThreadRoutine
{
public:
	ThreadRoutine(ThreadPool& threadPool) : m_pool(threadPool)
	{
		//empty ctor
	}

	virtual void operator()()
	{
		setpriority(PRIO_PROCESS, gettid(), m_pool.m_nice);
		try
		{
			while (true)
			{
				boost::shared_ptr<ThreadPool::Task> task_ptr;
				PQTask pqtask(task_ptr, 0);
				m_pool.m_taskQueue.Pop(pqtask);
				pqtask();
			}
		}
		catch(const KillMe& e)
		{
			std::vector<boost::shared_ptr<boost::thread> >::iterator it = 
				std::find_if (m_pool.m_threadVec.begin(), m_pool.m_threadVec.end(), is_same);
			m_pool.m_deadThreadQ.Push(*it);
		}
	}

private:
	ThreadPool& m_pool;
};

/*******************************************************************************
*							class TaskPause									   *
*******************************************************************************/
class ThreadPool::TaskPause : public Task
{
public:
	TaskPause(ThreadPool& threadPool): m_pool(threadPool)
	{
		//empty ctor
	}
	void operator()()
	{
		boost::mutex::scoped_lock lk(m_pool.m_mutex);
		m_pool.m_sem.post();
		m_pool.m_cond.wait(lk);
	}

private:
	ThreadPool& m_pool;
};

/*******************************************************************************
*							class BadApple									   *
*******************************************************************************/
class ThreadPool::BadApple : public Task
{
public:
	BadApple(ThreadPool& threadPool): m_pool(threadPool)
	{
		//empty ctor
	}
	
	void operator()()
	{
		throw KillMe();
	}

private:
	ThreadPool& m_pool;
};

/*******************************************************************************
*							class BadApple									   *
*******************************************************************************/
class ThreadPool::GoodApple : public Task
{
public:
	GoodApple(ThreadPool& threadPool): m_pool(threadPool)
	{
		//empty ctor
	}
	
	void operator()()
	{
		boost::mutex::scoped_lock lock(m_pool.m_mutex);
		m_pool.m_semLastTask.post();
	}

private:
	ThreadPool& m_pool;
};

/*******************************************************************************
*							Class ThreadPool								   *
*******************************************************************************/

ThreadPool::ThreadPool(size_t threadNum, int nice) 
	: m_nice(nice), m_running(true), m_numThreads(threadNum), m_sem(0),
	m_lastTask(false), m_semLastTask(0)
{
	AddThreads(m_numThreads);
	AddPauseTasks(m_numThreads);
	VerifyPause(m_numThreads);
	m_running = false;
}


ThreadPool::~ThreadPool()
{
	boost::shared_ptr<Task> task(new GoodApple(*this));
	AddTask(task, static_cast<Priority>(LOW - 1));

	m_semLastTask.wait();
	
	if (!m_running)
	{
		Run();
	}

	SetNumThreads(0);
}

void ThreadPool::AddTask(boost::shared_ptr<Task> task, Priority priority)
{
	m_taskQueue.Push(PQTask(task, priority));
}

void ThreadPool::Pause()
{
	if (m_running)
	{
		AddPauseTasks(m_numThreads);
		VerifyPause(m_numThreads);

		boost::mutex::scoped_lock lock(m_mutex);
		m_running = false;
	}
}

void ThreadPool::Run()
{
	{
		boost::mutex::scoped_lock lock(m_mutex);
		m_running = true;
	}

	m_cond.notify_all();
}

void ThreadPool::Stop()
{
	Pause();

	while (!m_taskQueue.Empty())
	{
		boost::shared_ptr<ThreadPool::Task> task_ptr;
		PQTask pqtask(task_ptr, 0);
		m_taskQueue.Pop(pqtask);
	}
}

void ThreadPool::SetNumThreads(size_t num)
{
	if (m_numThreads > num)
	{
		RemoveThreads(m_numThreads - num);
	}
	else
	{
		AddThreads(num - m_numThreads);
	}

	m_numThreads = num;
}

/*******************************************************************************
*						ThreadPool class helper functions					   *
*******************************************************************************/

void ThreadPool::AddPauseTasks(size_t threadNum)
{
	for (size_t i = 0; i < threadNum; ++i)
	{
		boost::shared_ptr<Task> task(new TaskPause(*this));
		m_taskQueue.Push(PQTask(task, static_cast<Priority>(HIGH + 1)));
	}
}

void ThreadPool::VerifyPause(size_t threadNum)
{
	for (size_t i = 0; i < threadNum; ++i)
	{
		m_sem.wait();
	}
}

void ThreadPool::AddThreads(size_t numThreadsToAdd)
{
	for (size_t i = 0; i < numThreadsToAdd; ++i)
	{
		boost::shared_ptr<boost::thread> thr_ptr(new boost::thread(ThreadRoutine(*this)));
		m_threadVec.push_back(thr_ptr);
	}
}

void ThreadPool::RemoveThreads(size_t numThreadsToRemove)
{
	for (; numThreadsToRemove != 0; --numThreadsToRemove)
	{
		boost::shared_ptr<Task> task(new BadApple(*this));
		AddTask(task, static_cast<Priority>(HIGH + 1));
		
		if (!m_running)
		{
			m_cond.notify_one();
		}

		boost::shared_ptr<boost::thread> threadToKill;
		m_deadThreadQ.Pop(threadToKill);
		(*threadToKill).join();
	}
}
/*******************************************************************************
*							class PQTask									   *
*******************************************************************************/

ThreadPool::PQTask::PQTask(boost::shared_ptr<Task> task, int priority)
	: m_task(task), m_priority(priority)
{
	//empty ctor body
}

bool ThreadPool::PQTask::operator<(const PQTask& other) const
{
	return (m_priority < other.m_priority);
}

void ThreadPool::PQTask::operator()()
{
	(*m_task)();
}

} //framework