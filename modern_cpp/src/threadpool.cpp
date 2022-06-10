/****************************************************************************
*Author : Gal Dagan
*Date : 
*Reviewer :
*
*Description :
*
*Lab : RD5678
****************************************************************************/
#include <iostream>
#include <exception>
#include <algorithm>

#include "threadpool.hpp"

namespace ilrd_5678
{

class StopException : public std::exception
{

};

class ThreadPool::MainRoutine : public ThreadPool::Task
{
public:
	MainRoutine(ThreadPool& pool) : m_pool{pool}
	{
		// empty Ctor
	}

	void operator()() override
	{
		try
		{
			while (true)
			{
				std::shared_ptr<Task> taskPtr;
				JokerTask tasker(taskPtr);
				m_pool.m_queue.Pop(tasker);
				tasker();
			}
		}
		catch(const StopException& e)
		{
			++m_pool.m_stoppedCounter;
			if (m_pool.m_stoppedCounter == m_pool.m_thNum)
			{
				m_pool.m_allDead = true;
			}
		}
	}

private:
	ThreadPool& m_pool;
};

class ThreadPool::PauseTask : public ThreadPool::Task
{
public:
	PauseTask(ThreadPool& pool) : m_pool{pool}
	{
		//empty Ctor
	}

	void operator()() override
	{
		std::unique_lock<std::mutex> lock(m_pool.m_mutex);
		++m_pool.m_pauseCounter;
		if (m_pool.m_pauseCounter == m_pool.m_thNum)
		{
			m_pool.m_allPaused = true;
		}
		m_pool.m_cond.wait(lock);
	}

private:
	ThreadPool& m_pool;
};

class ThreadPool::StopTask : public ThreadPool::Task
{
public:
	StopTask(ThreadPool& pool) : m_pool{pool}
	{
		
	}

	void operator()() override
	{
		throw StopException();
	}

private:
	ThreadPool& m_pool;
};


/******************************************************************************
*							Class ThreadPool    							  *
*******************************************************************************/

ThreadPool::ThreadPool(size_t thNum) 
		: m_thNum{thNum}, m_running{true}, m_queue{}, m_pauseCounter{0}, 
		m_allPaused{false}, m_stoppedCounter{0}, m_allDead{false}
{
	for (size_t i = 0; i < m_thNum; ++i)
	{
		using fut = std::future<void>;
		using ptrToFuture = std::shared_ptr<fut>;
		using namespace std;
		
		ptrToFuture th = make_shared<fut>(async(launch::async, MainRoutine{ref(*this)}));
		m_thrVec.push_back(th);
	}

	AddPauseTasks();
	VerifyPause();
	m_running = false;
}

ThreadPool::~ThreadPool()
{
	Stop();

	for (size_t i = 0; i < m_thNum; ++i)
	{
		std::shared_ptr<Task> taskPtr(new StopTask{*this});
		JokerTask newTask{taskPtr};
		m_queue.Push(newTask);
	}

	Run();

	while (!m_allDead)
	{}
}

void ThreadPool::AddTask(std::shared_ptr<Task> taskPtr)
{
	JokerTask newTask{taskPtr};
	m_queue.Push(newTask);
}

void ThreadPool::Pause()
{
	if (m_running)
	{
		AddPauseTasks();
		VerifyPause();
	}

	m_running = false;
}

void ThreadPool::Run()
{
	if (!m_running)
	{
		m_allPaused = false;
		m_pauseCounter = 0;
		m_running = true;
		m_cond.notify_all();
	}
}

void ThreadPool::Stop()
{
	Pause();

	while (!m_queue.Empty())
	{
		std::shared_ptr<Task> taskPtr;
		JokerTask tasker(taskPtr);
		m_queue.Pop(tasker);
	}
}

/******************************************************************************/
void ThreadPool::AddPauseTasks()
{
	for (size_t i = 0; i < m_thNum; ++i)
	{
		std::shared_ptr<Task> taskPtr(new PauseTask{*this});
		JokerTask newTask{taskPtr};
		m_queue.Push(newTask);
	}
}

void ThreadPool::VerifyPause()
{
	while (!m_allPaused)
	{
		/* code */
	}
}

} // namespace ilrd_5678