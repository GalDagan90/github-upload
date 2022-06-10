/******************************************************************************
 * Author: Gal Dagan                                                          *
 * Reviewer: Tal Fiderer                                                      *
 * Date: 26.12.2021            	                                              *
 *                                                                            *
 * Description:                                                               *
 * ThreadPool class.		                                                  *
 *                                                                            *
 * RD5678                                                                     *
 ******************************************************************************/

#ifndef __THREAD_POOL_H_RD5678__
#define __THREAD_POOL_H_RD5678__

#include <vector>
#include <queue>
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>
#include <boost/function.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>

#include "waitable_queue.hpp"

namespace framework
{

class ThreadPool : private boost::noncopyable  
{
public:
    class Task;

    enum Priority
    {
        LOW,
        MEDIUM,
        HIGH
    };

    explicit ThreadPool(size_t threadNum = boost::thread::hardware_concurrency(),
                         int nice = 0);
    ~ThreadPool();

    void AddTask(boost::shared_ptr<Task> task, Priority priority = LOW);
    void Pause();
    void Run();
    void Stop();
    void SetNumThreads(size_t num);

private:
    class PQTask;

	WaitableQueue< PQTask, PriorityQueue<PQTask> > m_taskQueue;
	std::vector< boost::shared_ptr<boost::thread> > m_threadVec;
	int m_nice;
	bool m_running;
	size_t m_numThreads;
	boost::mutex m_mutex;
	boost::condition_variable m_cond;
	boost::interprocess::interprocess_semaphore m_sem;
	bool m_lastTask;
	boost::interprocess::interprocess_semaphore m_semLastTask;
	WaitableQueue<boost::shared_ptr<boost::thread>, std::queue< boost::shared_ptr<boost::thread> > > m_deadThreadQ;

	class ThreadRoutine;
	class TaskPause;
	class BadApple;
	class GoodApple;

	void AddPauseTasks(size_t threadNum);
	void VerifyPause(size_t threadNum);
	void AddThreads(size_t numThreadsToAdd);
	void RemoveThreads(size_t numThreadsToRemove);
};

class ThreadPool::Task
{
public:
    virtual ~Task() {};
    virtual void operator()() = 0;
};

class ThreadPool::PQTask
{
public:
    explicit PQTask(boost::shared_ptr<Task> task, int priority);

    bool operator<(const PQTask& other) const;
	void operator()();

private:
    boost::shared_ptr<Task> m_task;
    int m_priority;
};

} // framework

#endif /* __THREAD_POOL_H_RD5678__ */