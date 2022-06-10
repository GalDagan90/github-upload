#include <iostream>

#include "thread_pool.hpp"

using namespace framework;

class SimpleTask : public ThreadPool::Task
{
public:
	virtual void operator()()
	{
		std::cout << "====== Simple task\n";
	}
};

class WaitTask : public ThreadPool::Task
{
public:
	virtual void operator()()
	{
		sleep(5);
		std::cout << "===== Wait task\n";
	}
};


int main()
{
	ThreadPool tPool(4);

	boost::shared_ptr<ThreadPool::Task> sh_ptr(new SimpleTask());
	
	for (int i = 0; i < 10; ++i)
	{
		tPool.AddTask(sh_ptr);
	}
	
	boost::shared_ptr<ThreadPool::Task> sh_ptr2(new WaitTask());
	tPool.AddTask(sh_ptr2);

	tPool.Run();
	sleep(1);
	tPool.Stop();
	std::cout << "===== Until Now 10 simple and 1 wait task finished" << std::endl;

	tPool.SetNumThreads(2);
	
	for (int i = 0; i < 50; ++i)
	{
		tPool.AddTask(sh_ptr);
	}

	std::cout << "===== Run Again" << std::endl;
	tPool.Run();
	//std::cout << "===== Paused" << std::endl;
	// sleep(5);
	//tPool.Run();
	return 0;
}