/****************************************************************************
*Author : Gal Dagan
*Date : 
*Reviewer :
*
*Description :
*
*Lab : RD5678
****************************************************************************/
#ifndef __THREADPOOL_H_RD5678_ILRD__
#define __THREADPOOL_H_RD5678_ILRD__

#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <atomic>
#include <vector>

#include "waitable_q.hpp"

namespace ilrd_5678
{

class ThreadPool
{
public:
	class Task;

	explicit ThreadPool(size_t thNum = std::thread::hardware_concurrency());
	~ThreadPool();

	ThreadPool(const ThreadPool& _other) = delete;
	ThreadPool& operator=(const ThreadPool& _other) = delete;

	void AddTask(std::shared_ptr<Task> taskPtr);
	void Pause();
	void Run();
	void Stop();

private:
	class JokerTask;

	size_t m_thNum;
	std::atomic_bool m_running;
	WaitableQ<JokerTask> m_queue;
	std::vector<std::shared_ptr<std::future<void>>> m_thrVec;
	std::mutex m_mutex;
	std::condition_variable m_cond;
	std::atomic<size_t> m_pauseCounter;
	std::atomic_bool m_allPaused;
	std::atomic<size_t> m_stoppedCounter;
	std::atomic_bool m_allDead;

	class MainRoutine;
	class PauseTask;
	class StopTask;

	void AddPauseTasks();
	void VerifyPause();
};

class ThreadPool::Task
{
public:
	virtual ~Task() {};
	virtual void operator()() = 0;
};

class ThreadPool::JokerTask
{
public:
	JokerTask(std::shared_ptr<ThreadPool::Task> task) : m_task{task} {}

	void operator()()
	{
		(*m_task)();
	}

private:
	std::shared_ptr<ThreadPool::Task> m_task;

};


} // namespace ilrd_5678


#endif /* __THREADPOOL_H_RD5678_ILRD__ */
