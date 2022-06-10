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
#include <string>

#include "threadpool.hpp"

using namespace ilrd_5678;

class SimpleTask : public ThreadPool::Task
{
public:
	SimpleTask() : m_str{std::move("Simple Task")} {}

	void operator()()
	{
		std::cout << m_str << std::endl;
	}

private:
	std::string m_str;
};

class ComplexTask : public ThreadPool::Task
{
public:
	ComplexTask() : m_str{std::move("Complex Task")} {}

	void operator()()
	{
		std::cout << m_str << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(4));
	}

private:
	std::string m_str;

};

int main()
{
	ThreadPool pool(3);

	for (size_t i = 0; i < 20; ++i)
	{
		std::shared_ptr<ThreadPool::Task> simple(new SimpleTask);
		pool.AddTask(simple);

		if (i % 3 == 0)
		{
			std::shared_ptr<ThreadPool::Task> complex(new ComplexTask);
			pool.AddTask(complex);
		}
	}

	pool.Run();

	return 0;
}



